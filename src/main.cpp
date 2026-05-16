#include "Camera.h"
#include "Exceptions.h"
#include "Light.h"
#include "SmartLock.h"
#include "Thermostat.h"

#include <iostream>

int main() {
    Light livingRoomLight("light-1", "Living Room Light", 9.5);
    Thermostat hallwayThermostat("thermo-1", "Hallway Thermostat", 3.0);
    Camera frontDoorCamera("camera-1", "Front Door Camera", 12.0);
    SmartLock frontDoorLock("lock-1", "Front Door Lock", 5.0, "1234");

    livingRoomLight.printStatus();
    hallwayThermostat.printStatus();
    frontDoorCamera.printStatus();
    frontDoorLock.printStatus();

    livingRoomLight.setBrightness(75);
    hallwayThermostat.setTemp(21.5);
    frontDoorCamera.setResolution("4K");

    if (frontDoorLock.unlock("1234")) {
        std::cout << "Smart lock unlocked successfully." << std::endl;
    }

    try {
        livingRoomLight.setBrightness(150);
    } catch (const InvalidSettingException& ex) {
        std::cout << "Invalid setting caught: " << ex.what() << std::endl;
    }

    return 0;
}
