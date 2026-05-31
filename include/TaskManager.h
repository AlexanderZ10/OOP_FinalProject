#pragma once

#include "ScheduledTask.h"
#include "SmartDevice.h"

#include <ctime>
#include <string>
#include <vector>

class TaskManager {
private:
    std::vector<ScheduledTask> tasks;
    std::time_t simulatedTime;

public:
    TaskManager();

    void addTask(SmartDevice* target, const std::string& command, int delaySeconds);
    void executePendingTasks();
    void listTasks() const;
    void advanceTime(int seconds);
    std::time_t getCurrentTime() const;
};
