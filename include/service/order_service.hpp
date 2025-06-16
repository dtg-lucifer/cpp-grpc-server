#pragma once

#include <grpcpp/grpcpp.h>
#include <grpcpp/server_context.h>
#include <grpcpp/support/status.h>

#include "order_service/order.grpc.pb.h"
#include "order_service/order.pb.h"

namespace osv1 = order_service::v1;

using grpc::ServerContext;
using grpc::ServerWriter;
using grpc::Status;

class OrderService final : public osv1::OrderService::Service {
   public:
    OrderService();

    grpc::Status GetOrder(ServerContext* context, const osv1::GetOrderRequest* request, osv1::GetOrderResponse* response) override;
    grpc::Status ListOrders(ServerContext* context, const osv1::ListOrdersRequest* request, osv1::ListOrdersResponse* response) override;
    grpc::Status CreateOrder(ServerContext* context, const osv1::CreateOrderRequest* request, osv1::CreateOrderResponse* response) override;
    grpc::Status UpdateOrder(ServerContext* context, const osv1::UpdateOrderResponse* request, osv1::UpdateOrderRequest* response) override;
    grpc::Status DeleteOrder(ServerContext* context, const osv1::DeleteOrderRequest* request, osv1::DeleteOrderResponse* response) override;
    grpc::Status StreamOrderUpdates(ServerContext* context, const osv1::StreamOrderUpdatesRequest* request,
                                    ServerWriter<osv1::StreamOrderUpdatesResponse>* writer) override;
};
