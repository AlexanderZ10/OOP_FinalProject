#include "Camera.h"
#include "HomeHub.h"
#include "Light.h"
#include "SmartLock.h"
#include "Thermostat.h"

#include <iostream>
#include <limits>
#include <string>

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
            case 5:
            case 6:
                std::cout << "Feature will be implemented in the next step." << std::endl;
                break;
            case 7:
                showAllDevicesMenu(hub);
                break;
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
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
