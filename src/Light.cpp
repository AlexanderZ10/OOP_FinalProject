#include "Light.h"

#include "Exceptions.h"

#include <iostream>
#include <utility>

using namespace std;

Light::Light(string id, string name, double wattage)
    : SmartDevice(move(id), move(name), wattage), brightness(100), colorTemp(4000) {
}

string Light::getType() const {
    return "Light";
}

void Light::setBrightness(int value) {
    if (value < 0 || value > 100) {
        throw InvalidSettingException("Brightness must be between 0 and 100.");
    }

    brightness = value;
}

void Light::setColorTemp(int value) {
    colorTemp = value;
}

void Light::printStatus() const {
    cout << "Light [" << id << "] " << name
              << " | Active: " << (isActive ? "yes" : "no")
              << " | Wattage: " << wattage
              << " | Brightness: " << brightness
              << " | Color temp: " << colorTemp << "K" << endl;
}

void Light::applyScene(const string& scene) {
    if (scene == "going_out") {
        turnOff();
    } else if (scene == "night_mode") {
        turnOn();
        brightness = 20;
    }
}
