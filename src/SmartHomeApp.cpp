#include "SmartHomeApp.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

SmartHomeApp::SmartHomeApp() : running(true) {
}

void SmartHomeApp::run() {
    while (running) {
        printTaskNotifications();
        printMainMenu();

        int choice = readInt("Choose option: ");
        printTaskNotifications();
        handleMainChoice(choice);
    }
}

void SmartHomeApp::printSeparator() const {
    cout << endl << "============================================================" << endl;
}

void SmartHomeApp::printSmallSeparator() const {
    cout << "------------------------------------------------------------" << endl;
}

void SmartHomeApp::printTaskNotifications() {
    vector<string> notifications = taskManager.drainNotifications();

    if (notifications.empty()) {
        return;
    }

    printSmallSeparator();
    cout << "Notifications:" << endl;
    for (const string& notification : notifications) {
        cout << "- " << notification << endl;
    }
    printSmallSeparator();
}

void SmartHomeApp::printMainMenu() const {
    printSeparator();
    cout << "Smart Home Management System" << endl
         << "1. Add device" << endl
         << "2. Remove device" << endl
         << "3. Rename device" << endl
         << "4. Turn device on/off" << endl
         << "5. Change device settings" << endl
         << "6. Execute scene" << endl
         << "7. Show all statuses" << endl
         << "8. Show active devices" << endl
         << "9. Show devices by category" << endl
         << "10. Sort devices by power" << endl
         << "11. Show total power" << endl
         << "12. Energy saving mode" << endl
         << "13. Scheduled tasks" << endl
         << "0. Exit" << endl;
    printSmallSeparator();
}

void SmartHomeApp::handleMainChoice(int choice) {
    switch (choice) {
        case 0:
            running = false;
            cout << "Exiting Smart Home Management System." << endl;
            break;
        case 1:
            addDeviceMenu();
            break;
        case 2:
            removeDeviceMenu();
            break;
        case 3:
            renameDeviceMenu();
            break;
        case 4:
            turnDeviceMenu();
            break;
        case 5:
            changeDeviceSettingsMenu();
            break;
        case 6:
            executeSceneMenu();
            break;
        case 7:
            showAllDevicesMenu();
            break;
        case 8:
            showActiveDevicesMenu();
            break;
        case 9:
            showDevicesByCategoryMenu();
            break;
        case 10:
            sortDevicesByPowerMenu();
            break;
        case 11:
            powerConsumptionMenu();
            break;
        case 12:
            energySavingModeMenu();
            break;
        case 13:
            scheduledTasksMenu();
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
            break;
    }
}
