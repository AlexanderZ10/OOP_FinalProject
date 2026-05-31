#include "SmartHomeApp.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void SmartHomeApp::executeSceneMenu() {
    cout << endl
         << "Execute scene:" << endl
         << "1. Going Out" << endl
         << "2. Night Mode" << endl
         << "0. Back" << endl;

    int choice = readIntInRange("Choose scene: ", 0, 2);

    if (choice == 0) {
        return;
    }

    if (choice == 1) {
        hub.executeScene("going_out");
    } else {
        hub.executeScene("night_mode");
    }

    cout << "Scene executed successfully." << endl;
}

void SmartHomeApp::showActiveDevicesMenu() const {
    cout << endl << "Active devices:" << endl;
    printDeviceList(hub.getActiveDevices(), "No active devices.");
}

void SmartHomeApp::showDevicesByCategoryMenu() {
    cout << endl
         << "Available categories: Light, Thermostat, Camera, SmartLock" << endl;

    string type = readValidCategory();

    cout << endl << type << " devices:" << endl;
    printDeviceList(hub.getByCategory(type), "No devices in this category.");
}

void SmartHomeApp::sortDevicesByPowerMenu() const {
    cout << endl << "Devices sorted by power:" << endl;
    printDeviceList(hub.getDevicesSortedByPower(), "No devices to sort.");
}

void SmartHomeApp::powerConsumptionMenu() const {
    cout << "Total power consumption: " << hub.calculateTotalPower() << "W" << endl;
}

void SmartHomeApp::energySavingModeMenu() {
    double threshold = readNonNegativeDouble("Enter power threshold: ");
    vector<SmartDevice*> devicesToTurnOff;

    for (SmartDevice* device : hub.getActiveDevices()) {
        if (device != nullptr && device->getWattage() > threshold) {
            devicesToTurnOff.push_back(device);
        }
    }

    cout << endl << "Devices that will be turned off:" << endl;
    printDeviceList(devicesToTurnOff, "No active devices are above the threshold.");

    hub.energySavingMode(threshold);

    cout << "New total power consumption: " << hub.calculateTotalPower() << "W" << endl;
}
