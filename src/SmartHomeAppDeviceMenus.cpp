#include "SmartHomeApp.h"

#include "Camera.h"
#include "Exceptions.h"
#include "Light.h"
#include "SmartLock.h"
#include "Thermostat.h"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void SmartHomeApp::printDeviceSummary(const SmartDevice* device) const {
    if (device == nullptr) {
        return;
    }

    cout << "- ID: " << device->getId()
         << " | Name: " << device->getName()
         << " | Type: " << device->getType()
         << " | Wattage: " << device->getWattage() << "W" << endl;
}

void SmartHomeApp::printDeviceList(const vector<SmartDevice*>& devices,
                                   const string& emptyMessage) const {
    if (devices.empty()) {
        cout << emptyMessage << endl;
        return;
    }

    for (const SmartDevice* device : devices) {
        printDeviceSummary(device);
    }
}

void SmartHomeApp::addDeviceMenu() {
    cout << endl
         << "Select device type:" << endl
         << "1. Light" << endl
         << "2. Thermostat" << endl
         << "3. Camera" << endl
         << "4. SmartLock (requires PIN)" << endl;

    int type = readIntInRange("Choose device type: ", 1, 4);
    string id = readUniqueDeviceId();
    string name = readString("Enter device name: ");
    double wattage = readValidWattage();

    switch (type) {
        case 1:
            hub.addDevice(new Light(id, name, wattage));
            cout << "Light added successfully." << endl;
            break;
        case 2:
            hub.addDevice(new Thermostat(id, name, wattage));
            cout << "Thermostat added successfully." << endl;
            break;
        case 3:
            hub.addDevice(new Camera(id, name, wattage));
            cout << "Camera added successfully." << endl;
            break;
        case 4: {
            string pin = readString("Enter initial PIN for SmartLock: ");
            hub.addDevice(new SmartLock(id, name, wattage, pin));
            cout << "SmartLock added successfully. Use this PIN to unlock or change the PIN later." << endl;
            break;
        }
    }
}

void SmartHomeApp::removeDeviceMenu() {
    SmartDevice* device = askForDeviceById();
    if (device == nullptr) {
        cout << "Remove cancelled." << endl;
        return;
    }

    hub.removeDevice(device->getId());
    cout << "Device removed successfully." << endl;
}

void SmartHomeApp::renameDeviceMenu() {
    SmartDevice* device = askForDeviceById();
    if (device == nullptr) {
        cout << "Rename cancelled." << endl;
        return;
    }

    string newName = readString("Enter new device name: ");

    hub.renameDevice(device->getId(), newName);
    cout << "Device renamed successfully." << endl;
}

void SmartHomeApp::showAllDevicesMenu() const {
    cout << endl << "All device statuses:" << endl;
    hub.printAllStatuses();
}

void SmartHomeApp::turnDeviceMenu() {
    cout << endl
         << "Device power control:" << endl
         << "1. Turn one device on/off" << endl
         << "2. Turn all devices on" << endl
         << "3. Turn all devices off" << endl
         << "0. Back" << endl;

    int menuChoice = readIntInRange("Choose action: ", 0, 3);

    switch (menuChoice) {
        case 0:
            return;
        case 1:
            turnSingleDeviceMenu();
            return;
        case 2:
            turnAllDevicesOnMenu();
            return;
        case 3:
            turnAllDevicesOffMenu();
            return;
    }
}

void SmartHomeApp::turnSingleDeviceMenu() {
    SmartDevice* device = askForDeviceById();
    if (device == nullptr) {
        cout << "Turn device action cancelled." << endl;
        return;
    }

    cout << endl
         << "Selected device: " << device->getName() << " (" << device->getType() << ")" << endl
         << "1. Turn on" << endl
         << "2. Turn off" << endl;

    int choice = readIntInRange("Choose action: ", 1, 2);

    if (choice == 1) {
        device->turnOn();
        cout << "Device turned on." << endl;
        return;
    }

    device->turnOff();
    cout << "Device turned off." << endl;
}

void SmartHomeApp::turnAllDevicesOnMenu() {
    hub.turnAllOn();
    cout << "All devices turned on." << endl;
}

void SmartHomeApp::turnAllDevicesOffMenu() {
    hub.turnAllOff();
    cout << "All devices turned off." << endl;
}

void SmartHomeApp::changeLightSettings(Light* light) {
    cout << endl
         << "Light settings:" << endl
         << "1. Change brightness" << endl
         << "2. Change color temperature" << endl;

    int choice = readIntInRange("Choose setting: ", 1, 2);

    if (choice == 1) {
        while (true) {
            try {
                int brightness = readInt("Enter brightness (0-100): ");
                light->setBrightness(brightness);
                cout << "Brightness updated." << endl;
                return;
            } catch (const InvalidSettingException& ex) {
                cout << "Invalid setting: " << ex.what() << endl;
            }
        }
    }

    int colorTemp = readInt("Enter color temperature: ");
    light->setColorTemp(colorTemp);
    cout << "Color temperature updated." << endl;
}

void SmartHomeApp::changeThermostatSettings(Thermostat* thermostat) {
    while (true) {
        try {
            double temperature = readDouble("Enter target temperature: ");
            thermostat->setTemp(temperature);
            cout << "Temperature updated." << endl;
            return;
        } catch (const InvalidSettingException& ex) {
            cout << "Invalid setting: " << ex.what() << endl;
        }
    }
}

void SmartHomeApp::changeCameraSettings(Camera* camera) {
    cout << endl
         << "Camera settings:" << endl
         << "1. Change resolution" << endl
         << "2. Start recording" << endl
         << "3. Stop recording" << endl;

    int choice = readIntInRange("Choose setting: ", 1, 3);

    if (choice == 1) {
        while (true) {
            try {
                string resolution = readString("Enter resolution (720p, 1080p, 4K): ");
                camera->setResolution(resolution);
                cout << "Resolution updated." << endl;
                return;
            } catch (const InvalidSettingException& ex) {
                cout << "Invalid setting: " << ex.what() << endl;
            }
        }
    }

    if (choice == 2) {
        camera->startRecording();
        cout << "Recording started." << endl;
        return;
    }

    camera->stopRecording();
    cout << "Recording stopped." << endl;
}

void SmartHomeApp::changeSmartLockSettings(SmartLock* smartLock) {
    cout << endl
         << "SmartLock settings:" << endl
         << "1. Lock" << endl
         << "2. Unlock with PIN" << endl
         << "3. Change PIN" << endl;

    int choice = readIntInRange("Choose setting: ", 1, 3);

    if (choice == 1) {
        smartLock->lock();
        cout << "SmartLock locked." << endl;
        return;
    }

    if (choice == 2) {
        while (true) {
            string pin = readString("Enter PIN: ");
            if (smartLock->unlock(pin)) {
                cout << "SmartLock unlocked." << endl;
                return;
            }

            cout << "Invalid PIN. Please try again." << endl;
        }
    }

    while (true) {
        try {
            string oldPin = readString("Enter old PIN: ");
            string newPin = readString("Enter new PIN: ");
            smartLock->changePin(oldPin, newPin);
            cout << "PIN changed." << endl;
            return;
        } catch (const InvalidSettingException& ex) {
            cout << "Invalid setting: " << ex.what() << endl;
        }
    }
}

void SmartHomeApp::changeDeviceSettingsMenu() {
    SmartDevice* device = askForDeviceById();
    if (device == nullptr) {
        cout << "Change settings cancelled." << endl;
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
            cout << "Unknown device type." << endl;
        }
    } catch (const InvalidSettingException& ex) {
        cout << "Invalid setting: " << ex.what() << endl;
    } catch (const LockedException& ex) {
        cout << "Lock error: " << ex.what() << endl;
    } catch (const exception& ex) {
        cout << "Error: " << ex.what() << endl;
    }
}
