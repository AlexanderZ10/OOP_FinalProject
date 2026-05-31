#include "TaskManager.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

TaskManager::TaskManager()
    : currentTime(time(nullptr)),
      running(true),
      workerThread(&TaskManager::backgroundLoop, this) {
}

TaskManager::~TaskManager() {
    running = false;

    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void TaskManager::backgroundLoop() {
    while (running) {
        this_thread::sleep_for(chrono::seconds(1));

        lock_guard<mutex> lock(taskMutex);
        updateCurrentTimeLocked();
        executePendingTasksLocked(false);
    }
}

void TaskManager::addTask(SmartDevice* target, const string& command, int delaySeconds) {
    lock_guard<mutex> lock(taskMutex);
    updateCurrentTimeLocked();
    tasks.emplace_back(target, command, delaySeconds, currentTime);
}

void TaskManager::executePendingTasks() {
    lock_guard<mutex> lock(taskMutex);
    executePendingTasksLocked(true);
}

bool TaskManager::executePendingTasksLocked(bool showNoTasksMessage) {
    bool executedAnyTask = false;

    for (ScheduledTask& task : tasks) {
        if (task.checkAndExecute(currentTime)) {
            SmartDevice* device = task.getTarget();

            ostringstream message;
            message << "Task executed | Device: " << (device != nullptr ? device->getId() : "unknown")
                    << " | Command: " << task.getCommand();
            notifications.push_back(message.str());

            executedAnyTask = true;
        }
    }

    if (!executedAnyTask && showNoTasksMessage) {
        cout << "No pending tasks are ready to execute." << endl;
    }

    return executedAnyTask;
}

void TaskManager::listTasks() const {
    lock_guard<mutex> lock(taskMutex);

    if (tasks.empty()) {
        cout << "No scheduled tasks." << endl;
        return;
    }

    for (const ScheduledTask& task : tasks) {
        SmartDevice* device = task.getTarget();
        long long remainingSeconds = task.getActivateAt() > currentTime
                                         ? static_cast<long long>(task.getActivateAt() - currentTime)
                                         : 0;

        cout << "- Device ID: " << (device != nullptr ? device->getId() : "unknown")
                  << " | Command: " << task.getCommand()
                  << " | Activation time: " << formatTime(task.getActivateAt())
                  << " | Status: " << (task.isExecuted() ? "Executed" : "Pending")
                  << " | Remaining time: " << remainingSeconds << "s" << endl;
    }
}

vector<string> TaskManager::drainNotifications() {
    lock_guard<mutex> lock(taskMutex);
    vector<string> drainedNotifications = notifications;
    notifications.clear();

    return drainedNotifications;
}

time_t TaskManager::getCurrentTime() const {
    lock_guard<mutex> lock(taskMutex);
    return currentTime;
}

string TaskManager::getCurrentTimeText() const {
    lock_guard<mutex> lock(taskMutex);
    return formatTime(currentTime);
}

string TaskManager::formatTime(time_t time) const {
    tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &time);
#else
    localtime_r(&time, &localTime);
#endif

    ostringstream output;
    output << put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return output.str();
}

void TaskManager::updateCurrentTimeLocked() {
    currentTime = time(nullptr);
}
