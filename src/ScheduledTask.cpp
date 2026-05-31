#include "ScheduledTask.h"

using namespace std;

ScheduledTask::ScheduledTask(SmartDevice* target, const string& command, int delaySeconds, time_t currentTime)
    : target(target),
      command(command),
      activateAt(currentTime + delaySeconds),
      executed(false) {
}

bool ScheduledTask::checkAndExecute(time_t now) {
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

SmartDevice* ScheduledTask::getTarget() const {
    return target;
}

string ScheduledTask::getCommand() const {
    return command;
}

time_t ScheduledTask::getActivateAt() const {
    return activateAt;
}

bool ScheduledTask::isExecuted() const {
    return executed;
}
