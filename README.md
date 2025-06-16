# C++ gRPC Order Service Demo

A modern C++ implementation of a gRPC-based order management service, showcasing how to build robust, high-performance microservices using Protocol Buffers and the gRPC framework.

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![gRPC](https://img.shields.io/badge/gRPC-latest-brightgreen.svg)
![Protocol Buffers](https://img.shields.io/badge/Protobuf-3-orange.svg)
![CMake](https://img.shields.io/badge/CMake-3.20+-yellow.svg)

## 📘 Overview

This project demonstrates how to implement a microservice architecture using C++ and gRPC. The sample service provides an order management system with the following features:

- CRUD operations for orders
- Real-time order status updates via server-side streaming
- User-based order listing with filtering and pagination
- Configurable server settings

The demo showcases several C++17 features, gRPC service implementation patterns, and proper project organization for maintainable C++ services.

## 🌟 Features

- **Modern C++ (C++17)** - Uses modern C++ features like structured bindings, std::optional, and more
- **gRPC Service Implementation** - Complete implementation of unary and streaming RPCs
- **Protocol Buffers** - Efficient serialization with proto3 syntax
- **CMake Build System** - Simple yet powerful build configuration
- **Thread-Safe Design** - Mutex-protected shared resources for concurrent access
- **Configuration Management** - Environment-based configuration using a clean approach

## 🧱 Project Structure

```
cpp-grpc-demo/
├── include/                 # Header files
│   ├── config/              # Configuration-related headers
│   ├── server/              # Server implementation headers
│   └── service/             # Service implementation headers
├── proto/                   # Protocol Buffer definitions
│   └── order_service/       # Order service proto files
├── src/                     # Source files
│   ├── main.cpp             # Entry point
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

3. Build the project:
   ```sh
   make
   ```

4. Run the service:
   ```sh
   ./build/bin/grpc-demo
   ```

### Environment Variables

The service can be configured via environment variables:

- `HOST`: The hostname to bind to (default: `localhost`)
- `PORT`: The port to listen on (default: `8080`)

Example:
```sh
HOST=0.0.0.0 PORT=9000 ./build/bin/grpc-demo
```

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

Creates a new order.

```protobuf
rpc CreateOrder(CreateOrderRequest) returns (CreateOrderResponse);
```

### UpdateOrder

Updates an existing order.

```protobuf
rpc UpdateOrder(UpdateOrderRequest) returns (UpdateOrderResponse);
```

### StreamOrderUpdates

Streams real-time updates for an order.

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
