#include <iostream>
#include <iomanip>
#include <array>
#include <thread>
#include <chrono>
#include <windows.h>
using namespace std;

// Constants
const int MAX_ITEMS = 5;

// Function declarations
void displayRoomService();
void handleFoodBeverages(double &totalCost);
void handleHousekeeping();
void handleEntertainment();
void setConsoleColor(int color);
void showWelcomeBanner();
void showExitMessage();
void welcomeAnimation();
void exitCountdown();
void pauseForEffect();
void animatedMenuTitle();
void printCentered(const string &text, int width = 50);

// Set console text color
void setConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Function to print centered text
void printCentered(const string &text, int width) {
    int padding = (width - text.length()) / 2;
    cout << string(padding, ' ') << text << endl;
}


// Exit countdown
void exitCountdown() {
    cout << "\nExiting in: ";
    for (int i = 3; i > 0; i--) {
        cout << i << " ";
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << "\nGoodbye!\n";
}

// Pause effect
void pauseForEffect() {
    this_thread::sleep_for(chrono::milliseconds(800));
    cout << "."; this_thread::sleep_for(chrono::milliseconds(800));
    cout << "."; this_thread::sleep_for(chrono::milliseconds(800));
    cout << "." << endl;
}

// Welcome banner
void showWelcomeBanner() {
    setConsoleColor(11); // Green text
    cout << R"(
******************************************
*                                        *
*    WELCOME TO DEL LUNA ROOM SERVICE    *
*                                        *
******************************************
    )" << endl;
    setConsoleColor(15); // Resets text color to white
    pauseForEffect(); // Adds a pause effect
}

// Exit message
void showExitMessage() {
    setConsoleColor(11); // Cyan text
    cout << "\nThank you for using Del Luna Room Service!" << endl;
    cout << "We hope to see you again soon!" << endl;
    cout << R"(
******************************************
*               GOODBYE!                 *
******************************************
    )" << endl;
    setConsoleColor(15); // White text
}

// Animated menu title
void animatedMenuTitle() {
    string title = "ROOM SERVICE MENU";
    for (char c : title) {
        cout << c;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    cout << endl;
}

// Display Room Service menu
void displayRoomService() {
    int choice;
    double totalCost = 0.0;

    showWelcomeBanner();

    do {
        system("cls");
        setConsoleColor(14); // Yellow text
        cout << "\n=============================================\n";
        printCentered("ROOM SERVICE MENU", 45);
        cout << "=============================================\n";
        setConsoleColor(15); // White text
        printCentered("1. Food and Beverages", 45);
        printCentered("2. Housekeeping", 45);
        printCentered("3. Entertainment", 45);
        printCentered("4. Exit", 45);
        setConsoleColor(14); // Yellow text
        cout << "=============================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                handleFoodBeverages(totalCost);
                break;
            }
            case 2: {
                handleHousekeeping();
                break;
            }
            case 3: {
                handleEntertainment();
                break;
            }
            case 4: {
                system("cls"); // Clears the screen before showing the summary
                cout << fixed << setprecision(2); // Ensures prices are displayed with two decimal points
                setConsoleColor(11); // Cyan text for the summary header
                cout << "\n=============================================\n";
                cout << "             TOTAL PRICE SUMMARY             \n";
                cout << "=============================================\n";
                setConsoleColor(15); // Resets text color to white
                cout << left << setw(20) << "Room Service" << setw(10) << "Price (RM)" << endl;
                cout << string(30, '-') << endl;
                cout << left << setw(20) << "Food & Beverage" << setw(10) << totalCost << endl;
                showExitMessage(); // Displays the goodbye message
                exitCountdown(); // Displays the countdown before exiting
                break;
            }
            default: {
                setConsoleColor(12); // Red text
                cout << "Invalid choice! Please select a valid option.\n";
                system("pause");
                break;
            }
        }
    } while (choice != 4);
}

// Handle Food and Beverages
void handleFoodBeverages(double &totalCost) {
    array<string, MAX_ITEMS> foodItems = {"Spaghetti", "Fish and Chips", "Chicken Chop", "Salmon", "Nasi Lemak"};
    array<string, MAX_ITEMS> beverageItems = {"Water", "Soda", "Green Tea", "Lemonade", "Ice Cappuccino"};
    array<double, MAX_ITEMS> foodCost = {28.99, 26.99, 25.00, 39.99, 24.00};
    array<double, MAX_ITEMS> beverageCost = {5.00, 8.99, 9.90, 7.99, 12.00};

    int foodChoice, beverageChoice;

    system("cls");
    setConsoleColor(13); // Magenta text
    printCentered("===== FOOD AND BEVERAGES =====", 50);
    setConsoleColor(15); // White text
    cout << "Available Food Items:\t\tAvailable Beverage Items:\n";
    for (int i = 0; i < MAX_ITEMS; ++i) {
        cout << i + 1 << ". " << setw(15) << left << foodItems[i] << " - RM" << fixed << setprecision(2)
             << foodCost[i] << "\t" << i + 1 << ". " << setw(15) << left << beverageItems[i] << " - RM"
             << beverageCost[i] << endl;
    }

    cout << "\nEnter food item (based on number): ";
    cin >> foodChoice;
    cout << "Enter beverage item (based on number): ";
    cin >> beverageChoice;

    if (foodChoice < 1 || foodChoice > MAX_ITEMS || beverageChoice < 1 || beverageChoice > MAX_ITEMS) {
        cout << "Invalid choice. Returning to the main menu...\n";
        system("pause");
        return;
    }

    double cost = foodCost[foodChoice - 1] + beverageCost[beverageChoice - 1];
    totalCost += cost;

    cout << "\nYou ordered: " << foodItems[foodChoice - 1] << " and " << beverageItems[beverageChoice - 1] << endl;
    cout << "Total cost: RM" << cost << endl;
    system("pause");
}

// Handle Housekeeping
void handleHousekeeping() {
    int roomNumber;
    double cleaningFee = 50.0;

    system("cls");
    setConsoleColor(13); // Magenta text
    printCentered("===== HOUSEKEEPING SERVICE =====", 50);
    setConsoleColor(15); // White text
    cout << "Enter the room number for cleaning: ";
    cin >> roomNumber;

    cout << "Cleaning room " << roomNumber << "...";
    pauseForEffect();
    cout << "\nRoom " << roomNumber << " has been cleaned.\n";

    system("pause");
}

// Handle Entertainment
void handleEntertainment() {
    array<string, 4> entertainmentOptions = {"Movies", "TV Shows", "Music", "Games"};
    array<string, 4> accessCodes = {"MOVIE2025", "TVSHOW2025", "MUSIC2025", "GAME2025"};

    int choice;
    system("cls");
    setConsoleColor(13); // Magenta text
    printCentered("===== ENTERTAINMENT =====", 50);
    setConsoleColor(15); // White text
    for (int i = 0; i < entertainmentOptions.size(); ++i) {
        cout << i + 1 << ". " << entertainmentOptions[i] << endl;
    }
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice >= 1 && choice <= 4) {
        cout << "Access Code for " << entertainmentOptions[choice - 1] << ": " << accessCodes[choice - 1] << endl;
    } else if (choice == 0) {
        cout << "Exiting entertainment menu...\n";
    } else {
        cout << "Invalid choice. Please try again.\n";
    }
    system("pause");
}

// Main function
int main() {
    displayRoomService();
    return 0;
}