#include "Thermostat.h"

#include "Exceptions.h"

#include <iostream>
#include <utility>

Thermostat::Thermostat(std::string id, std::string name, double wattage)
    : SmartDevice(std::move(id), std::move(name), wattage), targetTemp(22.0) {
}

void Thermostat::setTemp(double temp) {
    if (temp < MIN_TEMP || temp > MAX_TEMP) {
        throw InvalidSettingException("Temperature must be between 5 and 35 degrees.");
    }

    targetTemp = temp;
}

double Thermostat::getTemp() const {
    return targetTemp;
}

void Thermostat::printStatus() const {
    std::cout << "Thermostat [" << id << "] " << name
              << " | Active: " << (isActive ? "yes" : "no")
              << " | Wattage: " << wattage
              << " | Target temp: " << targetTemp << "C" << std::endl;
}

void Thermostat::applyScene(const std::string& scene) {
    if (scene == "going_out") {
        targetTemp = 18.0;
    } else if (scene == "night_mode") {
        targetTemp = 20.0;
    }
}
