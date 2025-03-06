#include <iostream>
#include <vector>

using namespace std;

// Interface for Extendability
class IService {
public:
    virtual void execute() = 0;
    virtual ~IService() = default;
};

// Welcome Screen (UI Handler)
class WelcomeScreen {
public:
    static void display() {
        cout << "**** Welcome to XYZ Hospital Management System ****" << endl;
        cout << "[1] Hospital Services" << endl;
        cout << "[2] Cafe" << endl;
        cout << "[0] Exit" << endl;
    }
};

// Abstract Bed Manager (LSP Applied)
class IBedManager {
public:
    virtual void showCapacity() const = 0;
    virtual bool allocateBed() = 0;
    virtual ~IBedManager() = default;
};

// General Bed Manager (LSP Compliant)
class GeneralBedManager : public IBedManager {
protected:
    int capacity;
public:
    GeneralBedManager(int cap) : capacity(cap) {}

    void showCapacity() const override { 
        cout << "Available General Beds: " << capacity << endl;
    }

    bool allocateBed() override {
        if (capacity > 0) {
            capacity--;
            return true;
        } else {
            cout << "No general beds available!" << endl;
            return false;
        }
    }
};

// Specialized ICU Bed Manager (LSP Applied)
class ICUBedManager : public IBedManager {
protected:
    int icuBeds;
public:
    ICUBedManager(int icuCap) : icuBeds(icuCap) {}

    void showCapacity() const override {
        cout << "Available ICU Beds: " << icuBeds << endl;
    }

    bool allocateBed() override {
        if (icuBeds > 0) {
            icuBeds--;
            return true;
        } else {
            cout << "No ICU beds available!" << endl;
            return false;
        }
    }
};

// Patient Management (LSP Applied)
class Patient {
private:
    string name;
    int age, id;
    IBedManager& bedManager;  // Dependency Injection for LSP
public:
    Patient(IBedManager& manager) : bedManager(manager) {}

    void registerPatient() {
        cout << "Enter Name: "; cin >> name;
        cout << "Enter Age: "; cin >> age;
        cout << "Enter Patient ID: "; cin >> id;

        if (bedManager.allocateBed()) {
            cout << "Patient Registered Successfully!" << endl;
        }
    }
};

// Hospital Services (LSP-Compliant)
class HospitalService : public IService {
private:
    IBedManager& bedManager;
public:
    HospitalService(IBedManager& manager) : bedManager(manager) {}

    void execute() override {
        int subChoice;
        Patient patient(bedManager);
        do {
            cout << "[1] Register Patient" << endl;
            cout << "[2] View Bed Capacity" << endl;
            cout << "[0] Back to Main Menu" << endl;
            cout << "Enter Choice: "; cin >> subChoice;

            if (subChoice == 1) patient.registerPatient();
            else if (subChoice == 2) bedManager.showCapacity();
            else if (subChoice != 0) cout << "Invalid Choice!" << endl;

        } while (subChoice != 0);
    }
};

// Cafe Menu (More Flexible Data Structure Instead of Static Array)
class CafeMenu {
public:
    struct Item {
        string name;
        float price;
    };

    static vector<Item> menu;

    static void displayMenu() {
        for (size_t i = 0; i < menu.size(); i++) {
            cout << "[" << i + 1 << "] " << menu[i].name << " - Rs. " << menu[i].price << endl;
        }
        cout << "[0] Checkout" << endl;
    }
};

// Initialize Cafe Menu Items
vector<CafeMenu::Item> CafeMenu::menu = {
    {"Tea", 450},
    {"Coffee", 550},
    {"Sandwich", 600},
    {"Cookie", 400}
};

// Cafe Ordering System
class CafeOrder {
private:
    float total = 0;
public:
    void placeOrder() {
        int choice;

        do {
            CafeMenu::displayMenu();
            cout << "Select Item: "; cin >> choice;

            if (choice > 0 && choice <= CafeMenu::menu.size()) {
                total += CafeMenu::menu[choice - 1].price;
            } else if (choice != 0) {
                cout << "Invalid Choice!" << endl;
            }

        } while (choice != 0);
        
        cout << "Total Bill: Rs. " << total << endl;
    }
};

// Cafe Service
class CafeService : public IService {
public:
    void execute() override {
        CafeOrder order;
        order.placeOrder();
    }
};

// Main System Controller (Using vector instead of fixed array)
class HospitalManagementSystem {
private:
    vector<IService*> services;
public:
    HospitalManagementSystem(IBedManager& bedManager) {
        services.push_back(new HospitalService(bedManager));
        services.push_back(new CafeService());
    }

    ~HospitalManagementSystem() {
        for (IService* service : services) {
            delete service;
        }
    }

    void run() {
        int choice;
        do {
            WelcomeScreen::display();
            cout << "Enter Your Choice: "; cin >> choice;
            cout << endl;

            if (choice > 0 && choice <= services.size()) {
                services[choice - 1]->execute();
            } else if (choice != 0) {
                cout << "Invalid Option! Try again." << endl;
            }
        } while (choice != 0);

        cout << "Exiting... Thank you!" << endl;
    }
};

// Entry Point
int main() {
    GeneralBedManager generalBeds(500); // Can be replaced with ICUBedManager
    HospitalManagementSystem system(generalBeds);
    system.run();
    return 0;
