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

    bool checkAndExecute(std::time_t now);
    bool isExecuted() const;
};
