#include "HomeHub.h"

#include <algorithm>
#include <iostream>

using namespace std;

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

void HomeHub::removeDevice(const string& id) {
    auto it = find_if(devices.begin(), devices.end(), [&id](SmartDevice* device) {
        return device != nullptr && device->getId() == id;
    });

    if (it != devices.end()) {
        delete *it;
        devices.erase(it);
    }
}

void HomeHub::renameDevice(const string& id, const string& newName) {
    for (SmartDevice* device : devices) {
        if (device != nullptr && device->getId() == id) {
            device->rename(newName);
            return;
        }
    }
}

SmartDevice* HomeHub::findDeviceById(const string& id) const {
    for (SmartDevice* device : devices) {
        if (device != nullptr && device->getId() == id) {
            return device;
        }
    }

    return nullptr;
}

void HomeHub::turnAllOn() {
    for (SmartDevice* device : devices) {
        if (device != nullptr) {
            device->turnOn();
        }
    }
}

void HomeHub::turnAllOff() {
    for (SmartDevice* device : devices) {
        if (device != nullptr) {
            device->turnOff();
        }
    }
}

void HomeHub::executeScene(const string& scene) {
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

vector<SmartDevice*> HomeHub::getActiveDevices() const {
    vector<SmartDevice*> activeDevices;

    for (SmartDevice* device : devices) {
        if (device != nullptr && device->isOn()) {
            activeDevices.push_back(device);
        }
    }

    return activeDevices;
}

vector<SmartDevice*> HomeHub::getByCategory(const string& type) const {
    vector<SmartDevice*> matchingDevices;

    for (SmartDevice* device : devices) {
        if (device != nullptr && device->getType() == type) {
            matchingDevices.push_back(device);
        }
    }

    return matchingDevices;
}

vector<SmartDevice*> HomeHub::getDevicesSortedByPower() const {
    vector<SmartDevice*> sortedDevices = devices;

    sort(sortedDevices.begin(), sortedDevices.end(), [](const SmartDevice* left, const SmartDevice* right) {
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

void HomeHub::energySavingMode(double threshold) {
    bool turnedOffAnyDevice = false;

    for (SmartDevice* device : devices) {
        if (device != nullptr && device->isOn() && device->getWattage() > threshold) {
            cout << "- Turning off " << device->getName()
                      << " (" << device->getType()
                      << ", " << device->getWattage() << "W)" << endl;
            device->turnOff();
            turnedOffAnyDevice = true;
        }
    }

    if (!turnedOffAnyDevice) {
        cout << "- No active devices above " << threshold << "W were turned off." << endl;
    }
}

void HomeHub::printDevicesAbovePower(double threshold) const {
    bool foundDevice = false;

    for (const SmartDevice* device : devices) {
        if (device != nullptr && device->getWattage() > threshold) {
            cout << "- " << device->getName()
                      << " | Type: " << device->getType()
                      << " | Wattage: " << device->getWattage() << "W"
                      << " | Active: " << (device->isOn() ? "yes" : "no") << endl;
            foundDevice = true;
        }
    }

    if (!foundDevice) {
        cout << "- No devices above " << threshold << "W." << endl;
    }
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
