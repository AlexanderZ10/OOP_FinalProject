#include "HomeHub.h"

#include <iostream>
#include <limits>
#include <string>

using std::string;

void printMainMenu() {
    std::cout << std::endl
              << "Smart Home Management System" << std::endl
              << "1. Add device" << std::endl
              << "2. Remove device" << std::endl
              << "3. Rename device" << std::endl
              << "4. Turn device on/off" << std::endl
              << "5. Change device settings" << std::endl
              << "6. Execute scene" << std::endl
              << "7. Show all statuses" << std::endl
              << "8. Show active devices" << std::endl
              << "9. Show devices by category" << std::endl
              << "10. Sort devices by power" << std::endl
              << "11. Show total power" << std::endl
              << "12. Energy saving mode" << std::endl
              << "13. Scheduled tasks" << std::endl
              << "0. Exit" << std::endl;
}

int readInt(const string& prompt) {
    int value;

    while (true) {
        std::cout << prompt;

        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }

        std::cout << "Invalid number. Please try again." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

string readString(const string& prompt) {
    string value;

    std::cout << prompt;
    std::getline(std::cin, value);

    return value;
}

void runMenu() {
    HomeHub hub;
    bool running = true;

    while (running) {
        printMainMenu();

        int choice = readInt("Choose option: ");

        switch (choice) {
            case 0:
                running = false;
                std::cout << "Exiting Smart Home Management System." << std::endl;
                break;
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
                std::cout << "Feature will be implemented in the next step." << std::endl;
                break;
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
                break;
        }
    }

    (void)hub;
}

int main() {
    runMenu();
    return 0;
}
