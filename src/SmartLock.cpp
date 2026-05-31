#include "SmartLock.h"

#include "Exceptions.h"

#include <iostream>
#include <utility>

using namespace std;

SmartLock::SmartLock(string id, string name, double wattage, string pin)
    : SmartDevice(move(id), move(name), wattage),
      pinCode(move(pin)),
      failedAttempts(0),
      isLocked(true) {
    isActive = true;
}

string SmartLock::getType() const {
    return "SmartLock";
}

void SmartLock::turnOn() {
    lock();
}

void SmartLock::turnOff() {
    isLocked = false;
    isActive = false;
    failedAttempts = 0;
}

bool SmartLock::unlock(string pin) {
    if (pin == pinCode) {
        turnOff();
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
    isActive = true;
}

void SmartLock::changePin(string oldPin, string newPin) {
    if (oldPin != pinCode) {
        ++failedAttempts;
        if (failedAttempts >= 3) {
            throw LockedException("Smart lock blocked after 3 failed attempts.");
        }

        throw InvalidSettingException("Old PIN is incorrect.");
    }

    pinCode = move(newPin);
    failedAttempts = 0;
}

void SmartLock::printStatus() const {
    cout << "SmartLock [" << id << "] " << name
              << " | Wattage: " << wattage
              << " | Locked: " << (isLocked ? "yes" : "no")
              << " | Failed attempts: " << failedAttempts << endl;
}

void SmartLock::applyScene(const string& scene) {
    if (scene == "going_out" || scene == "night_mode") {
        lock();
    }
}
