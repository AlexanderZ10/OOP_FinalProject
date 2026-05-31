#include "SmartHomeApp.h"

#include <iostream>
#include <limits>
#include <string>

using namespace std;

int SmartHomeApp::readInt(const string& prompt) const {
    int value;

    while (true) {
        cout << prompt;

        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Invalid number. Please try again." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int SmartHomeApp::readIntInRange(const string& prompt, int minValue, int maxValue) const {
    int value;

    do {
        value = readInt(prompt);
        if (value < minValue || value > maxValue) {
            cout << "Invalid option. Please enter a value between "
                 << minValue << " and " << maxValue << "." << endl;
        }
    } while (value < minValue || value > maxValue);

    return value;
}

double SmartHomeApp::readDouble(const string& prompt) const {
    double value;

    while (true) {
        cout << prompt;

        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }

        cout << "Invalid number. Please try again." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string SmartHomeApp::readString(const string& prompt) const {
    string value;

    cout << prompt;
    getline(cin, value);

    return value;
}

string SmartHomeApp::readRequiredId() const {
    string id;

    do {
        id = readString("Enter device ID: ");
        if (id.empty()) {
            cout << "ID cannot be empty." << endl;
        }
    } while (id.empty());

    return id;
}

double SmartHomeApp::readValidWattage() const {
    double wattage;

    do {
        wattage = readDouble("Enter wattage: ");
        if (wattage < 0) {
            cout << "Wattage cannot be negative." << endl;
        }
    } while (wattage < 0);

    return wattage;
}

double SmartHomeApp::readNonNegativeDouble(const string& prompt) const {
    double value;

    do {
        value = readDouble(prompt);
        if (value < 0) {
            cout << "Value cannot be negative." << endl;
        }
    } while (value < 0);

    return value;
}

int SmartHomeApp::readNonNegativeInt(const string& prompt) const {
    int value;

    do {
        value = readInt(prompt);
        if (value < 0) {
            cout << "Value cannot be negative." << endl;
        }
    } while (value < 0);

    return value;
}

bool SmartHomeApp::isValidDeviceType(const string& type) const {
    return type == "Light" || type == "Thermostat" || type == "Camera" || type == "SmartLock";
}

string SmartHomeApp::readUniqueDeviceId() {
    while (true) {
        string id = readRequiredId();

        if (hub.findDeviceById(id) == nullptr) {
            return id;
        }

        cout << "A device with this ID already exists. Please enter a different ID." << endl;
    }
}

SmartDevice* SmartHomeApp::askForDeviceById() {
    while (true) {
        string id = readString("Enter device ID (or 0 to cancel): ");

        if (id == "0") {
            return nullptr;
        }

        if (id.empty()) {
            cout << "ID cannot be empty." << endl;
            continue;
        }

        SmartDevice* device = hub.findDeviceById(id);
        if (device != nullptr) {
            return device;
        }

        cout << "Device not found. Please enter an existing ID." << endl;
    }
}

string SmartHomeApp::readValidCommand() const {
    while (true) {
        string command = readString("Enter command (on/off): ");

        if (command == "on" || command == "off") {
            return command;
        }

        cout << "Invalid command. Use on or off." << endl;
    }
}

string SmartHomeApp::readValidCategory() const {
    while (true) {
        string type = readString("Enter category: ");

        if (isValidDeviceType(type)) {
            return type;
        }

        cout << "Invalid category. Use Light, Thermostat, Camera, or SmartLock." << endl;
    }
}
