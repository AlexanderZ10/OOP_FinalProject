#include "Thermostat.h"

#include "Exceptions.h"

#include <iostream>
#include <utility>

using namespace std;

Thermostat::Thermostat(string id, string name, double wattage)
    : SmartDevice(move(id), move(name), wattage), targetTemp(22.0) {
}

string Thermostat::getType() const {
    return "Thermostat";
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
    cout << "Thermostat [" << id << "] " << name
              << " | Active: " << (isActive ? "yes" : "no")
              << " | Wattage: " << wattage
              << " | Target temp: " << targetTemp << "C" << endl;
}

void Thermostat::applyScene(const string& scene) {
    if (scene == "going_out") {
        targetTemp = 18.0;
    } else if (scene == "night_mode") {
        targetTemp = 20.0;
    }
}
