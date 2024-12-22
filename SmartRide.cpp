#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Ride structure (doubly linked list node for storing ride details)
struct Ride {
    string rideID;
    string pickupLocation;
    string dropoffLocation;
    Ride* next = nullptr; // pointer to the next ride
    Ride* prev = nullptr; // pointer to the previous ride

    Ride(string id, string pickup, string dropoff) 
        : rideID(id), pickupLocation(pickup), dropoffLocation(dropoff) {}
};

// Class to manage ride history as a doubly linked list
class RideHistory {
private:
    Ride* head; // pointer to the first ride
    Ride* tail; // pointer to the last ride

public:
    RideHistory() : head(nullptr), tail(nullptr) {} // initialize empty list

    // Add a new ride to the history
    void addRide(string id, string pickup, string dropoff) {
        Ride* newRide = new Ride(id, pickup, dropoff);
        if (!head) { // if the list is empty
            head = tail = newRide;
        } else { // append to the end of the list
            tail->next = newRide;
            newRide->prev = tail;
            tail = newRide;
        }
        cout << "Ride added: " << id << endl;
    }

    // Display ride history in backward direction (most recent to oldest)
    void displayHistoryBackward() {
        Ride* current = tail;
        cout << "Ride History (Backward):\n";
        while (current) {
            cout << "Ride ID: " << current->rideID 
                 << ", Pickup: " << current->pickupLocation 
                 << ", Dropoff: " << current->dropoffLocation << endl;
            current = current->prev;
        }
    }
};

// User structure (including ride history)
struct User {
    string username;
    string password;
    bool hasActiveRide = false;
    RideHistory rideHistory; // ride history for the user
};

// Driver structure
struct Driver {
    string username;
    string password;
    bool isAvailable = true;
};

// Hash Table Implementation (for storing users and drivers)
template <typename T>
class HashTable {
private:
    vector<vector<T>> table;
    int size;

    int hashFunction(string key) {
        int hash = 0;
        for (char ch : key) hash += ch;
        return hash % size;
    }

public:
    HashTable(int tableSize) : size(tableSize) {
        table.resize(size);
    }

    void insert(string key, T item) {
        int idx = hashFunction(key);
        table[idx].push_back(item);
    }

    T* search(string key, string password) {
        int idx = hashFunction(key);
        for (auto &item : table[idx]) {
            if (item.username == key && item.password == password) {
                return &item;
            }
        }
        return nullptr;
    }
};

// Main SmartRide System Class
class SmartRide {
private:
    HashTable<User> users;
    HashTable<Driver> drivers;

public:
    SmartRide() : users(10), drivers(10) {}

    void registerUser(string username, string password) {
        users.insert(username, {username, password});
        cout << "User " << username << " registered successfully.\n";
    }

    void registerDriver(string username, string password) {
        drivers.insert(username, {username, password});
        cout << "Driver " << username << " registered successfully.\n";
    }

    User* userLogin(string username, string password) {
        User* user = users.search(username, password);
        if (user) {
            cout << "User " << username << " logged in successfully.\n";
            return user;
        } else {
            cout << "Invalid credentials for user.\n";
            return nullptr;
        }
    }

    Driver* driverLogin(string username, string password) {
        Driver* driver = drivers.search(username, password);
        if (driver) {
            cout << "Driver " << username << " logged in successfully.\n";
            return driver;
        } else {
            cout << "Invalid credentials for driver.\n";
            return nullptr;
        }
    }
};

// Main CLI driver code
int main() {
    SmartRide app;
    string username, password;
    bool running = true;

    while (running) {
        cout << "\n=== SMART RIDE CLI ===\n";
        cout << "1. Register User\n";
        cout << "2. Register Driver\n";
        cout << "3. User Login\n";
        cout << "4. Driver Login\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            app.registerUser(username, password);
            break;

        case 2:
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            app.registerDriver(username, password);
            break;

        case 3: {
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            User* user = app.userLogin(username, password);
            if (user) {
                bool userMenu = true;
                while (userMenu) {
                    cout << "\n--- USER MENU ---\n";
                    cout << "1. Request Ride\n";
                    cout << "2. View Ride History\n";
                    cout << "3. Logout\n";
                    cout << "Enter your choice: ";
                    int userChoice;
                    cin >> userChoice;

                    switch (userChoice) {
                    case 1:
                        if (user->hasActiveRide) {
                            cout << "You already have an active ride.\n";
                        } else {
                            string rideID, pickup, dropoff;
                            cout << "Enter ride ID: ";
                            cin >> rideID;
                            cout << "Enter pickup location: ";
                            cin >> pickup;
                            cout << "Enter dropoff location: ";
                            cin >> dropoff;
                            user->rideHistory.addRide(rideID, pickup, dropoff);
                            user->hasActiveRide = true;
                            cout << "Ride requested successfully.\n";
                        }
                        break;

                    case 2:
                        user->rideHistory.displayHistoryBackward();
                        break;

                    case 3:
                        userMenu = false;
                        break;

                    default:
                        cout << "Invalid choice.\n";
                    }
                }
            }
            break;
        }

        case 4: {
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            Driver* driver = app.driverLogin(username, password);
            if (driver) {
                cout << "Welcome driver " << username << "!\n";
                // Additional driver menu options can be added here
            }
            break;
        }

        case 5:
            running = false;
            break;

        default:
            cout << "Invalid choice. Try again.\n";
        }
    }

    cout << "Exiting SmartRide. Goodbye!\n";
    return 0;
}
