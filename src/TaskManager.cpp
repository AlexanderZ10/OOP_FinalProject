#include "TaskManager.h"

#include <ctime>
#include <iostream>

TaskManager::TaskManager() : simulatedTime(std::time(nullptr)) {
}

void TaskManager::addTask(SmartDevice* target, const std::string& command, int delaySeconds) {
    tasks.emplace_back(target, command, delaySeconds, simulatedTime);
}

void TaskManager::executePendingTasks() {
    bool executedAnyTask = false;

    for (ScheduledTask& task : tasks) {
        if (task.checkAndExecute(simulatedTime)) {
            SmartDevice* device = task.getTarget();

            std::cout << "Task executed:" << std::endl;
            std::cout << "Device: " << (device != nullptr ? device->getId() : "unknown") << std::endl;
            std::cout << "Command: " << task.getCommand() << std::endl;

            executedAnyTask = true;
        }
    }

    if (!executedAnyTask) {
        std::cout << "No pending tasks are ready to execute." << std::endl;
    }
}

void TaskManager::listTasks() const {
    if (tasks.empty()) {
        std::cout << "No scheduled tasks." << std::endl;
        return;
    }

    for (const ScheduledTask& task : tasks) {
        SmartDevice* device = task.getTarget();

        std::cout << "- Device ID: " << (device != nullptr ? device->getId() : "unknown")
                  << " | Command: " << task.getCommand()
                  << " | Activation time: " << static_cast<long long>(task.getActivateAt())
                  << " | Status: " << (task.isExecuted() ? "executed" : "pending") << std::endl;
    }
}

void TaskManager::advanceTime(int seconds) {
    if (seconds < 0) {
        return;
    }

    simulatedTime += seconds;
}

std::time_t TaskManager::getCurrentTime() const {
    return simulatedTime;
}
