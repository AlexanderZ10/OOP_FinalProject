#include "SmartDevice.h"

#include <utility>

using namespace std;

SmartDevice::SmartDevice(string id, string name, double wattage)
    : id(move(id)), name(move(name)), isActive(false), wattage(wattage) {
}

SmartDevice::~SmartDevice() = default;

void SmartDevice::turnOn() {
    isActive = true;
}

void SmartDevice::turnOff() {
    isActive = false;
}

string SmartDevice::getId() const {
    return id;
}

string SmartDevice::getName() const {
    return name;
}

void SmartDevice::rename(string newName) {
    name = move(newName);
}

double SmartDevice::getWattage() const {
    return wattage;
}

bool SmartDevice::isOn() const {
    return isActive;
}
