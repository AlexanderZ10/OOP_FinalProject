#include "SmartDevice.h"

#include <utility>

SmartDevice::SmartDevice(std::string id, std::string name, double wattage)
    : id(std::move(id)), name(std::move(name)), isActive(false), wattage(wattage) {
}

SmartDevice::~SmartDevice() = default;

void SmartDevice::turnOn() {
    isActive = true;
}

void SmartDevice::turnOff() {
    isActive = false;
}

std::string SmartDevice::getId() const {
    return id;
}

std::string SmartDevice::getName() const {
    return name;
}

void SmartDevice::rename(std::string newName) {
    name = std::move(newName);
}

double SmartDevice::getWattage() const {
    return wattage;
}

bool SmartDevice::isOn() const {
    return isActive;
}
