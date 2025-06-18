#pragma once

#include <grpcpp/support/interceptor.h>
#include <grpcpp/support/server_interceptor.h>

class LoggerInterceptor final : public grpc::experimental::Interceptor {
   public:
    explicit LoggerInterceptor(grpc::experimental::ServerRpcInfo* info) {
        // Constructor to initialize the intercepto
        std::string method = info->method();

        if (method == "unknown") {
            std::cout << "Unknown method called" << std::endl;
            return;
        }

        std::cout << "Method called: " << method << std::endl;
    }

    void Intercept(grpc::experimental::InterceptorBatchMethods* methods) override {
        methods->Proceed();  // Important !!
    }
};

class LoggerInterceptorFactory final : public grpc::experimental::ServerInterceptorFactoryInterface {
   public:
    grpc::experimental::Interceptor* CreateServerInterceptor(grpc::experimental::ServerRpcInfo* info) override {
        return new LoggerInterceptor(info);
    }
};
