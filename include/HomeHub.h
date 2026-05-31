#pragma once

#include "SmartDevice.h"

#include <string>
#include <vector>

class HomeHub {
private:
    std::vector<SmartDevice*> devices;

public:
    HomeHub() = default;
    ~HomeHub();

    HomeHub(const HomeHub&) = delete;
    HomeHub& operator=(const HomeHub&) = delete;

    void addDevice(SmartDevice* device);
    void removeDevice(const std::string& id);
    void renameDevice(const std::string& id, const std::string& newName);
    SmartDevice* findDeviceById(const std::string& id) const;
    void turnAllOn();
    void turnAllOff();
    void executeScene(const std::string& scene);
    void printAllStatuses() const;
    std::vector<SmartDevice*> getActiveDevices() const;
    std::vector<SmartDevice*> getByCategory(const std::string& type) const;
    std::vector<SmartDevice*> getDevicesSortedByPower() const;
    void energySavingMode(double threshold);
    void printDevicesAbovePower(double threshold) const;
    double calculateTotalPower() const;
};
