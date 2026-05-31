#pragma once

#include "HomeHub.h"
#include "TaskManager.h"

#include <string>
#include <vector>

class Camera;
class Light;
class SmartDevice;
class SmartLock;
class Thermostat;

class SmartHomeApp {
private:
    HomeHub hub;
    TaskManager taskManager;
    bool running;

    void printSeparator() const;
    void printSmallSeparator() const;
    void printTaskNotifications();
    void printMainMenu() const;
    void handleMainChoice(int choice);

    int readInt(const std::string& prompt) const;
    int readIntInRange(const std::string& prompt, int minValue, int maxValue) const;
    double readDouble(const std::string& prompt) const;
    std::string readString(const std::string& prompt) const;
    std::string readRequiredId() const;
    double readValidWattage() const;
    double readNonNegativeDouble(const std::string& prompt) const;
    int readNonNegativeInt(const std::string& prompt) const;
    bool isValidDeviceType(const std::string& type) const;
    std::string readUniqueDeviceId();
    SmartDevice* askForDeviceById();
    std::string readValidCommand() const;
    std::string readValidCategory() const;

    void printDeviceSummary(const SmartDevice* device) const;
    void printDeviceList(const std::vector<SmartDevice*>& devices, const std::string& emptyMessage) const;

    void addDeviceMenu();
    void removeDeviceMenu();
    void renameDeviceMenu();
    void showAllDevicesMenu() const;

    void turnDeviceMenu();
    void turnSingleDeviceMenu();
    void turnAllDevicesOnMenu();
    void turnAllDevicesOffMenu();
    void changeDeviceSettingsMenu();
    void changeLightSettings(Light* light);
    void changeThermostatSettings(Thermostat* thermostat);
    void changeCameraSettings(Camera* camera);
    void changeSmartLockSettings(SmartLock* smartLock);

    void executeSceneMenu();
    void showActiveDevicesMenu() const;
    void showDevicesByCategoryMenu();
    void sortDevicesByPowerMenu() const;
    void powerConsumptionMenu() const;
    void energySavingModeMenu();

    void scheduledTasksMenu();
    void createScheduledTaskMenu();
    void listScheduledTasksMenu() const;
    void executePendingTasksMenu();

public:
    SmartHomeApp();

    void run();
};
