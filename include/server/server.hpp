#pragma once

#include <grpcpp/grpcpp.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/support/interceptor.h>
#include <grpcpp/support/server_interceptor.h>

#include <memory>
#include <utility>
#include <vector>

#include "interceptors/logger.hpp"

class Server {
   private:
    std::shared_ptr<grpc::Server> server_;
    std::shared_ptr<grpc::Service> service_;
    std::string addr_;
    std::string service_name_;
    std::vector<std::unique_ptr<grpc::experimental::ServerInterceptorFactoryInterface>> interceptors_;

   public:
    explicit Server(const std::string& addr, std::shared_ptr<grpc::Service> service,
                    const std::string& service_name = "gRPC::Server");

    void Run();
    void Stop();
};
