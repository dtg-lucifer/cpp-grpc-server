#pragma once

#include <cstdlib>
#include <string>

inline std::string getEnv(const std::string& key, const std::string& default_value) {
    std::string value = std::getenv(key.c_str());
    return !value.empty() ? value : default_value;
}

struct Config {
    std::string host;
    std::string port;

    static Config New() {
        Config config;
        config.host = getEnv("HOST", "localhost");
        config.port = getEnv("PORT", "8080");

        return config;
    };
};
