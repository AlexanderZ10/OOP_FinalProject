#pragma once

#include "ScheduledTask.h"
#include "SmartDevice.h"

#include <atomic>
#include <ctime>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class TaskManager {
private:
    std::vector<ScheduledTask> tasks;
    std::time_t currentTime;
    std::vector<std::string> notifications;
    std::atomic<bool> running;
    mutable std::mutex taskMutex;
    std::thread workerThread;

    bool executePendingTasksLocked(bool showNoTasksMessage);
    std::string formatTime(std::time_t time) const;
    void updateCurrentTimeLocked();
    void backgroundLoop();

public:
    TaskManager();
    ~TaskManager();

    TaskManager(const TaskManager&) = delete;
    TaskManager& operator=(const TaskManager&) = delete;

    void addTask(SmartDevice* target, const std::string& command, int delaySeconds);
    void executePendingTasks();
    void listTasks() const;
    std::vector<std::string> drainNotifications();
    std::time_t getCurrentTime() const;
    std::string getCurrentTimeText() const;
};
