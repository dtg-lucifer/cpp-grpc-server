#include "server/server.hpp"

Server::Server(const std::string& addr, std::shared_ptr<grpc::Service> service, const std::string& service_name)
    : service_(service), addr_(addr), service_name_(service_name) {
    // Register the interceptors
    this->interceptors_.emplace_back(std::make_unique<LoggerInterceptorFactory>());
}

void Server::Run() {
    grpc::ServerBuilder builder;

    // Bind the listening address
    builder.AddListeningPort(this->addr_, grpc::InsecureServerCredentials());

    // Register the interceptor
    builder.experimental().SetInterceptorCreators(std::move(this->interceptors_));

    // Register the services
    builder.RegisterService(this->service_.get());

    // Build the server and start it
    this->server_ = builder.BuildAndStart();

    if (!this->server_) {
        std::cerr << "Failed to start server at " << this->addr_ << std::endl;
        throw std::runtime_error("Server failed to start");
    }

    std::cout << "Server " << this->service_name_ << ", started at " << this->addr_ << std::endl;

    // Wait until it shutsdown
    this->server_->Wait();
}

void Server::Stop() {
    // Stop method to stop the server
    std::cout << "Stopping server " << this->service_name_ << ", please wait" << std::endl;

    // Stop the server if it is running
    if (this->server_) {
        this->server_->Shutdown();
        this->server_.reset();
        std::cout << "Server " << this->service_name_ << " stopped." << std::endl;
    } else {
        std::cout << "Server " << this->service_name_ << " is not running." << std::endl;
    }
}
