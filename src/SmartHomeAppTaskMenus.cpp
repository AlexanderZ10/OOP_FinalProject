#include "SmartHomeApp.h"

#include <iostream>
#include <string>

using namespace std;

void SmartHomeApp::scheduledTasksMenu() {
    bool inTaskMenu = true;

    while (inTaskMenu) {
        printTaskNotifications();

        printSeparator();
        cout << "Scheduled Tasks" << endl
             << "Current time: " << taskManager.getCurrentTimeText() << endl
             << "1. Create Task" << endl
             << "2. List Tasks" << endl
             << "3. Execute Pending Tasks" << endl
             << "0. Back" << endl;
        printSmallSeparator();

        int choice = readInt("Choose option: ");
        printTaskNotifications();

        switch (choice) {
            case 0:
                inTaskMenu = false;
                break;
            case 1:
                createScheduledTaskMenu();
                break;
            case 2:
                listScheduledTasksMenu();
                break;
            case 3:
                executePendingTasksMenu();
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
                break;
        }
    }
}

void SmartHomeApp::createScheduledTaskMenu() {
    SmartDevice* device = askForDeviceById();
    if (device == nullptr) {
        cout << "Create task cancelled." << endl;
        return;
    }

    string command = readValidCommand();
    int delaySeconds = readNonNegativeInt("Enter delay in seconds: ");
    taskManager.addTask(device, command, delaySeconds);

    cout << "Scheduled task created." << endl;
}

void SmartHomeApp::listScheduledTasksMenu() const {
    taskManager.listTasks();
}

void SmartHomeApp::executePendingTasksMenu() {
    taskManager.executePendingTasks();
}
