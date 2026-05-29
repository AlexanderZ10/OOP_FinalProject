#include "HomeHub.h"

#include <algorithm>

HomeHub::~HomeHub() {
    for (SmartDevice* device : devices) {
        delete device;
    }
}

void HomeHub::addDevice(SmartDevice* device) {
    if (device != nullptr) {
        devices.push_back(device);
    }
}

void HomeHub::removeDevice(const std::string& id) {
    auto it = std::find_if(devices.begin(), devices.end(), [&id](SmartDevice* device) {
        return device != nullptr && device->getId() == id;
    });

    if (it != devices.end()) {
        delete *it;
        devices.erase(it);
    }
}

void HomeHub::renameDevice(const std::string& id, const std::string& newName) {
    for (SmartDevice* device : devices) {
        if (device != nullptr && device->getId() == id) {
            device->rename(newName);
            return;
        }
    }
}

void HomeHub::executeScene(const std::string& scene) {
    for (SmartDevice* device : devices) {
        if (device != nullptr) {
            device->applyScene(scene);
        }
    }
}

void HomeHub::printAllStatuses() const {
    for (const SmartDevice* device : devices) {
        if (device != nullptr) {
            device->printStatus();
        }
    }
}

std::vector<SmartDevice*> HomeHub::getActiveDevices() const {
    std::vector<SmartDevice*> activeDevices;

    for (SmartDevice* device : devices) {
        if (device != nullptr && device->isOn()) {
            activeDevices.push_back(device);
        }
    }

    return activeDevices;
}

std::vector<SmartDevice*> HomeHub::getByCategory(const std::string& type) const {
    std::vector<SmartDevice*> matchingDevices;

    for (SmartDevice* device : devices) {
        if (device != nullptr && device->getType() == type) {
            matchingDevices.push_back(device);
        }
    }

    return matchingDevices;
}

std::vector<SmartDevice*> HomeHub::getDevicesSortedByPower() const {
    std::vector<SmartDevice*> sortedDevices = devices;

    std::sort(sortedDevices.begin(), sortedDevices.end(), [](const SmartDevice* left, const SmartDevice* right) {
        if (left == nullptr) {
            return false;
        }

        if (right == nullptr) {
            return true;
        }

        return left->getWattage() < right->getWattage();
    });

    return sortedDevices;
}

double HomeHub::calculateTotalPower() const {
    double totalPower = 0.0;

    for (const SmartDevice* device : devices) {
        if (device != nullptr && device->isOn()) {
            totalPower += device->getWattage();
        }
    }

    return totalPower;
}
