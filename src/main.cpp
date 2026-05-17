#include "Camera.h"
#include "Exceptions.h"
#include "HomeHub.h"
#include "Light.h"
#include "SmartLock.h"
#include "Thermostat.h"

#include <iostream>
#include <vector>

int main() {
    HomeHub hub;

    hub.addDevice(new Light("light-1", "Living Room Light", 9.5));
    hub.addDevice(new Thermostat("thermo-1", "Hallway Thermostat", 3.0));
    hub.addDevice(new Camera("camera-1", "Front Door Camera", 12.0));
    hub.addDevice(new SmartLock("lock-1", "Front Door Lock", 5.0, "1234"));

    std::cout << "Initial statuses:" << std::endl;
    hub.printAllStatuses();

    std::cout << std::endl << "Executing night_mode scene..." << std::endl;
    hub.executeScene("night_mode");

    std::cout << std::endl << "Statuses after night_mode:" << std::endl;
    hub.printAllStatuses();

    std::cout << std::endl
              << "Total power of active devices: "
              << hub.calculateTotalPower() << "W" << std::endl;

    std::cout << std::endl << "Renaming light-1..." << std::endl;
    hub.renameDevice("light-1", "Main Living Room Light");
    hub.printAllStatuses();

    std::cout << std::endl << "Active devices:" << std::endl;
    std::vector<SmartDevice*> activeDevices = hub.getActiveDevices();
    for (const SmartDevice* device : activeDevices) {
        std::cout << "- " << device->getName() << " (" << device->getId() << ")" << std::endl;
    }

    std::cout << std::endl << "Removing camera-1..." << std::endl;
    hub.removeDevice("camera-1");

    std::cout << std::endl << "Statuses after removing camera-1:" << std::endl;
    hub.printAllStatuses();

    return 0;
}
