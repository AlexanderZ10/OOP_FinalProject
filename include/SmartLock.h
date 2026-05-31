#pragma once

#include "SmartDevice.h"

class SmartLock : public SmartDevice {
private:
    std::string pinCode;
    int failedAttempts;
    bool isLocked;

public:
    SmartLock(std::string id, std::string name, double wattage, std::string pin);

    void turnOn() override;
    void turnOff() override;
    bool unlock(std::string pin);
    void lock();
    void changePin(std::string oldPin, std::string newPin);

    std::string getType() const override;
    void printStatus() const override;
    void applyScene(const std::string& scene) override;
};
