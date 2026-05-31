#include "Camera.h"
#include "Exceptions.h"
#include "HomeHub.h"
#include "Light.h"
#include "SmartLock.h"
#include "Thermostat.h"

#include <exception>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using std::string;

void printMainMenu() {
    std::cout << std::endl
              << "Smart Home Management System" << std::endl
              << "1. Add device" << std::endl
              << "2. Remove device" << std::endl
              << "3. Rename device" << std::endl
              << "4. Turn device on/off" << std::endl
              << "5. Change device settings" << std::endl
              << "6. Execute scene" << std::endl
              << "7. Show all statuses" << std::endl
              << "8. Show active devices" << std::endl
              << "9. Show devices by category" << std::endl
              << "10. Sort devices by power" << std::endl
              << "11. Show total power" << std::endl
              << "12. Energy saving mode" << std::endl
              << "13. Scheduled tasks" << std::endl
              << "0. Exit" << std::endl;
}

int readInt(const string& prompt) {
    int value;

    while (true) {
        std::cout << prompt;

        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cout << "Invalid number. Please try again." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

string readString(const string& prompt) {
    string value;

    std::cout << prompt;
    std::getline(std::cin, value);

    return value;
}

double readDouble(const string& prompt) {
    double value;

    while (true) {
        std::cout << prompt;

        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cout << "Invalid number. Please try again." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

string readRequiredId() {
    string id;

    do {
        id = readString("Enter device ID: ");
        if (id.empty()) {
            std::cout << "ID cannot be empty." << std::endl;
        }
    } while (id.empty());

    return id;
}

double readValidWattage() {
    double wattage;

    do {
        wattage = readDouble("Enter wattage: ");
        if (wattage < 0) {
            std::cout << "Wattage cannot be negative." << std::endl;
        }
    } while (wattage < 0);

    return wattage;
}

double readNonNegativeDouble(const string& prompt) {
    double value;

    do {
        value = readDouble(prompt);
        if (value < 0) {
            std::cout << "Value cannot be negative." << std::endl;
        }
    } while (value < 0);

    return value;
}

bool isValidDeviceType(const string& type) {
    return type == "Light" || type == "Thermostat" || type == "Camera" || type == "SmartLock";
}

void printDeviceSummary(const SmartDevice* device) {
    if (device == nullptr) {
        return;
    }

    std::cout << "- ID: " << device->getId()
              << " | Name: " << device->getName()
              << " | Type: " << device->getType()
              << " | Wattage: " << device->getWattage() << "W" << std::endl;
}

void printDeviceList(const std::vector<SmartDevice*>& devices, const string& emptyMessage) {
    if (devices.empty()) {
        std::cout << emptyMessage << std::endl;
        return;
    }

    for (const SmartDevice* device : devices) {
        printDeviceSummary(device);
    }
}

void addDeviceMenu(HomeHub& hub) {
    std::cout << std::endl
              << "Select device type:" << std::endl
              << "1. Light" << std::endl
              << "2. Thermostat" << std::endl
              << "3. Camera" << std::endl
              << "4. SmartLock" << std::endl;

    int type = readInt("Choose device type: ");
    if (type < 1 || type > 4) {
        std::cout << "Invalid device type. Device was not added." << std::endl;
        return;
    }

    string id = readRequiredId();
    string name = readString("Enter device name: ");
    double wattage = readValidWattage();

    switch (type) {
        case 1:
            hub.addDevice(new Light(id, name, wattage));
            std::cout << "Light added successfully." << std::endl;
            break;
        case 2:
            hub.addDevice(new Thermostat(id, name, wattage));
            std::cout << "Thermostat added successfully." << std::endl;
            break;
        case 3:
            hub.addDevice(new Camera(id, name, wattage));
            std::cout << "Camera added successfully." << std::endl;
            break;
        case 4: {
            string pin = readString("Enter PIN: ");
            hub.addDevice(new SmartLock(id, name, wattage, pin));
            std::cout << "SmartLock added successfully." << std::endl;
            break;
        }
    }
}

void removeDeviceMenu(HomeHub& hub) {
    string id = readRequiredId();
    hub.removeDevice(id);
    std::cout << "Remove device request completed." << std::endl;
}

void renameDeviceMenu(HomeHub& hub) {
    string id = readRequiredId();
    string newName = readString("Enter new device name: ");

    hub.renameDevice(id, newName);
    std::cout << "Rename device request completed." << std::endl;
}

void showAllDevicesMenu(HomeHub& hub) {
    std::cout << std::endl << "All device statuses:" << std::endl;
    hub.printAllStatuses();
}

void turnDeviceMenu(HomeHub& hub) {
    string id = readRequiredId();
    SmartDevice* device = hub.findDeviceById(id);

    if (device == nullptr) {
        std::cout << "Device not found." << std::endl;
        return;
    }

    std::cout << std::endl
              << "Selected device: " << device->getName() << " (" << device->getType() << ")" << std::endl
              << "1. Turn on" << std::endl
              << "2. Turn off" << std::endl;

    int choice = readInt("Choose action: ");

    if (choice == 1) {
        device->turnOn();
        std::cout << "Device turned on." << std::endl;
    } else if (choice == 2) {
        device->turnOff();
        std::cout << "Device turned off." << std::endl;
    } else {
        std::cout << "Invalid action." << std::endl;
    }
}

void changeLightSettings(Light* light) {
    std::cout << std::endl
              << "Light settings:" << std::endl
              << "1. Change brightness" << std::endl
              << "2. Change color temperature" << std::endl;

    int choice = readInt("Choose setting: ");

    if (choice == 1) {
        int brightness = readInt("Enter brightness (0-100): ");
        light->setBrightness(brightness);
        std::cout << "Brightness updated." << std::endl;
    } else if (choice == 2) {
        int colorTemp = readInt("Enter color temperature: ");
        light->setColorTemp(colorTemp);
        std::cout << "Color temperature updated." << std::endl;
    } else {
        std::cout << "Invalid setting." << std::endl;
    }
}

void changeThermostatSettings(Thermostat* thermostat) {
    double temperature = readDouble("Enter target temperature: ");
    thermostat->setTemp(temperature);
    std::cout << "Temperature updated." << std::endl;
}

void changeCameraSettings(Camera* camera) {
    std::cout << std::endl
              << "Camera settings:" << std::endl
              << "1. Change resolution" << std::endl
              << "2. Start recording" << std::endl
              << "3. Stop recording" << std::endl;

    int choice = readInt("Choose setting: ");

    if (choice == 1) {
        string resolution = readString("Enter resolution (720p, 1080p, 4K): ");
        camera->setResolution(resolution);
        std::cout << "Resolution updated." << std::endl;
    } else if (choice == 2) {
        camera->startRecording();
        std::cout << "Recording started." << std::endl;
    } else if (choice == 3) {
        camera->stopRecording();
        std::cout << "Recording stopped." << std::endl;
    } else {
        std::cout << "Invalid setting." << std::endl;
    }
}

void changeSmartLockSettings(SmartLock* smartLock) {
    std::cout << std::endl
              << "SmartLock settings:" << std::endl
              << "1. Lock" << std::endl
              << "2. Unlock with PIN" << std::endl
              << "3. Change PIN" << std::endl;

    int choice = readInt("Choose setting: ");

    if (choice == 1) {
        smartLock->lock();
        std::cout << "SmartLock locked." << std::endl;
    } else if (choice == 2) {
        string pin = readString("Enter PIN: ");
        if (smartLock->unlock(pin)) {
            std::cout << "SmartLock unlocked." << std::endl;
        } else {
            std::cout << "Invalid PIN." << std::endl;
        }
    } else if (choice == 3) {
        string oldPin = readString("Enter old PIN: ");
        string newPin = readString("Enter new PIN: ");
        smartLock->changePin(oldPin, newPin);
        std::cout << "PIN changed." << std::endl;
    } else {
        std::cout << "Invalid setting." << std::endl;
    }
}

void changeDeviceSettingsMenu(HomeHub& hub) {
    string id = readRequiredId();
    SmartDevice* device = hub.findDeviceById(id);

    if (device == nullptr) {
        std::cout << "Device not found." << std::endl;
        return;
    }

    try {
        string type = device->getType();

        if (type == "Light") {
            changeLightSettings(dynamic_cast<Light*>(device));
        } else if (type == "Thermostat") {
            changeThermostatSettings(dynamic_cast<Thermostat*>(device));
        } else if (type == "Camera") {
            changeCameraSettings(dynamic_cast<Camera*>(device));
        } else if (type == "SmartLock") {
            changeSmartLockSettings(dynamic_cast<SmartLock*>(device));
        } else {
            std::cout << "Unknown device type." << std::endl;
        }
    } catch (const InvalidSettingException& ex) {
        std::cout << "Invalid setting: " << ex.what() << std::endl;
    } catch (const LockedException& ex) {
        std::cout << "Lock error: " << ex.what() << std::endl;
    } catch (const std::exception& ex) {
        std::cout << "Error: " << ex.what() << std::endl;
    }
}

void executeSceneMenu(HomeHub& hub) {
    std::cout << std::endl
              << "Execute scene:" << std::endl
              << "1. Going Out" << std::endl
              << "2. Night Mode" << std::endl
              << "0. Back" << std::endl;

    int choice = readInt("Choose scene: ");

    if (choice == 0) {
        return;
    }

    if (choice == 1) {
        hub.executeScene("going_out");
        std::cout << "Scene executed successfully." << std::endl;
    } else if (choice == 2) {
        hub.executeScene("night_mode");
        std::cout << "Scene executed successfully." << std::endl;
    } else {
        std::cout << "Invalid scene option." << std::endl;
    }
}

void showTotalPowerMenu(HomeHub& hub) {
    std::cout << "Total power consumption: " << hub.calculateTotalPower() << "W" << std::endl;
}

void showActiveDevicesMenu(HomeHub& hub) {
    std::cout << std::endl << "Active devices:" << std::endl;
    printDeviceList(hub.getActiveDevices(), "No active devices.");
}

void showDevicesByCategoryMenu(HomeHub& hub) {
    std::cout << std::endl
              << "Available categories: Light, Thermostat, Camera, SmartLock" << std::endl;

    string type = readString("Enter category: ");
    if (!isValidDeviceType(type)) {
        std::cout << "Invalid category." << std::endl;
        return;
    }

    std::cout << std::endl << type << " devices:" << std::endl;
    printDeviceList(hub.getByCategory(type), "No devices in this category.");
}

void sortDevicesByPowerMenu(HomeHub& hub) {
    std::cout << std::endl << "Devices sorted by power:" << std::endl;
    printDeviceList(hub.getDevicesSortedByPower(), "No devices to sort.");
}

void energySavingModeMenu(HomeHub& hub) {
    double threshold = readNonNegativeDouble("Enter power threshold: ");
    std::vector<SmartDevice*> devicesToTurnOff;

    for (SmartDevice* device : hub.getActiveDevices()) {
        if (device != nullptr && device->getWattage() > threshold) {
            devicesToTurnOff.push_back(device);
        }
    }

    std::cout << std::endl << "Devices that will be turned off:" << std::endl;
    printDeviceList(devicesToTurnOff, "No active devices are above the threshold.");

    hub.energySavingMode(threshold);

    std::cout << "New total power consumption: " << hub.calculateTotalPower() << "W" << std::endl;
}

void powerManagementMenu(HomeHub& hub) {
    bool inPowerMenu = true;

    while (inPowerMenu) {
        std::cout << std::endl
                  << "Power Management" << std::endl
                  << "1. Show total power consumption" << std::endl
                  << "2. Show active devices" << std::endl
                  << "3. Show devices by category" << std::endl
                  << "4. Sort devices by power" << std::endl
                  << "5. Energy saving mode" << std::endl
                  << "0. Back" << std::endl;

        int choice = readInt("Choose option: ");

        switch (choice) {
            case 0:
                inPowerMenu = false;
                break;
            case 1:
                showTotalPowerMenu(hub);
                break;
            case 2:
                showActiveDevicesMenu(hub);
                break;
            case 3:
                showDevicesByCategoryMenu(hub);
                break;
            case 4:
                sortDevicesByPowerMenu(hub);
                break;
            case 5:
                energySavingModeMenu(hub);
                break;
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
                break;
        }
    }
}

void runMenu() {
    HomeHub hub;
    bool running = true;

    while (running) {
        printMainMenu();

        int choice = readInt("Choose option: ");

        switch (choice) {
            case 0:
                running = false;
                std::cout << "Exiting Smart Home Management System." << std::endl;
                break;
            case 1:
                addDeviceMenu(hub);
                break;
            case 2:
                removeDeviceMenu(hub);
                break;
            case 3:
                renameDeviceMenu(hub);
                break;
            case 4:
                turnDeviceMenu(hub);
                break;
            case 5:
                changeDeviceSettingsMenu(hub);
                break;
            case 6:
                executeSceneMenu(hub);
                break;
            case 7:
                showAllDevicesMenu(hub);
                break;
            case 8:
                showActiveDevicesMenu(hub);
                break;
            case 9:
                showDevicesByCategoryMenu(hub);
                break;
            case 10:
                sortDevicesByPowerMenu(hub);
                break;
            case 11:
                showTotalPowerMenu(hub);
                break;
            case 12:
                energySavingModeMenu(hub);
                break;
            case 13:
                std::cout << "Feature will be implemented in the next step." << std::endl;
                break;
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
                break;
        }
    }
}

int main() {
    runMenu();
    return 0;
}
