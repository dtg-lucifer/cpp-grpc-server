#include "service/order_service.hpp"

#include <grpcpp/support/status.h>
#include <time.h>

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <mutex>
#include <random>
#include <thread>

#include "order_service/order.pb.h"

namespace osv1 = order_service::v1;

using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;

// ---------------------------------------------------------------------------
// Public methods
// ---------------------------------------------------------------------------
//
// Initialise the class with some mock data to store
OrderService::OrderService() {
    osv1::Item item1;
    item1.set_id(this->generate_id());
    item1.set_name("Laptop");
    item1.set_price(100.5);
    item1.set_quantity(1);

    osv1::Order order1;
    order1.set_id(this->generate_id());
    order1.set_amount(100.5);
    order1.set_status(osv1::OrderStatus::COMPLETED);
    order1.set_address("123 Maple Street");
    order1.set_created_at(this->get_current_timestamp());
    order1.add_items()->CopyFrom(item1);

    osv1::Item item2;
    item2.set_id(this->generate_id());
    item2.set_name("Smartphone");
    item2.set_price(34.0);
    item2.set_quantity(2);

    osv1::Order order2;
    order2.set_id(this->generate_id());
    order2.set_amount(134.5);
    order2.set_status(osv1::OrderStatus::PENDING);
    order2.set_address("567 Wallnut street");
    order2.set_created_at(this->get_current_timestamp());
    order2.add_items()->CopyFrom(item2);

    this->orders_[order1.id()] = order1;
    this->orders_[order2.id()] = order2;

    this->user_orders_["user1"].push_back(order1);
    this->user_orders_["user1"].push_back(order2);
}

Status OrderService::GetOrder(ServerContext* ctx, const osv1::GetOrderRequest* request, osv1::GetOrderResponse* response) {
    std::lock_guard<std::mutex> lock(this->mutex_);

    std::string order_id = request->order_id();
    auto it = orders_.find(order_id);

    if (it != orders_.end()) {
        response->mutable_order()->CopyFrom(this->orders_[order_id]);
        return Status::OK;
    } else {
        return Status(grpc::NOT_FOUND, "Order not found");
    }
}

Status OrderService::ListOrders(ServerContext* ctx, const osv1::ListOrdersRequest* request, osv1::ListOrdersResponse* response) {
    std::lock_guard<std::mutex> lock(this->mutex_);

    std::string user_id = request->user_id();                  // Get the user id from the request object
    int limit = request->limit() > 0 ? request->limit() : 10;  // Default limit to 10 if not specified
    int page = request->page() > 0 ? request->page() : 1;      // Default page to 1 if not specified
    auto filters = request->filters();                         // Get the filters from the request object

    auto it = user_orders_.find(user_id);

    response->set_total(it->second.size());
    if (it != user_orders_.end()) {
        for (const auto& order : it->second) {
            response->add_orders()->CopyFrom(order);
        }
        return Status::OK;
    } else {
        return Status(grpc::NOT_FOUND, "No orders found for user");
    }
}

Status OrderService::CreateOrder(ServerContext* ctx, const osv1::CreateOrderRequest* request, osv1::CreateOrderResponse* response) {
    std::lock_guard<std::mutex> lock(this->mutex_);

    std::string user_id = request->user_id();  // Get the user id from the request object
    osv1::Order new_order = request->order();  // Get the order from the request object

    new_order.set_id(this->generate_id());
    new_order.set_status(osv1::OrderStatus::PENDING);
    new_order.set_created_at(this->get_current_timestamp());
    new_order.set_updated_at(this->get_current_timestamp());

    double total_amount = 0.0;
    for (const auto& item : new_order.items()) {
        total_amount += item.price() * item.quantity();  // Calculating the total amount of all of the items
    }

    new_order.set_amount(total_amount);  // Settings the total amount

    this->orders_[new_order.id()] = new_order;
    this->user_orders_[user_id].push_back(new_order);

    response->mutable_order()->CopyFrom(new_order);

    return Status::OK;
}

Status OrderService::UpdateOrder(ServerContext* ctx, const osv1::UpdateOrderResponse* request, osv1::UpdateOrderRequest* response) {
    std::lock_guard<std::mutex> lock(this->mutex_);

    osv1::Order order = request->order();  // Get the order from the request object
    std::string order_id = order.id();     // Get the order id from the order object

    if (this->orders_.find(order_id) == this->orders_.end()) {
        return Status(grpc::NOT_FOUND, "Order not found");
    }

    this->orders_[order_id] = order;  // Update the order in the orders map

    response->mutable_order()->CopyFrom(order);  // Copy the updated order to the response
    return Status::OK;
}

Status OrderService::DeleteOrder(ServerContext* ctx, const osv1::DeleteOrderRequest* request, osv1::DeleteOrderResponse* response) {
    std::lock_guard<std::mutex> lock(this->mutex_);

    std::string order_id = request->order_id();  // Get the order id from the request object

    if (this->orders_.find(order_id) == this->orders_.end()) {
        return Status(grpc::NOT_FOUND, "Order not found");
    }

    this->orders_.erase(order_id);  // Remove the order from the orders map
    response->set_success(true);    // Indicate that the deletion was successful

    return Status::OK;
}

Status OrderService::StreamOrderUpdates(ServerContext* ctx, const osv1::StreamOrderUpdatesRequest* request, ServerWriter<osv1::StreamOrderUpdatesResponse>* writer) {
    const std::string order_id = request->order_id();  // Get the user id from the request object
    {
        std::lock_guard<std::mutex> lock(this->mutex_);

        if (order_id.empty()) {
            return Status(grpc::INVALID_ARGUMENT, "Order ID cannot be empty");
        }

        auto it = this->orders_.find(order_id);
        if (it == this->orders_.end()) {
            return Status(grpc::NOT_FOUND, "Order not found");
        }

        osv1::StreamOrderUpdatesResponse response;
        *(response.mutable_order()) = it->second;  // Copy the order to the response
        response.set_status(osv1::UpdateStatus::CREATED);
        response.set_updated_at(this->get_current_timestamp());

        if (!writer->Write(response)) {
            return Status(grpc::CANCELLED, "Stream cancelled by client");
        }
    }

    int update_count = 0;
    while (!ctx->IsCancelled() && update_count < 5) {
        std::this_thread::sleep_for(std::chrono::seconds(5));

        std::lock_guard<std::mutex> lock(this->mutex_);

        if (!order_id.empty()) {
            auto it = this->orders_.find(order_id);
            if (it != this->orders_.end()) {
                osv1::Order& order = it->second;
                osv1::OrderStatus current_status = order.status();

                if (current_status == osv1::OrderStatus::PENDING) {
                    order.set_status(osv1::OrderStatus::PROCESSING);
                } else if (current_status == osv1::OrderStatus::PROCESSING) {
                    order.set_status(osv1::OrderStatus::SHIPPED);
                } else if (current_status == osv1::OrderStatus::SHIPPED) {
                    order.set_status(osv1::OrderStatus::DELIVERED);
                }

                if (current_status != order.status()) {
                    order.set_updated_at(this->get_current_timestamp());

                    osv1::StreamOrderUpdatesResponse response;
                    *(response.mutable_order()) = order;  // Copy the updated order to the response
                    response.set_status(osv1::UpdateStatus::STATUS_CHANGED);
                    response.set_updated_at(this->get_current_timestamp());

                    if (!writer->Write(response)) {
                        break;
                    }

                    update_count++;
                }
            }
        }
    }

    return Status::OK;
}

// ---------------------------------------------------------------------------
// Private methods
// ---------------------------------------------------------------------------
int64_t OrderService::get_current_timestamp() {
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string OrderService::generate_id() {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    static std::uniform_int_distribution<> dis(0, 16);
    static const char* hex = "123456789abcdef";

    std::string uuid;
    for (int i = 0; i < 32; i++) {
        uuid += hex[dis(gen)];
        if (i == 7 || i == 11 || i == 15 || i == 19) {
            uuid += '-';
        }
    }

    return uuid;
}
