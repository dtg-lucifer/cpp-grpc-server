#include <sstream>

#include "common.hpp"
#include "config/config.hpp"
#include "order_service/order.grpc.pb.h"
#include "server/server.hpp"
#include "service/order_service.hpp"

namespace osv1 = order_service::v1;

std::unique_ptr<Server> server = nullptr;
std::mutex lockdown_mutex;
std::condition_variable lockdown_cv;
bool shutdown_requested = false;

void handler(int signum) {
    std::lock_guard<std::mutex> lock(lockdown_mutex);
    shutdown_requested = true;
    lockdown_cv.notify_one();
    std::cout << "Received signal " << signum << ", shutting down..." << std::endl;
}

int main() {
    Config config;

    std::signal(SIGINT, handler);
    std::signal(SIGTERM, handler);

    try {
        config = Config::New();

        std::ostringstream addr;
        addr << config.host << ":" << config.port;

        std::shared_ptr<OrderService> oService = std::make_shared<OrderService>();
        server = std::make_unique<Server>(addr.str(), oService, osv1::OrderService::service_full_name());

        std::thread shutdown_thread([&]() {
            std::unique_lock<std::mutex> lock(lockdown_mutex);
            lockdown_cv.wait(lock, [] { return shutdown_requested; });
            server->Stop();
        });

        server->Run();

        shutdown_thread.join();
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
