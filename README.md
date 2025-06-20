# C++ gRPC Order Service Demo

A modern C++ implementation of a gRPC-based order management system, showcasing how to build robust, high-performance microservices using Protocol Buffers and the gRPC framework. This project includes both server and client implementations to demonstrate complete RPC communication flows.

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![gRPC](https://img.shields.io/badge/gRPC-latest-brightgreen.svg)
![Protocol Buffers](https://img.shields.io/badge/Protobuf-3-orange.svg)
![CMake](https://img.shields.io/badge/CMake-3.20+-yellow.svg)
![Abseil](https://img.shields.io/badge/Abseil-latest-purple.svg)

## 📘 Overview

This project demonstrates how to implement a microservice architecture using C++ and gRPC. The sample system provides an order management platform with the following features:

- Complete server implementation with CRUD operations for orders
- Client application with readable output formatting
- Real-time order status updates via server-side streaming
- User-based order listing with filtering and pagination
- Configurable server settings
- Server-side interceptors for logging and request handling
- JSON formatting utilities for Protocol Buffer messages

The demo showcases several C++17 features, gRPC service implementation patterns, and proper project organization for maintainable C++ services.

## 🌟 Features

- **Modern C++ (C++17)** - Uses modern C++ features like structured bindings, smart pointers, and more
- **Complete Client-Server Architecture** - Both client and server implementations included
- **gRPC Service Implementation** - Complete implementation of unary and streaming RPCs
- **Protocol Buffers** - Efficient serialization with proto3 syntax
- **CMake Build System** - Simple yet powerful build configuration
- **Thread-Safe Design** - Mutex-protected shared resources for concurrent access
- **Configuration Management** - Environment-based configuration using a clean approach
- **Server Interceptors** - gRPC interceptors for request logging and monitoring
- **Real-time Order Updates** - Server-side streaming to monitor order status changes
- **Protobuf Formatting Utilities** - Helper functions for JSON conversion and pretty-printing
- **Abseil Integration** - Uses Google's Abseil library for enhanced functionality

## 🧱 Project Structure

```
cpp-grpc-demo/
├── include/                 # Header files
│   ├── config/              # Configuration-related headers
│   ├── interceptors/        # gRPC interceptor implementations
│   ├── server/              # Server implementation headers
│   ├── service/             # Service implementation headers
│   └── common.hpp           # Common includes and utilities
├── proto/                   # Protocol Buffer definitions
│   └── order_service/       # Order service proto files
├── src/                     # Source files
│   ├── client.cpp           # gRPC client implementation
│   ├── main.cpp             # Server entry point
│   ├── server/              # Server implementations
│   └── service/             # Service implementations
├── scripts/                 # Utility scripts
│   ├── bootstrap.sh         # Project bootstrap script
│   └── proto-gen.sh         # Proto file generation script
├── genproto/                # Generated protobuf code (created during build)
├── build/                   # Build directory
├── CMakeLists.txt           # CMake configuration
└── Makefile                 # Makefile for common tasks
```

## 🚀 Getting Started

### Prerequisites

- C++17 compatible compiler (GCC 7+/Clang 5+/MSVC 2019+)
- CMake 3.20 or higher
- gRPC and Protocol Buffers development libraries
- utf8_range library (usually bundled with Protocol Buffers)
- Abseil libraries (for enhanced functionality)

### Building the Project

1. Clone the repository:
   ```sh
   git clone https://github.com/yourusername/cpp-grpc-demo.git
   cd cpp-grpc-demo
   ```

2. Bootstrap the project:
   ```sh
   make bootstrap
   ```

3. Generate Protocol Buffer code:
   ```sh
   make proto
   ```

4. Build the project:
   ```sh
   make
   ```

5. Run the service:
   ```sh
   make run
   ```

### Environment Variables

The server can be configured via environment variables:

- `HOST`: The hostname to bind to (default: `localhost`)
- `PORT`: The port to listen on (default: `8080`)

Example:
```sh
HOST=0.0.0.0 PORT=9000 ./build/bin/grpc-server
```

By default, the client connects to `0.0.0.0:8080`. If you change the server configuration, make sure to update the client connection address in the client code.

## 📋 Order Service API

The Order Service provides the following RPCs:

### GetOrder

Retrieves an order by ID.

```protobuf
rpc GetOrder(GetOrderRequest) returns (GetOrderResponse);
```

### ListOrders

Lists orders for a specific user with pagination and filtering.

```protobuf
rpc ListOrders(ListOrdersRequest) returns (ListOrdersResponse);
```

### CreateOrder

Creates a new order. The implementation automatically:
- Generates a unique order ID
- Sets the initial status to PENDING
- Calculates the total amount based on items
- Records creation timestamp

```protobuf
rpc CreateOrder(CreateOrderRequest) returns (CreateOrderResponse);
```

### UpdateOrder

Updates an existing order.

```protobuf
rpc UpdateOrder(UpdateOrderRequest) returns (UpdateOrderResponse);
```

### StreamOrderUpdates

Streams real-time updates for an order. The implementation simulates order progression:
- PENDING → PROCESSING → SHIPPED → DELIVERED
- Updates are sent every 5 seconds
- Demonstrates server-side streaming capability

```protobuf
rpc StreamOrderUpdates(StreamOrderUpdatesRequest) returns (stream StreamOrderUpdatesResponse);
```

### DeleteOrder

Deletes an order by ID.

```protobuf
rpc DeleteOrder(DeleteOrderRequest) returns (DeleteOrderResponse);
```

## 🧪 Generating Proto Files

If you modify the `.proto` files, you need to regenerate the C++ code:

```sh
./scripts/proto-gen.sh
```

This script will clean the `genproto` directory and generate new files based on your updated proto definitions.

## 🔍 Server Interceptors

The project includes gRPC interceptors to enhance server functionality:

### LoggerInterceptor

A simple logging interceptor that prints method information when an RPC is called:

```cpp
class LoggerInterceptor final : public grpc::experimental::Interceptor {
   public:
    explicit LoggerInterceptor(grpc::experimental::ServerRpcInfo* info) {
        std::string method = info->method();
        std::cout << "Method called: " << method << std::endl;
    }

    void Intercept(grpc::experimental::InterceptorBatchMethods* methods) override {
        methods->Proceed();
    }
};
```

Interceptors are registered when the server is created, providing a clean way to add cross-cutting concerns like logging, authentication, or metrics collection.

## 🖥️ Client Implementation

The project includes a client implementation that demonstrates how to:

1. Connect to the gRPC server
2. Make RPC calls to the Order Service
3. Process and display the response data

### ProtoFormatter Helper Class

The client includes a utility class with several helper methods for formatting Protocol Buffer messages:

```cpp
class ProtoFormatter {
public:
    // Format currency values consistently
    static std::string FormatPrice(float price);

    // Pretty print an order with indentation
    static void PrintOrder(const osv1::Order& order, int indent = 0);

    // Convert any protobuf message to JSON string
    template <typename ProtoMessage>
    static std::string ToJson(const ProtoMessage& message, bool pretty_print = true);

    // Display any enum descriptor with all its values
    static void PrintEnumValues(const google::protobuf::EnumDescriptor* descriptor);
};
```

This class demonstrates how to leverage Protocol Buffers' reflection capabilities to create human-readable output and work with dynamic message content.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the project
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📜 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 📚 Resources

- [gRPC Documentation](https://grpc.io/docs/)
- [Protocol Buffers Developer Guide](https://developers.google.com/protocol-buffers/docs/overview)
- [CMake Documentation](https://cmake.org/documentation/)
- [gRPC C++ Interceptors](https://grpc.io/docs/languages/cpp/interceptors/)
- [C++17 Features](https://en.cppreference.com/w/cpp/17)
- [Protocol Buffers C++ API](https://developers.google.com/protocol-buffers/docs/reference/cpp)
- [Protocol Buffers JSON Mapping](https://developers.google.com/protocol-buffers/docs/proto3#json)
- [Abseil C++ Libraries](https://abseil.io/)
