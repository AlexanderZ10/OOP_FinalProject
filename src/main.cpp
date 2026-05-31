#include "Camera.h"
#include "Exceptions.h"
#include "HomeHub.h"
#include "Light.h"
#include "ScheduledTask.h"
#include "SmartLock.h"
#include "Thermostat.h"

#include <ctime>
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

    std::cout << std::endl << "Light devices:" << std::endl;
    std::vector<SmartDevice*> lightDevices = hub.getByCategory("Light");
    for (const SmartDevice* device : lightDevices) {
        std::cout << "- " << device->getName() << " (" << device->getId() << ")" << std::endl;
    }

    std::cout << std::endl << "Camera devices:" << std::endl;
    std::vector<SmartDevice*> cameraDevices = hub.getByCategory("Camera");
    for (const SmartDevice* device : cameraDevices) {
        std::cout << "- " << device->getName() << " (" << device->getId() << ")" << std::endl;
    }

    std::cout << std::endl << "Devices sorted by power:" << std::endl;
    std::vector<SmartDevice*> sortedDevices = hub.getDevicesSortedByPower();
    for (const SmartDevice* device : sortedDevices) {
        std::cout << "- " << device->getName()
                  << " | Type: " << device->getType()
                  << " | Wattage: " << device->getWattage() << "W" << std::endl;
    }

    const double energySavingThreshold = 10.0;

    std::cout << std::endl << "Active devices before energy saving mode:" << std::endl;
    activeDevices = hub.getActiveDevices();
    for (const SmartDevice* device : activeDevices) {
        std::cout << "- " << device->getName()
                  << " | Type: " << device->getType()
                  << " | Wattage: " << device->getWattage() << "W" << std::endl;
    }

    std::cout << std::endl
              << "Total power before energy saving mode: "
              << hub.calculateTotalPower() << "W" << std::endl;

    std::cout << std::endl
              << "Devices above " << energySavingThreshold << "W:" << std::endl;
    hub.printDevicesAbovePower(energySavingThreshold);

    std::cout << std::endl
              << "Applying energy saving mode with threshold "
              << energySavingThreshold << "W:" << std::endl;
    hub.energySavingMode(energySavingThreshold);

    std::cout << std::endl
              << "Total power after energy saving mode: "
              << hub.calculateTotalPower() << "W" << std::endl;

    std::cout << std::endl << "Statuses after energy saving mode:" << std::endl;
    hub.printAllStatuses();

    SmartDevice* scheduledLight = lightDevices.empty() ? nullptr : lightDevices.front();
    SmartDevice* scheduledThermostat = hub.getByCategory("Thermostat").front();

    ScheduledTask turnLightOffTask(scheduledLight, "off", 5);
    ScheduledTask turnThermostatOnTask(scheduledThermostat, "on", 10);

    std::time_t currentTime = std::time(nullptr);

    auto checkTask = [currentTime](ScheduledTask& task,
                                   SmartDevice* device,
                                   const std::string& command,
                                   std::time_t simulatedTime) {
        bool executedNow = task.checkAndExecute(simulatedTime);

        std::cout << "- Time +" << (simulatedTime - currentTime) << "s | "
                  << device->getName() << " -> " << command
                  << " | Activated now: " << (executedNow ? "yes" : "no")
                  << " | Executed: " << (task.isExecuted() ? "yes" : "no") << std::endl;
    };

    std::cout << std::endl << "Scheduled tasks demo:" << std::endl;
    std::cout << "Created task: " << scheduledLight->getName() << " -> off at +5s" << std::endl;
    std::cout << "Created task: " << scheduledThermostat->getName() << " -> on at +10s" << std::endl;

    std::cout << std::endl << "Checking at current time:" << std::endl;
    checkTask(turnLightOffTask, scheduledLight, "off", currentTime);
    checkTask(turnThermostatOnTask, scheduledThermostat, "on", currentTime);

    std::cout << std::endl << "Checking at current time + 5 seconds:" << std::endl;
    checkTask(turnLightOffTask, scheduledLight, "off", currentTime + 5);
    checkTask(turnThermostatOnTask, scheduledThermostat, "on", currentTime + 5);

    std::cout << std::endl << "Checking at current time + 10 seconds:" << std::endl;
    checkTask(turnLightOffTask, scheduledLight, "off", currentTime + 10);
    checkTask(turnThermostatOnTask, scheduledThermostat, "on", currentTime + 10);

    std::cout << std::endl << "Statuses after scheduled tasks:" << std::endl;
    hub.printAllStatuses();

    std::cout << std::endl << "Removing camera-1..." << std::endl;
    hub.removeDevice("camera-1");

    std::cout << std::endl << "Statuses after removing camera-1:" << std::endl;
    hub.printAllStatuses();

    return 0;
}
