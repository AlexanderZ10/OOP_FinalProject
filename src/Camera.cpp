#include "Camera.h"

#include "Exceptions.h"

#include <iostream>
#include <utility>

using namespace std;

Camera::Camera(string id, string name, double wattage)
    : SmartDevice(move(id), move(name), wattage),
      isRecording(false),
      resolution("1080p"),
      motionDetected(false) {
}

string Camera::getType() const {
    return "Camera";
}

void Camera::startRecording() {
    isRecording = true;
}

void Camera::stopRecording() {
    isRecording = false;
}

void Camera::setResolution(string res) {
    if (res != "720p" && res != "1080p" && res != "4K") {
        throw InvalidSettingException("Camera resolution must be 720p, 1080p, or 4K.");
    }

    resolution = move(res);
}

void Camera::printStatus() const {
    cout << "Camera [" << id << "] " << name
              << " | Active: " << (isActive ? "yes" : "no")
              << " | Wattage: " << wattage
              << " | Recording: " << (isRecording ? "yes" : "no")
              << " | Resolution: " << resolution
              << " | Motion detected: " << (motionDetected ? "yes" : "no") << endl;
}

void Camera::applyScene(const string& scene) {
    if (scene == "going_out" || scene == "night_mode") {
        turnOn();
        startRecording();
    }
}
