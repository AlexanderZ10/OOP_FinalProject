#include "Camera.h"
#include "Exceptions.h"
#include "HomeHub.h"
#include "Light.h"
#include "SmartLock.h"
#include "TaskManager.h"
#include "Thermostat.h"

#include <exception>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using std::string;

void printSeparator() {
    std::cout << std::endl << "============================================================" << std::endl;
}

void printSmallSeparator() {
    std::cout << "------------------------------------------------------------" << std::endl;
}

void printTaskNotifications(TaskManager& taskManager) {
    std::vector<std::string> notifications = taskManager.drainNotifications();

    if (notifications.empty()) {
        return;
    }

    printSmallSeparator();
    std::cout << "Notifications:" << std::endl;
    for (const string& notification : notifications) {
        std::cout << "- " << notification << std::endl;
    }
    printSmallSeparator();
}

void printMainMenu() {
    printSeparator();
    std::cout << "Smart Home Management System" << std::endl
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
    printSmallSeparator();
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

int readIntInRange(const string& prompt, int minValue, int maxValue) {
    int value;

    do {
        value = readInt(prompt);
        if (value < minValue || value > maxValue) {
            std::cout << "Invalid option. Please enter a value between "
                      << minValue << " and " << maxValue << "." << std::endl;
        }
    } while (value < minValue || value > maxValue);

    return value;
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

int readNonNegativeInt(const string& prompt) {
    int value;

    do {
        value = readInt(prompt);
        if (value < 0) {
            std::cout << "Value cannot be negative." << std::endl;
        }
    } while (value < 0);

    return value;
}

bool isValidDeviceType(const string& type) {
    return type == "Light" || type == "Thermostat" || type == "Camera" || type == "SmartLock";
}

string readUniqueDeviceId(HomeHub& hub) {
    string id;

    while (true) {
        id = readRequiredId();

        if (hub.findDeviceById(id) == nullptr) {
            return id;
        }

        std::cout << "A device with this ID already exists. Please enter a different ID." << std::endl;
    }
}

SmartDevice* readExistingDevice(HomeHub& hub) {
    while (true) {
        string id = readString("Enter device ID (or 0 to cancel): ");

        if (id == "0") {
            return nullptr;
        }

        if (id.empty()) {
            std::cout << "ID cannot be empty." << std::endl;
            continue;
        }

        SmartDevice* device = hub.findDeviceById(id);
        if (device != nullptr) {
            return device;
        }

        std::cout << "Device not found. Please enter an existing ID." << std::endl;
    }
}

string readValidCommand() {
    while (true) {
        string command = readString("Enter command (on/off): ");

        if (command == "on" || command == "off") {
            return command;
        }

        std::cout << "Invalid command. Use on or off." << std::endl;
    }
}

string readValidCategory() {
    while (true) {
        string type = readString("Enter category: ");

        if (isValidDeviceType(type)) {
            return type;
        }

        std::cout << "Invalid category. Use Light, Thermostat, Camera, or SmartLock." << std::endl;
    }
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
              << "4. SmartLock (requires PIN)" << std::endl;

    int type = readIntInRange("Choose device type: ", 1, 4);
    string id = readUniqueDeviceId(hub);
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
            string pin = readString("Enter initial PIN for SmartLock: ");
            hub.addDevice(new SmartLock(id, name, wattage, pin));
            std::cout << "SmartLock added successfully. Use this PIN to unlock or change the PIN later." << std::endl;
            break;
        }
    }
}

void removeDeviceMenu(HomeHub& hub) {
    SmartDevice* device = readExistingDevice(hub);
    if (device == nullptr) {
        std::cout << "Remove cancelled." << std::endl;
        return;
    }

    hub.removeDevice(device->getId());
    std::cout << "Device removed successfully." << std::endl;
}

void renameDeviceMenu(HomeHub& hub) {
    SmartDevice* device = readExistingDevice(hub);
    if (device == nullptr) {
        std::cout << "Rename cancelled." << std::endl;
        return;
    }

    string newName = readString("Enter new device name: ");

    hub.renameDevice(device->getId(), newName);
    std::cout << "Device renamed successfully." << std::endl;
}

void showAllDevicesMenu(HomeHub& hub) {
    std::cout << std::endl << "All device statuses:" << std::endl;
    hub.printAllStatuses();
}

void turnDeviceMenu(HomeHub& hub) {
    SmartDevice* device = readExistingDevice(hub);
    if (device == nullptr) {
        std::cout << "Turn device action cancelled." << std::endl;
        return;
    }

    std::cout << std::endl
              << "Selected device: " << device->getName() << " (" << device->getType() << ")" << std::endl
              << "1. Turn on" << std::endl
              << "2. Turn off" << std::endl;

    int choice = readIntInRange("Choose action: ", 1, 2);

    if (choice == 1) {
        device->turnOn();
        std::cout << "Device turned on." << std::endl;
    } else {
        device->turnOff();
        std::cout << "Device turned off." << std::endl;
    }
}

void changeLightSettings(Light* light) {
    std::cout << std::endl
              << "Light settings:" << std::endl
              << "1. Change brightness" << std::endl
              << "2. Change color temperature" << std::endl;

    int choice = readIntInRange("Choose setting: ", 1, 2);

    if (choice == 1) {
        while (true) {
            try {
                int brightness = readInt("Enter brightness (0-100): ");
                light->setBrightness(brightness);
                std::cout << "Brightness updated." << std::endl;
                return;
            } catch (const InvalidSettingException& ex) {
                std::cout << "Invalid setting: " << ex.what() << std::endl;
            }
        }
    } else {
        int colorTemp = readInt("Enter color temperature: ");
        light->setColorTemp(colorTemp);
        std::cout << "Color temperature updated." << std::endl;
    }
}

void changeThermostatSettings(Thermostat* thermostat) {
    while (true) {
        try {
            double temperature = readDouble("Enter target temperature: ");
            thermostat->setTemp(temperature);
            std::cout << "Temperature updated." << std::endl;
            return;
        } catch (const InvalidSettingException& ex) {
            std::cout << "Invalid setting: " << ex.what() << std::endl;
        }
    }
}

void changeCameraSettings(Camera* camera) {
    std::cout << std::endl
              << "Camera settings:" << std::endl
              << "1. Change resolution" << std::endl
              << "2. Start recording" << std::endl
              << "3. Stop recording" << std::endl;

    int choice = readIntInRange("Choose setting: ", 1, 3);

    if (choice == 1) {
        while (true) {
            try {
                string resolution = readString("Enter resolution (720p, 1080p, 4K): ");
                camera->setResolution(resolution);
                std::cout << "Resolution updated." << std::endl;
                return;
            } catch (const InvalidSettingException& ex) {
                std::cout << "Invalid setting: " << ex.what() << std::endl;
            }
        }
    } else if (choice == 2) {
        camera->startRecording();
        std::cout << "Recording started." << std::endl;
    } else {
        camera->stopRecording();
        std::cout << "Recording stopped." << std::endl;
    }
}

void changeSmartLockSettings(SmartLock* smartLock) {
    std::cout << std::endl
              << "SmartLock settings:" << std::endl
              << "1. Lock" << std::endl
              << "2. Unlock with PIN" << std::endl
              << "3. Change PIN" << std::endl;

    int choice = readIntInRange("Choose setting: ", 1, 3);

    if (choice == 1) {
        smartLock->lock();
        std::cout << "SmartLock locked." << std::endl;
    } else if (choice == 2) {
        while (true) {
            string pin = readString("Enter PIN: ");
            if (smartLock->unlock(pin)) {
                std::cout << "SmartLock unlocked." << std::endl;
                return;
            }

            std::cout << "Invalid PIN. Please try again." << std::endl;
        }
    } else {
        while (true) {
            try {
                string oldPin = readString("Enter old PIN: ");
                string newPin = readString("Enter new PIN: ");
                smartLock->changePin(oldPin, newPin);
                std::cout << "PIN changed." << std::endl;
                return;
            } catch (const InvalidSettingException& ex) {
                std::cout << "Invalid setting: " << ex.what() << std::endl;
            }
        }
    }
}

void changeDeviceSettingsMenu(HomeHub& hub) {
    SmartDevice* device = readExistingDevice(hub);
    if (device == nullptr) {
        std::cout << "Change settings cancelled." << std::endl;
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

    int choice = readIntInRange("Choose scene: ", 0, 2);

    if (choice == 0) {
        return;
    }

    if (choice == 1) {
        hub.executeScene("going_out");
        std::cout << "Scene executed successfully." << std::endl;
    } else {
        hub.executeScene("night_mode");
        std::cout << "Scene executed successfully." << std::endl;
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

    string type = readValidCategory();

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

void createScheduledTaskMenu(HomeHub& hub, TaskManager& taskManager) {
    SmartDevice* device = readExistingDevice(hub);
    if (device == nullptr) {
        std::cout << "Create task cancelled." << std::endl;
        return;
    }

    string command = readValidCommand();
    int delaySeconds = readNonNegativeInt("Enter delay in seconds: ");
    taskManager.addTask(device, command, delaySeconds);

    std::cout << "Scheduled task created." << std::endl;
}

void scheduledTasksMenu(HomeHub& hub, TaskManager& taskManager) {
    bool inTaskMenu = true;

    while (inTaskMenu) {
        printTaskNotifications(taskManager);

        printSeparator();
        std::cout << "Scheduled Tasks" << std::endl
                  << "Current time: " << taskManager.getCurrentTimeText() << std::endl
                  << "1. Create Task" << std::endl
                  << "2. List Tasks" << std::endl
                  << "3. Execute Pending Tasks" << std::endl
                  << "0. Back" << std::endl;
        printSmallSeparator();

        int choice = readInt("Choose option: ");
        printTaskNotifications(taskManager);

        switch (choice) {
            case 0:
                inTaskMenu = false;
                break;
            case 1:
                createScheduledTaskMenu(hub, taskManager);
                break;
            case 2:
                taskManager.listTasks();
                break;
            case 3:
                taskManager.executePendingTasks();
                break;
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
                break;
        }
    }
}

void runMenu() {
    HomeHub hub;
    TaskManager taskManager;
    bool running = true;

    while (running) {
        printTaskNotifications(taskManager);
        printMainMenu();

        int choice = readInt("Choose option: ");
        printTaskNotifications(taskManager);

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
                scheduledTasksMenu(hub, taskManager);
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
