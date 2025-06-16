#pragma once

#include <grpcpp/grpcpp.h>
#include <grpcpp/server_context.h>
#include <grpcpp/support/status.h>

#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "google/protobuf/map.h"
#include "order_service/order.grpc.pb.h"
#include "order_service/order.pb.h"

namespace osv1 = order_service::v1;

using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;

class OrderService final : public osv1::OrderService::Service {
   public:
    OrderService();

    Status GetOrder(ServerContext* context, const osv1::GetOrderRequest* request, osv1::GetOrderResponse* response) override;
    Status ListOrders(ServerContext* context, const osv1::ListOrdersRequest* request, osv1::ListOrdersResponse* response) override;
    Status CreateOrder(ServerContext* context, const osv1::CreateOrderRequest* request, osv1::CreateOrderResponse* response) override;
    Status UpdateOrder(ServerContext* context, const osv1::UpdateOrderResponse* request, osv1::UpdateOrderRequest* response) override;
    Status DeleteOrder(ServerContext* context, const osv1::DeleteOrderRequest* request, osv1::DeleteOrderResponse* response) override;
    Status StreamOrderUpdates(ServerContext* context, const osv1::StreamOrderUpdatesRequest* request,
                              ServerWriter<osv1::StreamOrderUpdatesResponse>* writer) override;

   private:
    std::unordered_map<std::string, osv1::Order> orders_;
    std::unordered_map<std::string, std::vector<osv1::Order>> user_orders_;
    std::mutex mutex_;

    static std::string generate_id();
    static int64_t get_current_timestamp();
};
