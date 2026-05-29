#pragma once

#include "SmartDevice.h"

class Thermostat : public SmartDevice {
private:
    double targetTemp;
    const double MIN_TEMP = 5.0;
    const double MAX_TEMP = 35.0;

public:
    Thermostat(std::string id, std::string name, double wattage);

    void setTemp(double temp);
    double getTemp() const;

    std::string getType() const override;
    void printStatus() const override;
    void applyScene(const std::string& scene) override;
};
