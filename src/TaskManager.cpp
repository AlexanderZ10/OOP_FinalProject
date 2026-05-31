#include "TaskManager.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

TaskManager::TaskManager()
    : currentTime(std::time(nullptr)),
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
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::lock_guard<std::mutex> lock(taskMutex);
        updateCurrentTimeLocked();
        executePendingTasksLocked(false);
    }
}

void TaskManager::addTask(SmartDevice* target, const std::string& command, int delaySeconds) {
    std::lock_guard<std::mutex> lock(taskMutex);
    updateCurrentTimeLocked();
    tasks.emplace_back(target, command, delaySeconds, currentTime);
}

void TaskManager::executePendingTasks() {
    std::lock_guard<std::mutex> lock(taskMutex);
    executePendingTasksLocked(true);
}

bool TaskManager::executePendingTasksLocked(bool showNoTasksMessage) {
    bool executedAnyTask = false;

    for (ScheduledTask& task : tasks) {
        if (task.checkAndExecute(currentTime)) {
            SmartDevice* device = task.getTarget();

            std::ostringstream message;
            message << "Task executed | Device: " << (device != nullptr ? device->getId() : "unknown")
                    << " | Command: " << task.getCommand();
            notifications.push_back(message.str());

            executedAnyTask = true;
        }
    }

    if (!executedAnyTask && showNoTasksMessage) {
        std::cout << "No pending tasks are ready to execute." << std::endl;
    }

    return executedAnyTask;
}

void TaskManager::listTasks() const {
    std::lock_guard<std::mutex> lock(taskMutex);

    if (tasks.empty()) {
        std::cout << "No scheduled tasks." << std::endl;
        return;
    }

    for (const ScheduledTask& task : tasks) {
        SmartDevice* device = task.getTarget();
        long long remainingSeconds = task.getActivateAt() > currentTime
                                         ? static_cast<long long>(task.getActivateAt() - currentTime)
                                         : 0;

        std::cout << "- Device ID: " << (device != nullptr ? device->getId() : "unknown")
                  << " | Command: " << task.getCommand()
                  << " | Activation time: " << formatTime(task.getActivateAt())
                  << " | Status: " << (task.isExecuted() ? "Executed" : "Pending")
                  << " | Remaining time: " << remainingSeconds << "s" << std::endl;
    }
}

std::vector<std::string> TaskManager::drainNotifications() {
    std::lock_guard<std::mutex> lock(taskMutex);
    std::vector<std::string> drainedNotifications = notifications;
    notifications.clear();

    return drainedNotifications;
}

std::time_t TaskManager::getCurrentTime() const {
    std::lock_guard<std::mutex> lock(taskMutex);
    return currentTime;
}

std::string TaskManager::getCurrentTimeText() const {
    std::lock_guard<std::mutex> lock(taskMutex);
    return formatTime(currentTime);
}

std::string TaskManager::formatTime(std::time_t time) const {
    std::tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &time);
#else
    localtime_r(&time, &localTime);
#endif

    std::ostringstream output;
    output << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return output.str();
}

void TaskManager::updateCurrentTimeLocked() {
    currentTime = std::time(nullptr);
}
