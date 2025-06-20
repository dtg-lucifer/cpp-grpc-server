#pragma once

#include <cstdlib>
#include <iostream>
#include <string>

inline std::string getEnv(const std::string& key, const std::string& default_value) {
    const char* value = std::getenv(key.c_str());
    return value ? std::string(value) : default_value;
}

struct Config {
    std::string host;
    std::string port;

    static Config New() {
        Config config;
        config.host = getEnv("HOST", "0.0.0.0");
        config.port = getEnv("PORT", "8080");

        return config;
    };

    void display() {
        std::cout << "Configuration:" << std::endl;
        std::cout << "Host: " << this->host << std::endl;
        std::cout << "Port: " << this->port << std::endl;
        std::cout << "------------------------" << std::endl;
    }
};
