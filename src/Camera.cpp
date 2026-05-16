#include "Camera.h"

#include "Exceptions.h"

#include <iostream>
#include <utility>

Camera::Camera(std::string id, std::string name, double wattage)
    : SmartDevice(std::move(id), std::move(name), wattage),
      isRecording(false),
      resolution("1080p"),
      motionDetected(false) {
}

void Camera::startRecording() {
    isRecording = true;
}

void Camera::stopRecording() {
    isRecording = false;
}

void Camera::setResolution(std::string res) {
    if (res != "720p" && res != "1080p" && res != "4K") {
        throw InvalidSettingException("Camera resolution must be 720p, 1080p, or 4K.");
    }

    resolution = std::move(res);
}

void Camera::printStatus() const {
    std::cout << "Camera [" << id << "] " << name
              << " | Active: " << (isActive ? "yes" : "no")
              << " | Wattage: " << wattage
              << " | Recording: " << (isRecording ? "yes" : "no")
              << " | Resolution: " << resolution
              << " | Motion detected: " << (motionDetected ? "yes" : "no") << std::endl;
}

void Camera::applyScene(const std::string& scene) {
    if (scene == "going_out" || scene == "night_mode") {
        turnOn();
        startRecording();
    }
}
