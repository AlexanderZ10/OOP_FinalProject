#pragma once

#include <string>

class SmartDevice {
protected:
    std::string id;
    std::string name;
    bool isActive;
    double wattage;

public:
    SmartDevice(std::string id, std::string name, double wattage);
    virtual ~SmartDevice();

    void turnOn();
    void turnOff();

    std::string getId() const;
    std::string getName() const;
    void rename(std::string newName);
    double getWattage() const;
    bool isOn() const;

    virtual void printStatus() const = 0;
    virtual void applyScene(const std::string& scene) = 0;
};
