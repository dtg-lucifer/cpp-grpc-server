#include "common.hpp"
#include "config/config.hpp"
#include "order_service/order.grpc.pb.h"
#include "server/server.hpp"
#include "service/order_service.hpp"

namespace osv1 = order_service::v1;

int main() {
    Config config;
    std::ostrstream addr;

    try {
        config = Config::New();

        addr << config.host << ":" << config.port;

        std::shared_ptr<OrderService> oService = std::make_shared<OrderService>();
        std::unique_ptr<Server> server =
            std::make_unique<Server>(addr.str(), oService, osv1::OrderService::service_full_name());

        server->Run();

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
