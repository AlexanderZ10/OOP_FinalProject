#include "ScheduledTask.h"

#include <ctime>

ScheduledTask::ScheduledTask(SmartDevice* target, const std::string& command, int delaySeconds)
    : target(target),
      command(command),
      activateAt(std::time(nullptr) + delaySeconds),
      executed(false) {
}

bool ScheduledTask::checkAndExecute(std::time_t now) {
    if (executed) {
        return false;
    }

    if (now < activateAt) {
        return false;
    }

    if (target != nullptr) {
        if (command == "on") {
            target->turnOn();
        } else if (command == "off") {
            target->turnOff();
        }
    }

    executed = true;
    return true;
}

bool ScheduledTask::isExecuted() const {
    return executed;
}
