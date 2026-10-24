#include <iostream>
#include <windows.h>
#include "MMSystem.h"
#include <array>
#include <iomanip>
#include <chrono>
#include <thread>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <algorithm> // For all_of()

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50

using namespace std;

// Global declarations
int i, choice;
double TotalCostHotel = 0.00;
char username[MAX_USERNAME_LENGTH];
char password[MAX_PASSWORD_LENGTH];
double TotalCostRooms = 0.00;
double TotalCostServices = 0.00;
double TotalCostFacilities = 0.00;
double TotalCostEvents = 0.00;

// Constants
const int MAX_ITEMS = 5;

//Function for animation
void setConsoleColor(int color);
void welcomeAnimation();
void exitCountdown();
void pauseForEffect();

// Function declarations for homepage, sign up and login
void showWelcomeBannerPage();
void showExitMessagePage();
void MainMenu();
void signUp();
void logIn();
bool hasSignedUp = false; // Tracks if a user has signed up

// Function declarations for room booking
void displayHotelRoomRsv(); 
void fileResit(const string &filename, string receive);
void roomUpdate(const string &filename, string receive[], int size); // Update room based on provided data
void roomStatus(const string &filename, string receive[], int size); // Displays status room based on data
bool isLeapYear(int year);
bool isValidDate(int year, int month, int day);
int generateOrderID();      
void showWelcomeBannerRoomRsv();
void showExitMessageRoomRsv();
void animatedMenuTitleRoomRsv();
string resit(string name[], int room, int day, int currentYear, int currentMonth, int currentDay, int price, int orderID);
void cancelReservation( string Qreceive[], string Kreceive[], string Sreceive[]);
   
// Function declarations for service
void displayRoomService();
void handleFoodBeverages(double &totalCost); // pass by reference 
void handleHousekeeping();
void handleEntertainment(); 
void receiptService(); 
void showWelcomeBannerService();
void showExitMessageService(); 
void animatedMenuTitleService();

// Function declarations for facility
void displayFacilities();
void handleGym(double &gymPrice);
void handleSpa(double &spaPrice);
void handleBicycle(double &bicyclePrice);
void showWelcomeBannerFacilities();
void showExitMessageFacilities();
void animatedMenuTitleFacilities(); 

// Function declarations for event booking 
void displayEventRsv();
int getValidatedInput(int min, int max);
void displayEventMenu();
void handleReservation();
string getEventType(int choice, double &baseCost);
string getHallType(int choice, double &hallCost, int duration);
string getAddOn(int choice, double &addOnCost);
void displayReceipt(const string &eventType, const string &hallType, int duration, const string &addOn, double totalCostEvent);
void showWelcomeBannerEvent();
void showExitMessageEvent();
void animatedMenuTitleEvent();

// Function declaration for payment
bool isValidCardNumber(const string &cardNumber); // check credit card number validity format
bool isValidExpiryDate(const string &expiryDate); // check validity of card's expiration date
bool isValidCVV(const string &cvv); // check if CVV valid
void displayPaymentOptions();
void processOnlinePayment(double TotalCostHotel);
bool processPayment(float amount, const string &cardNumber, const string &expiryDate, const string &cvv); // processes a payment transaction
void collectPaymentDetails(string &cardNumber, string &expiryDate, string &cvv);
bool validateCardDetails(const string &cardNumber, const string &expiryDate, const string &cvv); // validates overall credit card details
void returnToMenu();
void showRedirectAnimation();
void showWelcomeBannerPayment(); 
void showExitMessagePayment(); 
void animatedMenuTitlePayment(); 
void displayOverallReceipt();

// Set console text color
void setConsoleColor(int color) 
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Exit countdown
void exitCountdown() 
{
    cout << "\nExiting in: ";
    for (int i = 3; i > 0; i--) 
	{
        cout << i << " ";
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << "\nGoodbye!\n";
}

// Pause effect
void pauseForEffect() 
{
    this_thread::sleep_for(chrono::milliseconds(800));
    cout << "."; this_thread::sleep_for(chrono::milliseconds(800));
    cout << "."; this_thread::sleep_for(chrono::milliseconds(800));
    cout << "." << endl;
}

int main() 
{
    PlaySound(TEXT("coffee.wav"), NULL, SND_ASYNC);  // Play the sound asynchronously
	showWelcomeBannerPage();
	int choice;
	bool isLoggedIn = false;
	bool hasSignedUp = false;
	
	do
    {
		system("cls");
		setConsoleColor(13); // Light purple text
		cout << "\t***** WELCOME TO HOTEL DEL LUNA *****\n\n";
    	cout << "Please Sign Up or Log-in to enter Main Menu";
    	cout << "\n(1) Sign Up\n(2) Log In\n(3) Exit\n";
    	cout << "\n=============================================\n\n";
    	cout << "Enter your choice: "; //ask user to enter choice
    	setConsoleColor(15); // Reset text color
    	cin >> choice;

    	switch(choice) //multiple selection for decision making
		{
        	case 1:
           		signUp();
           		pauseForEffect();
           		break;
        	case 2:
            	logIn();
            	break;
        	case 3:
            	system("cls");
				showExitMessagePage(); 
				break;
        	default:
            	setConsoleColor(4); // Red text
				cout << "Invalid choice. Please try again.\n";
				pauseForEffect();
    	}
	}	
	while (choice != 3);
}

// Welcome banner for homepage
void showWelcomeBannerPage() 
{
   setConsoleColor(11); // Light aqua text
   cout << R"(
***********************************************
*                                   	      *
*    WELCOME TO DEL LUNA HOTEL RESERVATION    *
*                                             *
***********************************************
    )" << endl;
   setConsoleColor(15); // Resets text color to white
   pauseForEffect(); // Adds a pause effect
}

// Exit message
void showExitMessagePage() 
{
    setConsoleColor(11); // Light aqua text
    cout << "\nThank you for using Del Luna Hotel Reservation!" << endl;
    cout << "We hope to see you again soon!" << endl;
    cout << R"(
******************************************
*               GOODBYE!                 *
******************************************
    )" << endl;
    cout << "\n\nPress any key to exit...";
    setConsoleColor(15); // White text
}

// Function to check if a username already exists in the file
bool isUsernameTaken(const string& username) {
    ifstream file("users.txt");
    string line;
    while (getline(file, line)) {
        string storedUsername = line.substr(0, line.find(':'));
        if (storedUsername == username) {
            return true;
        }
    }
    return false;
}

// Function to sign up a new user
void signUp() {
    string username, password;
    setConsoleColor(13); // Light purple text
    cout << "Enter username: ";
    setConsoleColor(15); // Reset color text
    cin >> username;

    // Check if username already exists
    if (isUsernameTaken(username)) {
    	setConsoleColor(4); // Red text
        cout << "Username is already taken. Please try again.\n";
        setConsoleColor(15); // Reset color text
        return;
    }

    setConsoleColor(13); // Light purple text
	cout << "Enter password: ";
    setConsoleColor(15); // Reset color text
    cin >> password;

    // Store the username and password in the file
    ofstream file("users.txt", ios::app);
    if (file.is_open()) {
        file << username << ":" << password << endl;
        system("cls"); // Clear the console screen
        setConsoleColor(2); // Green text
		cout << "\nSign up successful!\n";
		setConsoleColor(15); // Reset color text
    } else {
        cerr << "Error: Unable to open file.\n";
    }
}

// Function to log in an existing user
void logIn() {
    string username, password;
    setConsoleColor(13); // Light purple text
    cout << "Enter your username: ";
    setConsoleColor(15); // Reset colour text
    cin >> username;
    
    setConsoleColor(13); // Light purple text
    cout << "Enter your password: ";
    setConsoleColor(15); // Reset color text
    cin >> password;
    
    system("cls"); // Clear the console screen

    ifstream file("users.txt");
    string line;
    bool loginSuccess = false;

    while (getline(file, line)) {
        istringstream iss(line);
        string storedUsername, storedPassword;
        getline(iss, storedUsername, ':');
        getline(iss, storedPassword, ':');

        if (storedUsername == username && storedPassword == password) {
            loginSuccess = true;
            break;
        }
    }

    if (loginSuccess) {
    	setConsoleColor(2); // Green text
        cout << "Login successful! Welcome, " << username << "!\n";
        setConsoleColor(15); // Reset colour text
        pauseForEffect();
		system("cls"); // Clear the console screen
        MainMenu();
    } else {
    	setConsoleColor(4); // Red text
		cout << "----- Username or password incorrect! ------\n";
		setConsoleColor(15); // Reset color text
		pauseForEffect();
		system("cls"); // Clear the console screen
    }
}

void MainMenu() 
{
   int type;
    do 
	{
		system("cls");
		//display the main menu
        setConsoleColor(13); // Light purple text
		cout << "\n----- MAIN MENU OF HOTEL DEL LUNA-----\n";
		setConsoleColor(15); // Reset colour text
        cout << "\n(1) Hotel Room Reservation\n";
        cout << "(2) Room Service\n";
        cout << "(3) Hotel Facilities\n";
        cout << "(4) Hall Reservation\n";
        cout << "(5) Payment Options\n";
        cout << "(0) Exit\n\n";
        setConsoleColor(13); // Light purple text
        cout << "\n=============================================\n\n";
    	cout << "Enter your choice: "; //ask user to enter choice
    	setConsoleColor(15); // Reset text color
        cin >> type;
        
        system("cls");
        
        switch (type)  //allow user to have a selection
		{
            case 1:
                displayHotelRoomRsv(); //funtion call for hotel room reservation
                break;
            case 2:
                displayRoomService(); //funtion call for room service
                break;    
            case 3:
                displayFacilities(); //function call for facilities
                break;
            case 4:
                displayEventRsv(); // funtion call for valet parking
                break;
			case 5:
				while (true)
				{
					displayPaymentOptions();
					// Ask user whether to return to the main menu or exit
					returnToMenu();
				}
				break;	
            case 0:
                cout << "Exiting the program...\n"; // when user enter 0 the main menu and program will close
                break;
            default:
                cout << "Invalid choice. Please try again...\n"; //when user enter choice not 0,1,2,3,4 the system will ask user to re-enter
                break;
        }
    } 
	while (type != 0); // Infinite loop until the user chooses to exit
}

// Welcome banner for service
void showWelcomeBannerRoomRsv() 
{
   system("cls");
   setConsoleColor(11); // Light aqua text
   cout << R"(
******************************************
*                                        *
*    WELCOME TO DEL LUNA ROOM BOOKING    *
*                                        *
******************************************
    )" << endl;
   setConsoleColor(15); // Resets text color to white
   pauseForEffect(); // Adds a pause effect
}

// Exit message
void showExitMessageRoomRsv() 
{
    setConsoleColor(11); // Light aqua text
    cout << "\nThank you for using Del Luna Room Booking!" << endl;
    cout << "We hope to see you again soon!" << endl;
    cout << R"(
******************************************
*               GOODBYE!                 *
******************************************
    )" << endl;
    setConsoleColor(15); // White text
}

// Animated menu title
void animatedMenuTitleRoomRsv() 
{
    string title = "DEL LUNA ROOM BOOKING MENU";
    for (char c : title) {
        cout << c;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    cout << endl;
}

//new file for resit
void fileResit(const string &filename, string receive) 
{
    string line = filename + ".txt";
    ifstream file(line);

    // If the file doesn't exist or is empty, initialize it
    if (!file)
	{
        ofstream ofile(line);
        if (!ofile) 
		{
            cout << "Error opening file." << endl;
            return;
        }

        ofile << receive << endl;
        
        ofile.close();
    }
    file.close();
}

// update the file
void roomUpdate(const string &filename, string receive[], int size) 
{
    ofstream ofile(filename);
    if (!ofile) 
	{
        cout << "Error opening file." << endl;
        return;
    }

    // update data
    for (int i = 0; i < size; i++) 
	{
        ofile << receive[i] << endl;
    }
    ofile.close(); 
}

// function for file
void roomStatus(const string &filename, string receive[], int size) 
{
    string line;
    ifstream file(filename);

    // If the file doesn't exist or is empty, initialize it
    if (!file) 
	{
        ofstream ofile(filename);
        if (!ofile) 
		{
            cout << "Error opening file." << endl;
            return;
        }

        // Write default "AVAILABLE" data into the file
        for (int i = 0; i < 10; i++)
		{
            receive[i] = "AVAILABLE";
            ofile << receive[i] << endl;
        }
        ofile.close();
    }
    // If the file exists, read the data into the array
    for (int i = 0; i < size && getline(file, line); i++) 
	{
        receive[i] = line;  // Store each line in the array
    }
    file.close();
}

// Function to check if a year is a leap year
bool isLeapYear(int year) 
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Function to validate a date
bool isValidDate(int year, int month, int day) 
{
    if (year < 1900 || year > 2100) // Check year range
        return false;
    if (month < 1 || month > 12) // Check month range
        return false;

    // Days in each month
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Adjust for leap year
    if (month == 2 && isLeapYear(year))
        daysInMonth[1] = 29;

    // Check if the day is valid for the given month
    if (day < 1 || day > daysInMonth[month - 1])
        return false;

    return true;
}

// Function to generate a unique order ID for each booking
int generateOrderID() 
{
    time_t now = time(0);
    tm* ltm = localtime(&now);
    return (ltm->tm_hour * 10000) + (ltm->tm_min * 100) + ltm->tm_sec;
}

// Function to handle hotel room reservations
void displayHotelRoomRsv() 
{
    int m, n, inyear, inmonth, inday, outyear, outmonth, outday, checkin_checkout, room, day, Qprice = 250, Kprice = 300, Sprice = 150;
    string Qname[10];
    string Kname[10];
    string Sname[10];
    string checkin;
    string checkout;
    string current;
    tm indate = {}, outdate = {};
    string Qreceive[50];
    const string Qfile = "Qstring.txt";
    string Kreceive[50];
    const string Kfile = "Kstring.txt";
    string Sreceive[50];
    const string Sfile = "Sstring.txt";
    
    showWelcomeBannerRoomRsv();
	system("cls");

    // Get the current date
    time_t t = time(nullptr);
    tm* currentDate = localtime(&t);
    int currentYear = currentDate->tm_year + 1900;
    int currentMonth = currentDate->tm_mon + 1;
    int currentDay = currentDate->tm_mday;
    current=to_string(currentDay)+"/"+to_string(currentMonth)+"/"+to_string(currentYear);

    int x;
    do 
	{
        setConsoleColor(9); // Light blue text
		system("cls");
		animatedMenuTitleRoomRsv();
        cout << "\n\n=============================================\n";
        cout << "               ROOM BOOKING MENU             \n";
        cout << "=============================================\n";
        setConsoleColor(15); // Reset colour text
		cout << "1) Room reservation" << endl;
        cout << "2) Exit" << endl;
        setConsoleColor(9); // Light blue text
		cout << "=============================================\n";
        cout << "Enter your choice: ";
        setConsoleColor(15); // Reset colour text
        cin >> x;
        if (x == 1) 
		{
            do 
			{
        		system("cls");
				
				setConsoleColor(9); // Light blue text
				cout << "Please select the option below:\n" << endl;
				setConsoleColor(15); // Reset colour text
        		cout << "1) Booking room" << endl;
        		cout << "2) Cancellation book" << endl;
        		cout << "3) Back" << endl;
        		setConsoleColor(9); // Light blue text
        		cout << "\n=============================================\n";
        		cout << "\nEnter your choice: ";
        		setConsoleColor(15); // Reset colour text
        		cin >> n;
				
				system("cls");
				
        		if (n == 1) 
				{
            		// to check enter date valid
            		in_date:
               		// Prompt user for check-in date input
                	setConsoleColor(9); // Light blue text
					cout << "Please enter check-in date\n" << endl;
                	cout << "Enter a date (DD MM YYYY): ";
                	setConsoleColor(15); // Reset colour text
                	cin >> inday >> inmonth >> inyear;
                	checkin=to_string(inday)+"/"+to_string(inmonth)+"/"+to_string(inyear);
                	
                	system("cls");

                	// Validate the date
                	if (!isValidDate(inyear, inmonth, inday)) 
					{
                    	setConsoleColor(4); // Red text
						cout << "Invalid date entered! Please enter again" << endl;
                    	goto in_date;
                	}

                	// Populate the tm structure
                	indate.tm_year = inyear - 1900;
                	indate.tm_mon = inmonth - 1;
                	indate.tm_mday = inday;

                	// Convert both dates to time_t
                	time_t inTime = mktime(&indate);

                	if (inyear < currentYear || (inyear == currentYear && inmonth < currentMonth) || (inyear == currentYear && inmonth == currentMonth && inday < currentDay)) 
					{
                    	setConsoleColor(4); // Red text
						cout << "The entered date is in the past. Please enter again" << endl;
                    	goto in_date;
                	} 	
					
					else 
					{
                    	system("cls");
                    	goto out_date;
               		}

            		// to check leave date valid
            		out_date:
                	// Prompt user for check-out date input
                	setConsoleColor(9); // Light blue text
					cout << "Please enter check-out date\n" << endl;
                	cout << "Enter a date (DD MM YYYY): ";
                	setConsoleColor(15); // Reset colour text
                	cin >> outday >> outmonth >> outyear;
                	checkout=to_string(outday)+"/"+to_string(outmonth)+"/"+to_string(outyear);
                	system("cls");

                	// Validate the date
                	if (!isValidDate(outyear, outmonth, outday)) 
					{
                    	setConsoleColor(4); // Red text
						cout << "Invalid date entered! Please enter again" << endl;
                    	goto out_date;
                	}

                	// Populate the tm structure
               		outdate.tm_year = outyear - 1900;
                	outdate.tm_mon = outmonth - 1;
                	outdate.tm_mday = outday;

                	// Convert both dates to time_t
                	time_t outTime = mktime(&outdate);

                	if (outyear < inyear || (outyear == inyear && outmonth < inmonth) || (outyear == inyear && outmonth == inmonth && outday < inday)) 
					{
                    	do 
						{
                        	setConsoleColor(4); // Red text
							cout << "The entered date is behind the check-in date.\n" << endl;
                        	setConsoleColor(15); // Reset colour text
                        	setConsoleColor(9); // Light blue text
							cout << "Click \n1. to change check-in date \n2. to enter checkout date again" << endl;
							setConsoleColor(15); // Reset colour text
                        	cin >> checkin_checkout;
                       		system("cls");

                        	switch (checkin_checkout) 
							{
                            	case 1:
                               		goto in_date;
                            	case 2:
                                	goto out_date;
                            	default:
                                	setConsoleColor(4); // Red text
									cout << "Wrong input, try again" << endl;
                                	break;
                        	}
                    	} 
						while (checkin_checkout != 1 && checkin_checkout != 2);
                	}
				
					else if  (outyear == inyear && outmonth == inmonth && outday == inday) 
					{
                    	do 
						{
                        	setConsoleColor(4); // Red text
							cout << "The entered date is same as the check-in date.\n\n" << endl;
                        	setConsoleColor(15); // Reset colour text
                        	setConsoleColor(9); // Light blue text
							cout << "Click \n1. to change check-in date \n2. to enter checkout date again" << endl;
							setConsoleColor(15); // Reset colour text
                        	cin >> checkin_checkout;
                       		system("cls");

                        	switch (checkin_checkout) 
							{
                            	case 1:
                                	goto in_date;
                            	case 2:
                                	goto out_date;
                            	default:
                                	setConsoleColor(4); // Red text
									cout << "Wrong input, try again" << endl;
                                	break;
                        	}
                    	} 
						while (checkin_checkout != 1 && checkin_checkout != 2);
                	} 
					
					else 
					{
                    	day = (difftime(outTime, inTime) / (60 * 60 * 24)) + 1;
                    	setConsoleColor(9); // Light blue text
						cout << "Your booking is " << day << " day(s) and " << (day - 1) << " night(s)\n" << endl;
                	}

            	choose_room:
                	setConsoleColor(9); // Light blue text
					cout << "=============================================\n";
        			cout << "Enter your choice: ";
					cout << "\nPlease choose room type\n" << endl;
					setConsoleColor(15); // Reset colour text
                	cout << "1) Deluxe Room = RM250 per night" << endl;
                	cout << "2) Suite Room = RM300 per night" << endl;
                	cout << "3) Standard Room = RM150 per night" << endl;
                	cout << "4) Change date" << endl;
                	setConsoleColor(9); // Light blue text
                	cout << "\n=============================================\n";
        			cout << "\nEnter your choice: ";
        			setConsoleColor(15); // Reset colour text
                	cin >> room;
                	system("cls");
                
					switch (room) 
					{
                    	case 1:
                    	    goto queenbed;
                    	case 2:
                        	goto kingbed;
                    	case 3:
                        	goto singlebed;
                    	case 4:
                        	goto in_date;
                    	default:
                        	setConsoleColor(4); // Red text
							cout << "Wrong input! Please try again" << endl;
                        	goto choose_room;
                	}

            	queenbed:
                	roomStatus(Qfile, Qreceive, 50);
                	do 
					{
                    	setConsoleColor(9); // Light blue text
						cout << "Please select room to book" << endl;
                    	cout << "\n=============================================\n\n";
                    	setConsoleColor(15); // Reset colour text
                    	for (int i = 0; i < 10; i++) 
						{
                        	cout << i + 1 << ") room Q0" << i + 1 << " (" << Qreceive[i] << ")" << endl;
                   		}
                    	
						cout << "11) Change room type" << endl;
                    	setConsoleColor(9); // Light blue text
						cout << "\n=============================================\n";
						cout << "\nEnter your choice: ";
						setConsoleColor(15); // Reset colour text
						cin >> m;
                    	system("cls");
                    	
                    	if (m >= 1 && m <= 10) 
						{
							if (Qreceive[m - 1] == "AVAILABLE") 
							{
                            	setConsoleColor(9); // Light blue text
								cout << "Please enter your name: ";
                            	setConsoleColor(15); // Reset colour text
                            	cin >> Qname[m - 1];
                            	cout << "\n=============================================\n\n";
                            	int totalPrice = (day-1) * Qprice;
                            	int orderID = generateOrderID();
                            	string reservationDetails = resit(Qname, m, day, currentYear, currentMonth, currentDay, Qprice, orderID);
                            	cout << "Thank you, your room is successfully booked" << endl;
                            	cout << reservationDetails;
                            	Qreceive[m - 1] = "RESERVED BY " + Qname[m - 1];
                            	Qreceive[(m-1)+ 10]= checkin;
                            	Qreceive[(m-1) + 20]= checkout;
                            	Qreceive[(m - 1)+30] = to_string(orderID);
                            	Qreceive[(m-1)+40]=to_string(totalPrice);
                            	roomUpdate(Qfile, Qreceive, 50);
                            	fileResit(Qreceive[(m - 1)+30], reservationDetails);
                            	setConsoleColor(15); // Reset colour text
                            	showExitMessageRoomRsv();
								system("pause");
                            return;
                        	} 
						
							else 	
							{
                          		setConsoleColor(4); // Red text
								cout << "Sorry, this room has been reserved by " + Qname[m - 1] << endl;
                        	}
                    	} 
						
						else if (m == 11) 
						{
                        	goto choose_room;
                   		} 
						
						else 
						{
                        	setConsoleColor(4); // Red text
							cout << "Wrong input! Try again" << endl;
                    	}
                    	
               		} while (m != 11);

            	kingbed:
                	roomStatus(Kfile, Kreceive, 50);
                	do 
					{
                    	setConsoleColor(9); // Light blue text
						cout << "Please select room to book" << endl;
                    	cout << "\n=============================================\n\n";
                    	setConsoleColor(15); // Reset colour text
                    	for (int i = 0; i < 10; i++) 
						{
                        	cout << i + 1 << ") room K0" << i + 1 << " (" << Kreceive[i] << ")" << endl;
                    	}
                    	
                    	cout << "11) Change room type" << endl;
                    	setConsoleColor(9); // Light blue text
						cout << "\n=============================================\n";
						cout << "\nEnter your choice: ";
        				setConsoleColor(15); // Reset colour text
						cin >> m;
                    	system("cls");
                    	
                    	if (m >= 1 && m <= 10) 
						{
                        	if (Kreceive[m - 1] == "AVAILABLE") 
							{
                            	cout << "Please enter your name: ";
                            	cin >> Kname[m - 1];
                            	cout << "\n=============================================\n\n";
                            	int totalPrice = (day-1) * Kprice;
                            	int orderID = generateOrderID();                          
                            	string reservationDetails = resit(Kname, m, day, currentYear, currentMonth, currentDay, Kprice, orderID);
                           		cout << "Thank you, your room is successfully booked" << endl;
                            	cout << reservationDetails;
                            	Kreceive[m - 1] = "RESERVED BY " + Kname[m - 1];
                            	Kreceive[(m-1)+ 10]= checkin;
                            	Kreceive[(m-1) + 20]= checkout;
                            	Kreceive[(m - 1)+30] = to_string(orderID);
                            	Kreceive[(m-1)+40]=to_string(totalPrice);
                            	roomUpdate(Kfile, Kreceive, 50);
                            	fileResit(Kreceive[(m - 1)+30], reservationDetails);
                            	setConsoleColor(15); // Reset colour text
                            	showExitMessageRoomRsv();
								system("pause");
                            	return;
                        	} 
							else 
							{
                            	setConsoleColor(4); // Red text
								cout << "Sorry, this room has been reserved by " + Kname[m - 1] << endl;
                       		}
                    	} 
						else if (m == 11) 	
						{
                        	goto choose_room;
                    	} 
						else 
						{
                        	setConsoleColor(4); // Red text
							cout << "Wrong input! Try again" << endl;
                   		}
                	} while (m != 11);

            	singlebed:
                	roomStatus(Sfile, Sreceive, 50);
                	do 
					{
                    	setConsoleColor(9); // Light blue text
						cout << "Please select room to book" << endl;
                    	cout << "\n=============================================\n\n";
                    	setConsoleColor(15); // Reset colour text
                    	for (int i = 0; i < 10; i++) 
						{
                        	cout << i + 1 << ") room S0" << i + 1 << " (" << Sreceive[i] << ")" << endl;
                    	}
                    	
						cout << "11) Change room type" << endl;
                    	setConsoleColor(9); // Light blue text
						cout << "\n=============================================\n";
        				cout << "\nEnter your choice: ";
        				setConsoleColor(15); // Reset colour text
						cin >> m;
                    	system("cls");
                    	
						if (m >= 1 && m <= 10) 
						{
                        	if (Sreceive[m - 1] == "AVAILABLE") 
							{
                            	cout << "Please enter your name: ";
                           		cin >> Sname[m - 1];
                           		cout << "\n=============================================\n\n";
                            	int orderID = generateOrderID();
                            	int totalPrice = (day-1) * Sprice;
                            	string reservationDetails = resit(Sname, m, day, currentYear, currentMonth, currentDay, Sprice, orderID);
                            	cout << "Thank you, your room is successfully booked" << endl;
                            	cout << reservationDetails;
                            	Sreceive[m - 1] = "RESERVED BY " + Sname[m - 1];
                            	Sreceive[(m-1)+ 10]= checkin;
                            	Sreceive[(m-1) + 20]= checkout;
                            	Sreceive[(m - 1)+30] = to_string(orderID);
                            	Sreceive[(m-1)+40]=to_string(totalPrice);
                            	roomUpdate(Sfile, Sreceive, 50);
                            	fileResit(Sreceive[(m - 1)+30], reservationDetails);
                            	setConsoleColor(15); // Reset colour text
                            	showExitMessageRoomRsv();
								system("pause");
                            	return;
                        	} 
							else 
							{
                            	setConsoleColor(4); // Red text
								cout << "Sorry, this room has been reserved by " + Sname[m - 1] << endl;
                        	}
                    	} 
						else if (m == 11) 
						{
                        	goto choose_room;
                    	} 	
						else 
						{
                        	setConsoleColor(4); // Red text
							cout << "Wrong input! Try again" << endl;
                    	}
                	} while (m != 11);
        		} 
				else if (n == 2) 
				{
            		// Handle cancellation           	
					roomStatus(Qfile, Qreceive, 50);
					roomStatus(Kfile, Kreceive, 50);
					roomStatus(Sfile, Sreceive, 50);
            		cancelReservation(Qreceive, Kreceive, Sreceive);
            		roomUpdate(Qfile, Qreceive, 50);
            		roomUpdate(Kfile, Kreceive, 50);
            		roomUpdate(Sfile, Sreceive, 50);
        		}
        	
    		} while (n != 3);
    	} 
    } while (x != 2);
}

// Function to cancel a reservation
void cancelReservation( string Qreceive[], string Kreceive[], string Sreceive[]) 
{
    string name;
    int roomType, found = 0;
    string orderID;
	int x;
	string example;

    // Ask for order ID and name to cancel
    setConsoleColor(9); // Light blue text
    cout << "Please enter your order ID to cancel the reservation." << endl;
    cout << "Order ID: ";
    setConsoleColor(15); // Reset colour text
    cin >> orderID;
    
    // Search for the dreservation in Queen, King, and Single rooms
    for (int i = 30; i < 40; i++) 
	{
    	if ((Qreceive[i] == orderID )||
            (Kreceive[i] == orderID ) ||
            (Sreceive[i] == orderID )) 
			{
            	found = 1;

            	// Cancel the reservation by marking the room as available
            	if (Qreceive[i] == orderID) 
				{
            	
            		example=Qreceive[i]+".txt";
            	
            		// Create an input file stream object
    				ifstream inputFile(example);

    				// Check if the file is open
    				if (!inputFile.is_open()) 
					{
    		    		setConsoleColor(4); // Red text
						cerr << "Error opening file!" << endl;
    				}

    				string line;

    				// Read the file line by line
    				while (getline(inputFile, line)) 
					{
    		    		// Output each line from the file
    		    		cout << line << endl;
    				}

    				// Close the file
    				inputFile.close();
    				setConsoleColor(9); // Light blue text
					cout<<"click 1 to continue cancel and 2 for exit: ";
					setConsoleColor(15); // Reset colour text
    				cin>>x;
    				if (x==1)
					{
    					setConsoleColor(9); // Light blue text
						Qreceive[i-30] = "AVAILABLE";
                		Qreceive[i-20]="";
						Qreceive[i-10]="";
						Qreceive[i]="";
						Qreceive[i+10]="";
                		cout << "\nReservation cancelled successfully for Queen room!" << endl;
                		setConsoleColor(15); // Reset colour text
					}
                
            	} 
			
				else if (Kreceive[i] == orderID) 
				{
            		example=Qreceive[i]+".txt";
            		
            		// Create an input file stream object
    				ifstream inputFile(example);

    				// Check if the file is open
    				if (!inputFile.is_open()) 
					{
    		    		setConsoleColor(4); // Red text
						cerr << "Error opening file!" << endl;
    				}

    				string line;

    				// Read the file line by line
    				while (getline(inputFile, line)) 
					{
    		    		// Output each line from the file
    		    		cout << line << endl;
    				}

    				// Close the file
    				inputFile.close();
    				setConsoleColor(9); // Light blue text
					cout<<"click 1 to continue cancel and 2 for exit: ";
					setConsoleColor(15); // Reset colour text
    				cin>>x;
    				if (x==1)
					{
    					setConsoleColor(9); // Light blue text
						Kreceive[i-30] = "AVAILABLE";
                		Kreceive[i-20]="";
						Kreceive[i-10]="";
						Kreceive[i]="";
						Kreceive[i+10]="";
                		cout << "\nReservation cancelled successfully for King room!" << endl;
                		setConsoleColor(15); // Reset colour text
					}
                
            	} 
			
				else if (Sreceive[i] == orderID) 
				{
            		example=Qreceive[i]+".txt";
            	
            		// Create an input file stream object
    				ifstream inputFile(example);

    				// Check if the file is open
    				if (!inputFile.is_open()) 
					{
    		    		setConsoleColor(4); // Red text
						cerr << "Error opening file!" << endl;
    				}

    				string line;

    				// Read the file line by line
    				while (getline(inputFile, line)) 
					{
    		    		// Output each line from the file
    		    		cout << line << endl;
    				}

    				// Close the file
    				inputFile.close();
    				setConsoleColor(9); // Light blue text
					cout<<"click 1 to continue cancel and 2 for exit: ";
					setConsoleColor(15); // Reset colour text
    				cin>>x;
    				if (x==1)
					{
    					setConsoleColor(9); // Light blue text
						Sreceive[i-30] = "AVAILABLE";
                		Sreceive[i-20]="";
						Sreceive[i-10]="";
						Sreceive[i]="";
						Sreceive[i+10]="";
                		cout << "\nReservation cancelled successfully for Single room!" << endl;
                		setConsoleColor(15); // Reset colour text
               		}
            	}
           		break;
        	}
   	}

    if (found == 0) 
	{
        setConsoleColor(4); // Red text
		cout << "Error: Reservation not found with the provided Order ID." << endl;
    }
    system("pause");
    system("cls");
}

string resit(string name[], int room, int day, int currentYear, int currentMonth, int currentDay, int price, int orderID) 
{
    // Create a stringstream to build the result string
    stringstream ss;

    system("cls");
	// Build the reservation summary string
    setConsoleColor(9); // Light blue text
	ss << "\nReservation Summary:\n";
    ss << "Order ID: " << orderID << endl;
    ss << "Name: " << name[room - 1] << endl;
    ss << "Room: " << room << endl;
    ss << "Duration: " << day - 1 << " nights" << endl;
    ss << "Total Price: RM" << (day - 1) * price << endl;
    ss << "Booking Date: " << currentDay << "/" << currentMonth << "/" << currentYear << endl;
    setConsoleColor(15); // Reset colour text
	 
	TotalCostRooms += (day - 1) * price;
	TotalCostHotel = TotalCostRooms + TotalCostServices + TotalCostFacilities + TotalCostEvents;
	
    // Return the constructed string
    return ss.str();
}

// Welcome banner for service
void showWelcomeBannerService() 
{
   system("cls");
   setConsoleColor(11); // Light aqua text
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
void showExitMessageService() 
{
    setConsoleColor(11); // Light aqua text
    cout << "\nThank you for using Del Luna Room Service!" << endl;
    cout << "We hope to see you again soon!" << endl;
    cout << R"(
******************************************
*               GOODBYE!                 *
******************************************
    )" << endl;
    setConsoleColor(15); // Reset colour text
}

// Animated menu title
void animatedMenuTitleService() 
{
    string title = "DEL LUNA ROOM SERVICE MENU";
    for (char c : title) {
        cout << c;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    cout << endl;
}

// Display Room Service menu
void displayRoomService() 
{
    int choice;
    double totalCost = 0.0;

    showWelcomeBannerService();

    do {
        system("cls");
        setConsoleColor(14); // Yellow text
        animatedMenuTitleService();
        cout << "\n\n=============================================\n";
        cout << "               ROOM SERVICE MENU             \n";
        cout << "=============================================\n";
        setConsoleColor(15); // White text
        cout << "1. Food and Beverages\n";
        cout << "2. Housekeeping\n";
        cout << "3. Entertainment\n";
        cout << "4. Exit\n";
        setConsoleColor(14); // Yellow text
        cout << "=============================================\n";
        cout << "Enter your choice: ";
        setConsoleColor(15); // Reset colour text
        cin >> choice;

        switch (choice) 
		{
            case 1: 
			{
                handleFoodBeverages(totalCost);
                break;
            }
            case 2: 
			{
                handleHousekeeping();
                break;
            }
            case 3: 
			{
                handleEntertainment();
                break;
            }
            case 4: 
			{
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
    			TotalCostServices += totalCost;
    			TotalCostHotel = TotalCostRooms + TotalCostServices + TotalCostFacilities + TotalCostEvents;
    			showExitMessageService(); // Displays the goodbye message
    			exitCountdown(); // Displays the countdown before exiting
                break;
            }
            default: 
			{
                setConsoleColor(12); // Red text
                cout << "Invalid choice! Please select a valid option.\n";
                system("pause");
                break;
            }
        }
    } while (choice != 4);
}

// Handle Food and Beverages
void handleFoodBeverages(double &totalCost) 
{
    array<string, MAX_ITEMS> foodItems = {"Spaghetti", "Fish and Chips", "Chicken Chop", "Salmon", "Nasi Lemak"};
    array<string, MAX_ITEMS> beverageItems = {"Water", "Soda", "Green Tea", "Lemonade", "Ice Cappuccino"};
    array<double, MAX_ITEMS> foodCost = {28.99, 26.99, 25.00, 39.99, 24.00};
    array<double, MAX_ITEMS> beverageCost = {5.00, 8.99, 9.90, 7.99, 12.00};

    int foodChoice, beverageChoice;

    system("cls");
    setConsoleColor(13); // Magenta text
    cout << "                ===== FOOD AND BEVERAGES =====\n";
    setConsoleColor(15); // White text
    cout << "Available Food Items:\t\tAvailable Beverage Items:\n";
    for (int i = 0; i < MAX_ITEMS; ++i) 
	{
        cout << i + 1 << ". " << setw(15) << left << foodItems[i] << " - RM" << fixed << setprecision(2)
             << foodCost[i] << "\t" << i + 1 << ". " << setw(15) << left << beverageItems[i] << " - RM"
             << beverageCost[i] << endl;
    }

    cout << "\nEnter food item (based on number): ";
    cin >> foodChoice;
    cout << "Enter beverage item (based on number): ";
    cin >> beverageChoice;

    if (foodChoice < 1 || foodChoice > MAX_ITEMS || beverageChoice < 1 || beverageChoice > MAX_ITEMS) 
	{
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
void handleHousekeeping() 
{
    int roomNumber;

    system("cls");
    setConsoleColor(13); // Magenta text
    cout << "    ===== HOUSEKEEPING SERVICE =====\n";
    setConsoleColor(15); // White text
    
	// Loop until a valid room number is entered
    do 
    {
        cout << "Enter the room number for cleaning: ";
        cin >> roomNumber;
		
        // Validate room number
        if (roomNumber <= 0 || roomNumber > 10) 
        {
            cout << "Error: Invalid room number. Please enter a room number between 1 and 10.\n\n";
        }
    } while (roomNumber <= 0 || roomNumber > 10); // Repeat until valid input

	// Proceed with cleaning
    cout << "Cleaning room " << roomNumber << "...";
    pauseForEffect();
    cout << "\nRoom " << roomNumber << " has been cleaned.\n";
    system("pause");
    
}

// Handle Entertainment
void handleEntertainment() 
{
    array<string, 4> entertainmentOptions = {"Movies", "TV Shows", "Music", "Games"};
    array<string, 4> accessCodes = {"MOVIE2025", "TVSHOW2025", "MUSIC2025", "GAME2025"};

    int choice;
    system("cls");
    setConsoleColor(13); // Magenta text
    cout << "===== ENTERTAINMENT =====\n";
    setConsoleColor(15); // White text
    for (int i = 0; i < entertainmentOptions.size(); ++i) 
	{
        cout << i + 1 << ". " << entertainmentOptions[i] << endl;
    }
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice >= 1 && choice <= 4) 
	{
        cout << "Access Code for " << entertainmentOptions[choice - 1] << ": " << accessCodes[choice - 1] << endl;
    } 
	else if (choice == 0) 
	{
        cout << "Exiting entertainment menu...\n";
    } 
	else 
	{
        cout << "Invalid choice. Please try again.\n";
    }
    system("pause");
}

// Welcome banner
void showWelcomeBannerFacilities() 
{
    setConsoleColor(11); // Light aqua text
    cout << R"(
***********************************************
*                                             *
*       WELCOME TO DEL LUNA FACILITIES        *
*                                             *
***********************************************
    )" << endl;
    setConsoleColor(15); // Resets text color to white
    pauseForEffect(); // Adds a pause effect
}

// Exit message
void showExitMessageFacilities() 
{
    setConsoleColor(11); // Sets text color to cyan
    cout << "\nPlease proceed the payment in counter" << endl;
    cout << "Thank you for using Del Luna Facilities System!" << endl;
    cout << "We hope to see you again soon!" << endl;
    cout << R"(
******************************************
*               GOODBYE!                 *
******************************************
    )" << endl;
    setConsoleColor(15); // Resets text color to white
}

// Welcome animation
void animatedMenuTitleFacilities() 
{
    string welcome = "DEL LUNA FACILITIES MENU";
    for (char c : welcome) // Animates the text character by character
	{ 
        cout << c;
        this_thread::sleep_for(chrono::milliseconds(100)); // Adds a 50ms delay between characters
    }
    cout << endl;
}

// Main menu for hotel facilities
void displayFacilities() 
{
    int numType;
    double gymPrice = 0, spaPrice = 0, bicyclePrice = 0;

    showWelcomeBannerFacilities();

    do 
	{
        system("cls"); 
        setConsoleColor(14); // Sets text color to yellow
        animatedMenuTitleFacilities();
        cout << "\n=============================================\n";
        cout << "              FACILITIES AVAILABLE            \n";
        cout << "=============================================\n";
        setConsoleColor(15); // Resets text color to white
        cout << "1. Gym\n";
        cout << "2. Spa\n";
        cout << "3. Bicycle Rental\n";
        cout << "4. Exit\n";
        setConsoleColor(14); // Yellow text
        cout << "=============================================\n";
        cout << "Enter your choice: ";
        setConsoleColor(15); // Reset colour text
        cin >> numType;

        switch (numType) 
		{
            case 1: 
			{
                handleGym(gymPrice);
                break;
            }
            case 2: 
			{
                handleSpa(spaPrice); 
                break;
            }
            case 3: 
			{
                handleBicycle(bicyclePrice);
                break;
            }
            case 4: 
			{
                system("cls"); 
                double totalPriceFacilities = gymPrice + spaPrice + bicyclePrice;
                cout << fixed << setprecision(2); 
                setConsoleColor(11); // Cyan text
                cout << "\n=============================================\n";
                cout << "             TOTAL PRICE SUMMARY             \n";
                cout << "=============================================\n";
                setConsoleColor(15); //text color white
                cout << left << setw(20) << "Facility" << setw(10) << "Price (RM)" << endl;
                cout << string(30, '-') << endl;
                cout << left << setw(20) << "Gym" << setw(10) << gymPrice << endl;
                cout << left << setw(20) << "Spa" << setw(10) << spaPrice << endl;
                cout << left << setw(20) << "Bicycle Rental" << setw(10) << bicyclePrice << endl;
                cout << string(30, '-') << endl;
                cout << left << setw(20) << "Total" << setw(10) << totalPriceFacilities << endl;
                TotalCostFacilities += totalPriceFacilities;
                TotalCostHotel = TotalCostRooms + TotalCostServices + TotalCostFacilities + TotalCostEvents;
                showExitMessageFacilities(); 
                exitCountdown();
                break;
            }
            default: 
			{
                setConsoleColor(12); // Red text for error messages
                cout << "Invalid choice! Please select a valid option.\n";
                system("pause"); // Pauses to allow the user to read the message
                break;
            }
        }
    } while (numType != 4);
}

// Handles Gym details and pricing
void handleGym(double &gymPrice) 
{
    int numPeople;
    const double pricePerEntry = 25.00;

    system("cls");
    setConsoleColor(13); // Sets text color to magenta
    cout << "\n===== DEL LUNA GYM =====\n";
    setConsoleColor(15); // Resets text color to white
    cout << "Location: Level 2\n";
    cout << "Open: Everyday 10 AM - 10 PM\n";
    cout << "Price per entry: RM" << pricePerEntry << "\n";

    cout << endl << "Enter number of people: ";
    cin >> numPeople;
    gymPrice += numPeople * pricePerEntry; 

    setConsoleColor(10); // Green text for success
    cout << endl << "Total Gym Price: RM" << fixed << setprecision(2) << gymPrice << "\n";
    system("pause"); // Pauses for user acknowledgment
}

// Handles Spa details and pricing
void handleSpa(double &spaPrice) 
{
    int numPeople;
    const double pricePerEntry = 30.00;

    system("cls"); 
    setConsoleColor(13);
    cout << "\n===== DEL LUNA SPA =====\n";
    setConsoleColor(15); 
    cout << "Location: Level 2\n";
    cout << "Open: Everyday 10 AM - 10 PM\n";
    cout << "Price per entry: RM" << pricePerEntry << "\n";

    cout << endl << "Enter number of people: ";
    cin >> numPeople;
    spaPrice += numPeople * pricePerEntry;

    setConsoleColor(10); // Green text
    cout << endl << "Total Spa Price: RM" << fixed << setprecision(2) << spaPrice << "\n";
    system("pause");
}

// Handles Bicycle Rental details and pricing
void handleBicycle(double &bicyclePrice) 
{
    int numPeople, rentHours;
    const double pricePerHour = 10.00;

    system("cls");
    setConsoleColor(13); 
    cout << "\n===== DEL LUNA BICYCLE RENTAL =====\n";
    setConsoleColor(15);
    cout << "Location: Ground Level\n";
    cout << "Open: Everyday 10 AM - 6 PM\n";
    cout << "Price per hour: RM" << pricePerHour << "\n";

    cout << endl << "Enter rental duration (in hours): ";
    cin >> rentHours; 
    cout << "Enter number of people: ";
    cin >> numPeople;

    bicyclePrice += rentHours * numPeople * pricePerHour;
    setConsoleColor(10); // Green text for success
    cout << endl << "Total Bicycle Rental Price: RM" << fixed << setprecision(2) << bicyclePrice << "\n";
    system("pause");
}

// Welcome banner
void showWelcomeBannerEvent() 
{
    setConsoleColor(11); // Light aqua text
    cout << R"(
*****************************************************
*                                        	    *
*       WELCOME TO DEL LUNA HALL RESERVATION        *
*                                        	    *
*****************************************************
    )" << endl;
    setConsoleColor(15); // Resets text color to white
    pauseForEffect(); // Adds a pause effect
}

// Exit message
void showExitMessageEvent() 
{
    setConsoleColor(11); // Sets text color to cyan
    cout << "\nPlease proceed the payment in counter" << endl;
    cout << "Thank you for using Del Luna Hall Reservation System!" << endl;
    cout << "We hope to see you again soon!" << endl;
    cout << R"(
******************************************
*               GOODBYE!                 *
******************************************
    )" << endl;
    setConsoleColor(15); // Resets text color to white
}

// Welcome animation
void animatedMenuTitleEvent() 
{
    string welcome = "DEL LUNA HALL RESERVATION"; // The welcome text
    for (char c : welcome) { // Animates the text character by character
        cout << c;
        this_thread::sleep_for(chrono::milliseconds(100)); // Adds a 50ms delay between characters
    }
    cout << endl;
}

int getValidatedInput(int min, int max) 
{
    int choice;
    while (true) 
	{
        cin >> choice;
        if (!cin.fail() && choice >= min && choice <= max) 
		{
            return choice;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
    }
}

void displayEventMenu() 
{
	setConsoleColor(10); // Light green text
	animatedMenuTitleEvent();
	cout << "\n\n=============================================\n";
    cout << "               HALL RESERVATION MENU             \n";
    cout << "=============================================\n";
    setConsoleColor(15); // White text
    cout << "1. Wedding\n2. Conference\n3. Seminar\n4. Exit System\n";
    setConsoleColor(10); // Light green text
    cout << "=============================================\n";
    cout << "Enter your choice: ";
    setConsoleColor(15); // Reset colour text
}

string getEventType(int choice, double &baseCost) 
{
    switch (choice) 
	{
        case 1: baseCost = 500; return "Wedding";
        case 2: baseCost = 300; return "Conference";
        case 3: baseCost = 200; return "Seminar";
        default: return "";
    }
}

string getHallType(int choice, double &hallCost, int duration) 
{
    switch (choice) 
	{
        case 1: hallCost = 100 * duration; return "Small Hall";
        case 2: hallCost = 150 * duration; return "Medium Hall";
        case 3: hallCost = 200 * duration; return "Large Hall";
        default: return "";
    }
}

string getAddOn(int choice, double &addOnCost) 
{
    switch (choice) 
	{
        case 1: addOnCost = 300; return "Catering";
        case 2: addOnCost = 200; return "Decoration";
        case 3: addOnCost = 450; return "Catering and Decoration";
        case 4: addOnCost = 400; return "Photography";
        default: addOnCost = 0; return "None";
    }
}

void displayReceipt(const string &eventType, const string &hallType, int duration, const string &addOn, double totalCostEvent) 
{
    system("cls");                                                                                                      
	cout << fixed << setprecision(2);
    cout << "\n=============================================\n";
    cout << "             TOTAL PRICE SUMMARY             \n";
    cout << "=============================================\n";
    cout << "Event Type: " << eventType << endl;
    cout << "Hall Type: " << hallType << endl;
    cout << "Duration: " << duration << " hours" << endl;
    cout << "Additional Services: " << addOn << endl;
    cout << "Total Cost: RM" << totalCostEvent << endl;
    TotalCostEvents += totalCostEvent;
    TotalCostHotel = TotalCostRooms + TotalCostServices + TotalCostFacilities + TotalCostEvents;
}
                                         
void displayEventRsv() 
{
    while (true) 
	{
        showWelcomeBannerEvent();
		system("cls");
		
		displayEventMenu();
        int choice = getValidatedInput(1, 4);

        if (choice == 4) 
		{
            
            setConsoleColor(10); // Light green text
			cout << "\nAre you sure you want to exit? (y/n): ";
            char confirm;
            setConsoleColor(15); // Red text
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y') 
			{
                setConsoleColor(10); // Light green text
				cout << "Exiting the system. Goodbye!" << endl;
                break;
            } 
			else 
			{
                continue;
            }
        }

        double baseCost;
        string eventType = getEventType(choice, baseCost);

        system("cls");
		setConsoleColor(10); // Light green text
		cout << "You selected: " << eventType << endl;
		cout << "\n=============================================\n";
        cout << "\nHow many hours would you like to use the hall (hours): ";
        setConsoleColor(15); // Reset colour text
        int duration = getValidatedInput(1, 24);
        double totalCost = baseCost + (duration * 50);

        system("cls");
        setConsoleColor(10); // Light green text
		cout << "Please select the type of hall:" << endl;
		cout << "\n=============================================\n";
		setConsoleColor(15); // Reset colour text
        cout << "1. Small Hall (RM100 per hour)\n2. Medium Hall (RM150 per hour)\n3. Large Hall (RM200 per hour)\n";
        setConsoleColor(10); // Light green text
		cout << "=============================================\n";
		cout << "\nEnter your choice (1-3): ";
        setConsoleColor(15); // Reset colour text
        int hallChoice = getValidatedInput(1, 3);

        double hallCost;
        string hallType = getHallType(hallChoice, hallCost, duration);
        totalCost += hallCost;

        // Handle multiple additional services
        system("cls");
        double addOnCost = 0.00;
        string addOnSummary = "";

        while (true) 
		{
            setConsoleColor(10); // Light green text
			cout << "Would you like to add additional services?" << endl;
            cout << "1. Catering (RM300)\n2. Decoration (RM200)\n3. Both Catering and Decoration (RM450)\n4. Photography (RM400)\n5. None\n";
            cout << "Enter your choice (1-5): ";
            setConsoleColor(15); // Reset colour text
            int addOnChoice = getValidatedInput(1, 5);

            double singleAddOnCost;
            string singleAddOn = getAddOn(addOnChoice, singleAddOnCost);

            if (addOnChoice == 5) 
			{
                break; // Exit the loop if the user selects "None"
            }

            // Accumulate costs and update the summary
            addOnCost += singleAddOnCost;
            if (!addOnSummary.empty()) 
			{
                addOnSummary += ", ";
            }
            addOnSummary += singleAddOn;

            setConsoleColor(10); // Light green text
			cout << "Would you like to add another service? (y/n): ";
            char addMore;
            setConsoleColor(15); //Reset colour text
            cin >> addMore;
            if (addMore == 'n' || addMore == 'N') 
			{
                break;
            }
            system("cls");
        }

        totalCost += addOnCost;

        if (addOnSummary.empty()) 
		{
            addOnSummary = "None";
        }

        displayReceipt(eventType, hallType, duration, addOnSummary, totalCost);

        setConsoleColor(10); // Light green text
		cout << "\nWould you like to confirm this reservation? (y/n): ";
		setConsoleColor(15); // Reset colour text
        char confirm;
        cin >> confirm;

        if (confirm == 'y' || confirm == 'Y') 
		{
            system("cls");
            setConsoleColor(11); // Light aqua text
			displayReceipt(eventType, hallType, duration, addOnSummary, totalCost);
        	showExitMessageEvent();
        	exitCountdown(); 
            break;
        } 
		else 
		{
            setConsoleColor(4); // Red text
			cout << "\nReservation canceled. Returning to main menu.\n";
			setConsoleColor(15); // Reset colour text
            exitCountdown(); 
            system("cls");
        }
    }
}

// Welcome banner
void showWelcomeBannerPayment() 
{
    setConsoleColor(11); // Light aqua text
    cout << R"(
*****************************************************
*                                        	    *
*       WELCOME TO DEL LUNA PAYMENT SYSTEM          *
*                                        	    *
*****************************************************
    )" << endl;
    setConsoleColor(15); // Resets text color to white
    pauseForEffect(); // Adds a pause effect
}

// Exit message
void showExitMessagePayment() 
{
    setConsoleColor(11); // Sets text color to cyan
    cout << "Thank you for using Del Luna Payment System!" << endl;
    cout << "We hope to see you again soon!" << endl;
    cout << R"(
******************************************
*               GOODBYE!                 *
******************************************
    )" << endl;
    setConsoleColor(15); // Resets text color to white
}

// Welcome animation
void animatedMenuTitlePayment() 
{
    string welcome = "DEL LUNA PAYMENT SYSTEM"; // The welcome text
    for (char c : welcome) { // Animates the text character by character
        cout << c;
        this_thread::sleep_for(chrono::milliseconds(100)); // Adds a 50ms delay between characters
    }
    cout << endl;
}

bool isValidCardNumber(const string &cardNumber)
{
    return cardNumber.length() == 16 && all_of(cardNumber.begin(), cardNumber.end(), ::isdigit);
}

bool isValidExpiryDate(const string &expiryDate)
{
    if (expiryDate.length() != 5 || expiryDate[2] != '/')
        return false;

    int month = stoi(expiryDate.substr(0, 2));
    int year = stoi(expiryDate.substr(3, 2));

    int currentYear = 25; // Assuming the current year is 2025
    int currentMonth = 1; // Assuming the current month is January

    if (month < 1 || month > 12)
        return false;

    if (year < currentYear || (year == currentYear && month < currentMonth))
        return false;

    return true;
}

bool isValidCVV(const string &cvv)
{
    return cvv.length() == 3 && all_of(cvv.begin(), cvv.end(), ::isdigit);
}

void displayPaymentOptions()
{
   system("cls");

    int choice;

    setConsoleColor(14); // Light yellow text
    showWelcomeBannerPayment();
    system("cls");
    setConsoleColor(14); // Light yellow text
	animatedMenuTitlePayment();
	cout << "\n\n********** Welcome to Payment Portal **********\n\n";
    cout << "(1) Cash\n";
    cout << "(2) Online Payment\n";
    cout << "Please choose your payment option: ";
    setConsoleColor(15); // Reset colour text
    cin >> choice;
	
	switch (choice)
    {
    case 1:
        setConsoleColor(14); // Light yellow text
		cout << "\nPayment received in cash. Thank you!\n";
        cout << "Please proceed to the front desk.\n\n";
        pauseForEffect();
        displayOverallReceipt();
		system("pause");
		system("cls");
        showExitMessagePayment();
        pauseForEffect();
        system("cls");
        returnToMenu();
        break;
    case 2:
        setConsoleColor(14); // Light yellow text
		cout << "\nRedirecting to online payment portal...";
        showRedirectAnimation(); // Show animation while redirecting
        system("cls");
        processOnlinePayment(TotalCostHotel);
        break;
    default:
        setConsoleColor(4); // Red text
		cout << "\nInvalid choice! Please try again.\n";
        break;
    }
}

void processOnlinePayment(double TotalCostHotel)
{
    string cardNumber, expiryDate, cvv;

    collectPaymentDetails(cardNumber, expiryDate, cvv);

    if (!validateCardDetails(cardNumber, expiryDate, cvv))
    {
        setConsoleColor(4); // Red text
		cout << "Invalid input! Please check your details and try again.\n";
        return;
    }

	cout << "\nProcessing your payment...\n";
	system("cls");

    if (processPayment(TotalCostHotel, cardNumber, expiryDate, cvv))
    {
		setConsoleColor(14); // Light yellow text    
		cout << "----- Payment processed successfully! -----\n\n";
		displayOverallReceipt();
		system("pause");
		system("cls");
    }
    else
    {
        setConsoleColor(4); // Red text
		cout << "Payment failed. Please try again later.\n";
    }
}

bool processPayment(float amount, const string &cardNumber, const string &expiryDate, const string &cvv)
{
    setConsoleColor(14); // Light yellow text  
	cout << "Processing payment of RM " << fixed << setprecision(2) << amount << " with card ending in ..."
         << cardNumber.substr(cardNumber.size() - 4) << "\n\n";
    return true;
}

void collectPaymentDetails(string &cardNumber, string &expiryDate, string &cvv)
{
    setConsoleColor(14); // Light yellow text  
	cout << "Enter card number (16 digits): ";
	setConsoleColor(15); // Reset colour text  
    cin >> cardNumber;

    while (!isValidCardNumber(cardNumber))
    {
        setConsoleColor(4); // Red text  
		cout << "\nInvalid card number! Please try again: ";
		setConsoleColor(15); // Reset colour text 
        cin >> cardNumber;
    }

    setConsoleColor(14); // Light yellow text  
	cout << "Enter expiry date (MM/YY): ";
	setConsoleColor(15); // Reset colour text 
    cin >> expiryDate;

    while (!isValidExpiryDate(expiryDate))
    {
        setConsoleColor(4); // Red text  
		cout << "\nInvalid expiry date! Please use MM/YY format: ";
		setConsoleColor(15); // Reset colour text 
        cin >> expiryDate;
    }

    setConsoleColor(14); // Light yellow text  
	cout << "Enter CVV (3 digits): ";
	setConsoleColor(15); // Reset colour text 
    cin >> cvv;

    while (!isValidCVV(cvv))
    {
        setConsoleColor(4); // Red text
		cout << "\nInvalid CVV! It should be 3 digits: ";
		setConsoleColor(15); // Reset colour text 
        cin >> cvv;
    }
}

bool validateCardDetails(const string &cardNumber, const string &expiryDate, const string &cvv)
{
    return isValidCardNumber(cardNumber) && isValidExpiryDate(expiryDate) && isValidCVV(cvv);
}

void returnToMenu()
{
    int choice;
    setConsoleColor(14); // Light yellow text  
    cout << "\nWould you like to return to the main menu or exit?\n";
    cout << "(1) Return to Main Menu\n";
    cout << "(2) Exit\n";
    cout << "Please enter your choice: ";
    setConsoleColor(15); // Reset colour text 
    cin >> choice;

    if (choice == 1)
    {
    	MainMenu();
	}
	
	if (choice == 2)
    {
        system("cls");
		showExitMessagePayment();
        exit(0); // Exit the program
    }
}

void showRedirectAnimation()
{
    const char spinner[] = {'|', '/', '-', '\\'};
    for (int i = 0; i < 10; ++i) // 10 cycles of animation
    {
        cout << "\rRedirecting to online payment portal... " << spinner[i % 4] << flush;
        this_thread::sleep_for(chrono::milliseconds(200)); // Pause for 200ms
    }
    cout << "\r" << string(50, ' ') << "\r"; // Clear the line
}

void displayOverallReceipt() {
    system("pause");
	system("cls");
    setConsoleColor(11); // Light aqua text
    cout << "\n=============================================\n";
    cout << "            DEL LUNA HOTEL RECEIPT        \n";
    cout << "=============================================\n";
    setConsoleColor(15);
    cout << fixed << setprecision(2);
    cout << left << setw(20) << "Items" << setw(10) << "Price(RM)"<< endl;
    cout << "----------------------------------------------" << endl;
    cout << left << setw(20) << "Rooms" << setw(10) << TotalCostRooms << endl;
    cout << left << setw(20) << "Room Services" << setw(10) << TotalCostServices << endl;
    cout << left << setw(20) << "Facilities" << setw(10) << TotalCostFacilities << endl;
    cout << left << setw(20) << "Hall Event" << setw(10) << TotalCostEvents << endl;
    cout << "----------------------------------------------" << endl;
    cout << left << setw(20) << "Total Cost: RM" << setw(10) << TotalCostHotel << endl;
    setConsoleColor(11);
	cout << "=============================================\n";
    
    setConsoleColor(10);
    cout << "\nThank you for choosing Del Luna Hotel!" << endl;
    cout << "We hope to see you again soon.\n";
}