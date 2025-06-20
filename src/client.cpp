#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/credentials.h>

#include <exception>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "google/protobuf/util/json_util.h"
#include "order_service/order.grpc.pb.h"
#include "order_service/order.pb.h"

namespace osv1 = order_service::v1;

// Helper class for formatting protobuf messages nicely
class ProtoFormatter {
   public:
    // Format currency values consistently
    static std::string FormatPrice(float price) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << "$" << price;
        return ss.str();
    }

    // Pretty print an order with indentation
    static void PrintOrder(const osv1::Order& order, int indent = 0) {
        std::string indentStr(indent, ' ');
        std::cout << indentStr << "Order ID: " << order.id() << ", Status: " << osv1::OrderStatus_Name(order.status())
                  << std::endl;

        for (const auto& item : order.items()) {
            std::cout << indentStr << "  Item ID: " << item.id() << ", Name: " << item.name()
                      << ", Price: " << FormatPrice(item.price()) << ", Quantity: " << item.quantity() << std::endl;
        }
    }

    // Convert any protobuf message to JSON string for easy display
    template <typename ProtoMessage>
    static std::string ToJson(const ProtoMessage& message, bool pretty_print = true) {
        std::string json_string;
        google::protobuf::util::JsonPrintOptions options;
        options.add_whitespace = pretty_print;
        options.always_print_primitive_fields = true;
        options.preserve_proto_field_names = true;
        options.always_print_enums_as_ints = false;

        auto status = google::protobuf::util::MessageToJsonString(message, &json_string, options);
        if (!status.ok()) {
            return "Error converting to JSON: " + status.ToString();
        }
        return json_string;
    }

    // Display any enum descriptor with all its values
    static void PrintEnumValues(const google::protobuf::EnumDescriptor* descriptor) {
        std::cout << "Enum: " << descriptor->name() << " values:" << std::endl;
        for (int i = 0; i < descriptor->value_count(); i++) {
            const auto* value = descriptor->value(i);
            std::cout << "  " << value->number() << ": " << value->name() << std::endl;
        }
    }
};

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
    // Create the client and request
    OrderClient client("0.0.0.0:8080");

    // Build the request message
    osv1::ListOrdersRequest request;
    request.set_user_id("user1");
    request.set_limit(10);
    request.set_page(1);

    // Show the request as JSON
    std::cout << "Sending request: " << ProtoFormatter::ToJson(request) << std::endl;

    try {
        auto response = client.ListOrders(request);
        std::cout << "Response received successfully!" << std::endl;

        std::cout << "Found " << response.orders_size() << " orders (Total in system: " << response.total() << ")"
                  << std::endl;

        // Display all available enum values for reference using the built-in reflection
        std::cout << "\nAvailable order statuses:" << std::endl;
        const auto* order_status_desc = osv1::OrderStatus_descriptor();
        ProtoFormatter::PrintEnumValues(order_status_desc);

        std::cout << "\nOrder details (custom formatting):" << std::endl;
        for (const auto& order : response.orders()) {
            ProtoFormatter::PrintOrder(order);
        }

        // Showcase JSON formatting
        if (!response.orders().empty()) {
            std::cout << "\nSample order as JSON:" << std::endl;
            std::cout << ProtoFormatter::ToJson(response.orders(0)) << std::endl;

            std::cout << "\nFull response as JSON:" << std::endl;
            std::cout << ProtoFormatter::ToJson(response) << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
