#pragma once

#include "SmartDevice.h"

class Camera : public SmartDevice {
private:
    bool isRecording;
    std::string resolution;
    bool motionDetected;

public:
    Camera(std::string id, std::string name, double wattage);

    void startRecording();
    void stopRecording();
    void setResolution(std::string res);

    std::string getType() const override;
    void printStatus() const override;
    void applyScene(const std::string& scene) override;
};
