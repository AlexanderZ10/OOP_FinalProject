#include "SmartLock.h"

#include "Exceptions.h"

#include <iostream>
#include <utility>

SmartLock::SmartLock(std::string id, std::string name, double wattage, std::string pin)
    : SmartDevice(std::move(id), std::move(name), wattage),
      pinCode(std::move(pin)),
      failedAttempts(0),
      isLocked(true) {
}

bool SmartLock::unlock(std::string pin) {
    if (pin == pinCode) {
        isLocked = false;
        failedAttempts = 0;
        return true;
    }

    ++failedAttempts;
    if (failedAttempts >= 3) {
        throw LockedException("Smart lock blocked after 3 failed attempts.");
    }

    return false;
}

void SmartLock::lock() {
    isLocked = true;
}

void SmartLock::changePin(std::string oldPin, std::string newPin) {
    if (oldPin != pinCode) {
        ++failedAttempts;
        if (failedAttempts >= 3) {
            throw LockedException("Smart lock blocked after 3 failed attempts.");
        }

        throw InvalidSettingException("Old PIN is incorrect.");
    }

    pinCode = std::move(newPin);
    failedAttempts = 0;
}

void SmartLock::printStatus() const {
    std::cout << "SmartLock [" << id << "] " << name
              << " | Active: " << (isActive ? "yes" : "no")
              << " | Wattage: " << wattage
              << " | Locked: " << (isLocked ? "yes" : "no")
              << " | Failed attempts: " << failedAttempts << std::endl;
}

void SmartLock::applyScene(const std::string& scene) {
    if (scene == "going_out" || scene == "night_mode") {
        lock();
    }
}
