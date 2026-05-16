#pragma once

#include "SmartDevice.h"

class Light : public SmartDevice {
private:
    int brightness;
    int colorTemp;

public:
    Light(std::string id, std::string name, double wattage);

    void setBrightness(int value);
    void setColorTemp(int value);

    void printStatus() const override;
    void applyScene(const std::string& scene) override;
};
