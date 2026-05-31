#pragma once

#include "SmartDevice.h"

#include <ctime>
#include <string>

class ScheduledTask {
private:
    SmartDevice* target;
    std::string command;
    std::time_t activateAt;
    bool executed;

public:
    ScheduledTask(SmartDevice* target, const std::string& command, int delaySeconds);
    ScheduledTask(SmartDevice* target, const std::string& command, int delaySeconds, std::time_t currentTime);

    bool checkAndExecute(std::time_t now);
    SmartDevice* getTarget() const;
    std::string getCommand() const;
    std::time_t getActivateAt() const;
    bool isExecuted() const;
};
