#pragma once

#include <exception>
#include <string>
#include <utility>

class InvalidSettingException : public std::exception {
private:
    std::string message;

public:
    explicit InvalidSettingException(std::string message) : message(std::move(message)) {
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

class LockedException : public std::exception {
private:
    std::string message;

public:
    explicit LockedException(std::string message) : message(std::move(message)) {
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};
