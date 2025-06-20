#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/credentials.h>

#include <exception>
#include <iostream>
#include <memory>
#include <string>

#include "order_service/order.grpc.pb.h"
#include "order_service/order.pb.h"

namespace osv1 = order_service::v1;

class OrderClient {
   public:
    OrderClient(const std::string addr_) : addr_(addr_) {
        std::cout << "OrderClient initialized with address: " << addr_ << std::endl;
        this->channel_ = grpc::CreateChannel(this->addr_, grpc::InsecureChannelCredentials());
        this->stub_ = osv1::OrderService::NewStub(this->channel_);
    }

    ~OrderClient() { std::cout << "OrderClient destroyed." << std::endl; }

    osv1::ListOrdersResponse ListOrders(const osv1::ListOrdersRequest& request) {
        grpc::ClientContext context;
        osv1::ListOrdersResponse response;

        grpc::Status status = this->stub_->ListOrders(&context, request, &response);
        if (!status.ok()) {
            std::cerr << "ListOrders failed: " << status.error_message() << std::endl;
            return osv1::ListOrdersResponse();
        }

        return response;
    }

   private:
    std::shared_ptr<grpc::Channel> channel_;
    std::unique_ptr<osv1::OrderService::Stub> stub_;
    std::string addr_;
};

int main() {
    OrderClient client("0.0.0.0:8080");
    osv1::ListOrdersRequest request;
    request.set_user_id("user1");
    request.set_limit(10);
    request.set_page(1);

    try {
        auto response = client.ListOrders(request);
        std::cout << "Response: " << std::endl;
        std::cout << "List: " << response.total() << std::endl;

        for (const auto& order : response.orders()) {
            std::cout << "Order ID: " << order.id() << ", Status: " << order.status() << std::endl;

            for (auto& item : order.items()) {
                std::cout << "  Item ID: " << item.id() << ", Name: " << item.name() << ", Price: " << item.price()
                          << ", Quantity: " << item.quantity() << std::endl;
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
