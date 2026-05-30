  
// ╔══════════════════════════════════════════════════════════════════╗
// ║       THE IMPERIAL REGENCY - HOTEL MANAGEMENT SYSTEM            ║
// ║       OOP Semester 2 Project | Islamabad, Pakistan              ║
// ║                                                                  ║
// ║  OOP Concepts : Classes, Inheritance, Polymorphism,             ║
// ║                 Encapsulation, Abstraction, Composition,        ║
// ║                 Virtual Functions, Vectors, File Handling        ║
// ╚══════════════════════════════════════════════════════════════════╝

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <limits>
using namespace std;

// ════════════════════════════════════════════════════════
//  UTILITY HELPERS
// ════════════════════════════════════════════════════════

void printLine(char c = '=', int len = 50)
{
    cout << "  ";
    for (int i = 0; i < len; i++) cout << c;
    cout << "\n";
}

void printTitle(string title)
{
    printLine();
    int pad = (50 - (int)title.length()) / 2;
    cout << "  ";
    for (int i = 0; i < pad; i++) cout << " ";
    cout << title << "\n";
    printLine();
}

int getChoice(int lo, int hi)
{
    int ch;
    while (true)
    {
        cout << "  Choice : ";
        if (cin >> ch && ch >= lo && ch <= hi)
        {
            cin.ignore();
            return ch;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Please enter a number between " << lo << " and " << hi << ".\n";
    }
}

// ════════════════════════════════════════════════════════
//  [VALIDATION ADDED] HELPER FUNCTIONS FOR INPUT
//  Simple, beginner-friendly validation functions.
//  These do NOT change any class or business logic.
// ════════════════════════════════════════════════════════

// Reads a valid integer from cin.
// Rejects letters, symbols, decimals, mixed text.
// Keeps asking until a valid integer is entered.
int getValidInt(const string& prompt)
{
    int val;
    while (true)
    {
        cout << prompt;
        if (cin >> val)
        {
            // [VALIDATION] Check there is no leftover non-whitespace
            // on the line (e.g. "12abc" should be rejected)
            char leftover;
            if (cin.peek() == '\n' || cin.eof())
            {
                cin.ignore();
                return val;
            }
            // There is extra garbage after the number
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Invalid input. Please enter a whole number only.\n";
        }
        else
        {
            // [VALIDATION] cin failed (e.g. user typed letters)
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Invalid input. Please enter a whole number only.\n";
        }
    }
}

// Reads a valid positive integer (greater than zero).
// Used for nights, IDs, room numbers where negative/zero makes no sense.
int getValidPositiveInt(const string& prompt)
{
    int val;
    while (true)
    {
        val = getValidInt(prompt);
        // [VALIDATION] Reject zero and negative numbers
        if (val > 0)
            return val;
        cout << "  Value must be greater than zero. Please try again.\n";
    }
}

// Reads a valid non-negative double (>=0.0).
// Rejects letters and invalid symbols.
// Used for prices and extra charges.
double getValidDouble(const string& prompt)
{
    double val;
    while (true)
    {
        cout << prompt;
        if (cin >> val)
        {
            // [VALIDATION] Check for leftover garbage after the number
            if (cin.peek() == '\n' || cin.eof())
            {
                // [VALIDATION] Reject negative prices/charges
                if (val < 0.0)
                {
                    cin.ignore();
                    cout << "  Amount cannot be negative. Please enter 0 or more.\n";
                    continue;
                }
                cin.ignore();
                return val;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Invalid amount. Please enter a numeric value (e.g. 1500 or 250.50).\n";
        }
        else
        {
            // [VALIDATION] cin failed — user typed non-numeric text
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Invalid amount. Please enter a numeric value (e.g. 1500 or 250.50).\n";
        }
    }
}

// Reads a non-empty string line from cin.
// Rejects blank/empty input and re-prompts.
string getValidString(const string& prompt)
{
    string val;
    while (true)
    {
        cout << prompt;
        getline(cin, val);
        // [VALIDATION] Reject empty input
        if (!val.empty())
            return val;
        cout << "  This field cannot be empty. Please enter a value.\n";
    }
}

// Validates a date string in the format DD/MM/YYYY.
// Checks:
//   - Exactly 10 characters long
//   - Slashes at positions 2 and 5 only
//   - Day, month, year are all numeric
//   - Day between 1-31, Month between 1-12, Year >= 1900
// Returns true if the date is valid, false otherwise.
bool isValidDate(const string& date)
{
    // [VALIDATION] Must be exactly 10 characters: DD/MM/YYYY
    if (date.length() != 10)
        return false;

    // [VALIDATION] Slashes must be at positions 2 and 5
    if (date[2] != '/' || date[5] != '/')
        return false;

    // [VALIDATION] All other positions must be digits
    for (int i = 0; i < 10; i++)
    {
        if (i == 2 || i == 5) continue;  // skip the slash positions
        if (date[i] < '0' || date[i] > '9')
            return false;
    }

    // [VALIDATION] Extract day, month, year and check ranges
    int day   = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year  = (date[6] - '0') * 1000 + (date[7] - '0') * 100
              + (date[8] - '0') * 10   + (date[9] - '0');

    if (day   < 1 || day   > 31) return false;
    if (month < 1 || month > 12) return false;
    if (year  < 1900)            return false;

    return true;
}

// Reads a date string, validates DD/MM/YYYY format, re-prompts if invalid.
string getValidDate(const string& prompt)
{
    string date;
    while (true)
    {
        cout << prompt;
        getline(cin, date);
        // [VALIDATION] Check format and ranges
        if (isValidDate(date))
            return date;
        cout << "  Invalid date. Please use DD/MM/YYYY format (e.g. 25/12/2025).\n";
        cout << "  - Use slashes ( / ) as separators, not dashes or dots.\n";
        cout << "  - Day: 01-31, Month: 01-12, Year: 1900 onwards.\n";
    }
}

// ════════════════════════════════════════════════════════
//  CLASS : Person   (Abstract Base Class)
//  Demonstrates: Abstraction, Encapsulation
// ════════════════════════════════════════════════════════

class Person
{
protected:
    string name;
    string phone;
    string email;

public:
    Person() : name(""), phone(""), email("") {}

    Person(string n, string p, string e)
        : name(n), phone(p), email(e) {}

    // Getters
    string getName()  { return name;  }
    string getPhone() { return phone; }
    string getEmail() { return email; }

    // Setters
    void setName (string n) { name  = n; }
    void setPhone(string p) { phone = p; }
    void setEmail(string e) { email = e; }

    // Pure virtual function — makes Person abstract
    // Every derived class MUST implement this
    virtual void display() = 0;

    virtual ~Person() {}
};

// ════════════════════════════════════════════════════════
//  CLASS : Guest   (Inherits Person)
//  Demonstrates: Inheritance, Function Overriding
// ════════════════════════════════════════════════════════

class Guest : public Person
{
private:
    int    guestID;
    string cnic;
    string nationality;
    bool   isVIP;

public:
    // Default Constructor
    Guest() : Person(), guestID(0), cnic(""), nationality(""), isVIP(false) {}

    // Parameterised Constructor
    Guest(string n, string p, string e, int id, string c, string nat, bool vip)
        : Person(n, p, e), guestID(id), cnic(c), nationality(nat), isVIP(vip) {}

    int    getGuestID()     { return guestID;     }
    string getCnic()        { return cnic;        }
    string getNationality() { return nationality; }
    bool   getIsVIP()       { return isVIP;       }
    void   setVIP(bool v)   { isVIP = v;          }

    // Override pure virtual — Polymorphism
    void display() override
    {
        cout << "  Name        : " << name        << "\n";
        cout << "  Phone       : " << phone       << "\n";
        cout << "  Email       : " << email       << "\n";
        cout << "  Guest ID    : " << guestID     << "\n";
        cout << "  CNIC/Pass   : " << cnic        << "\n";
        cout << "  Nationality : " << nationality << "\n";
        cout << "  VIP Status  : " << (isVIP ? "*** YES ***" : "No") << "\n";
    }
};

// ════════════════════════════════════════════════════════
//  CLASS : Staff   (Inherits Person)
//  Demonstrates: Inheritance, Virtual Functions
// ════════════════════════════════════════════════════════

class Staff : public Person
{
protected:
    int    staffID;
    string role;
    string shift;

public:
    Staff() : Person(), staffID(0), role(""), shift("") {}

    Staff(string n, string p, string e, int id, string r, string sh)
        : Person(n, p, e), staffID(id), role(r), shift(sh) {}

    int    getStaffID() { return staffID; }
    string getRole()    { return role;    }
    string getShift()   { return shift;   }

    // Virtual — can be overridden by Manager
    void display() override
    {
        cout << "  Name        : " << name    << "\n";
        cout << "  Staff ID    : " << staffID << "\n";
        cout << "  Role        : " << role    << "\n";
        cout << "  Shift       : " << shift   << "\n";
    }

    virtual ~Staff() {}
};

// ════════════════════════════════════════════════════════
//  CLASS : Manager   (Inherits Staff)
//  Demonstrates: Multi-level Inheritance, Overriding
// ════════════════════════════════════════════════════════

class Manager : public Staff
{
private:
    string username;
    string password;    // stored as plain text at semester-2 level

public:
    Manager() {}

    Manager(string n, string p, string e,
            int id, string sh, string u, string pass)
        : Staff(n, p, e, id, "General Manager", sh)
    {
        username = u;
        password = pass;
    }

    bool login(string u, string pass)
    {
        return (username == u && password == pass);
    }

    // Override — Polymorphism in action
    void display() override
    {
        Staff::display();
        cout << "  Username    : " << username << "\n";
    }
};

// ════════════════════════════════════════════════════════
//  ABSTRACT CLASS : Room   (Base for all room types)
//  Demonstrates: Abstraction (pure virtual), Encapsulation
// ════════════════════════════════════════════════════════

class Room
{
protected:
    int    roomNumber;
    double pricePerNight;
    int    capacity;
    string status;       // Available / Occupied / Maintenance
    string view;         // City / Garden / Mountain
    int    floorNumber;
    string bookedBy;

public:
    Room() : roomNumber(0), pricePerNight(0), capacity(0),
             status("Available"), view("City"), floorNumber(0), bookedBy("") {}

    Room(int rn, double p, int cap, string v, int floor)
        : roomNumber(rn), pricePerNight(p), capacity(cap),
          status("Available"), view(v), floorNumber(floor), bookedBy("") {}

    // Getters
    int    getRoomNumber()  { return roomNumber;   }
    double getPrice()       { return pricePerNight; }
    int    getCapacity()    { return capacity;      }
    string getStatus()      { return status;        }
    string getView()        { return view;          }
    int    getFloor()       { return floorNumber;   }
    string getBookedBy()    { return bookedBy;      }
    bool   isAvailable()    { return status == "Available"; }

    // Setters
    void setStatus  (string s) { status   = s; }
    void setPrice   (double p) { pricePerNight = p; }
    void setBookedBy(string g) { bookedBy = g; }

    void checkOut()
    {
        status   = "Available";
        bookedBy = "";
    }

    // Pure virtual — every room type must implement
    virtual string getRoomType()  = 0;
    virtual string getAmenities() = 0;

    // Virtual display — can be overridden
    virtual void display()
    {
        cout << "  Room No     : " << roomNumber       << "\n";
        cout << "  Type        : " << getRoomType()    << "\n";
        cout << "  Floor       : " << floorNumber      << "\n";
        cout << "  Price/Night : Rs. " << pricePerNight << "\n";
        cout << "  Capacity    : " << capacity << " guests\n";
        cout << "  View        : " << view              << "\n";
        cout << "  Status      : " << status            << "\n";
        cout << "  Amenities   : " << getAmenities()    << "\n";
        if (!bookedBy.empty())
            cout << "  Booked By   : " << bookedBy     << "\n";
    }

    virtual ~Room() {}
};

// ════════════════════════════════════════════════════════
//  ROOM SUBCLASSES   (Inheritance + Polymorphism)
// ════════════════════════════════════════════════════════

class StandardRoom : public Room
{
public:
    StandardRoom() {}

    StandardRoom(int rn, double p, int cap, string v, int floor)
        : Room(rn, p, cap, v, floor) {}

    string getRoomType()  override { return "Standard";                   }
    string getAmenities() override { return "Queen Bed, TV, WiFi, AC";   }

    void display() override
    {
        cout << "\n  -- Standard Room --\n";
        Room::display();
    }
};

class DeluxeRoom : public Room
{
public:
    DeluxeRoom() {}

    DeluxeRoom(int rn, double p, int cap, string v, int floor)
        : Room(rn, p, cap, v, floor) {}

    string getRoomType()  override { return "Deluxe";                              }
    string getAmenities() override { return "King Bed, Minibar, Bathtub, WiFi, AC"; }

    void display() override
    {
        cout << "\n  -- Deluxe Room --\n";
        Room::display();
    }
};

class SuiteRoom : public Room
{
private:
    bool hasJacuzzi;

public:
    SuiteRoom() : hasJacuzzi(false) {}

    SuiteRoom(int rn, double p, int cap, string v, int floor, bool jac)
        : Room(rn, p, cap, v, floor), hasJacuzzi(jac) {}

    string getRoomType()  override { return "Suite";                                          }
    string getAmenities() override { return "King Bed, Living Room, Kitchen, Minibar, WiFi"; }

    void display() override
    {
        cout << "\n  -- Suite Room --\n";
        Room::display();
        cout << "  Jacuzzi     : " << (hasJacuzzi ? "Yes" : "No") << "\n";
    }
};

// ════════════════════════════════════════════════════════
//  CLASS : Booking
//  Demonstrates: Composition (has-a Guest, links-to Room)
// ════════════════════════════════════════════════════════

class Booking
{
private:
    int    bookingID;
    Guest  guest;        // Composition: Booking HAS-A Guest
    Room*  room;         // Association: Booking points to a Room
    int    nights;
    double roomTotal;
    double extraCharges; // food / services added during stay
    bool   isCheckedIn;
    string checkInDate;
    string checkOutDate;

public:
    Booking()
        : bookingID(0), room(nullptr), nights(0),
          roomTotal(0), extraCharges(0), isCheckedIn(false) {}

    Booking(int id, Guest g, Room* r, int n, string ci, string co)
        : bookingID(id), guest(g), room(r), nights(n),
          extraCharges(0), isCheckedIn(false),
          checkInDate(ci), checkOutDate(co)
    {
        roomTotal = r->getPrice() * nights;
    }

    // Getters
    int    getBookingID()   { return bookingID;   }
    int    getNights()      { return nights;       }
    double getRoomTotal()   { return roomTotal;    }
    double getExtraCharges(){ return extraCharges; }
    bool   getIsCheckedIn() { return isCheckedIn;  }
    string getGuestName()   { return guest.getName(); }
    string getCheckIn()     { return checkInDate;  }
    string getCheckOut()    { return checkOutDate; }
    Guest& getGuest()       { return guest;        }

    int getRoomNumber()
    {
        if (room != nullptr) return room->getRoomNumber();
        return -1;
    }

    string getRoomType()
    {
        if (room != nullptr) return room->getRoomType();
        return "N/A";
    }

    void addExtraCharge(double amt) { extraCharges += amt; }

    void confirmBooking()
    {
        if (room != nullptr && room->isAvailable())
        {
            room->setStatus("Occupied");
            room->setBookedBy(guest.getName());
            isCheckedIn = true;
            cout << "\n  Booking confirmed! ID : " << bookingID << "\n";
            cout << "  Room Total : Rs. " << roomTotal << "\n";
        }
        else
        {
            cout << "\n  Room is not available.\n";
        }
    }

    void cancelBooking()
    {
        if (room != nullptr && isCheckedIn)
        {
            room->checkOut();
            isCheckedIn = false;
            cout << "\n  Booking #" << bookingID << " cancelled.\n";
        }
        else
        {
            cout << "\n  No active booking to cancel.\n";
        }
    }

    void display()
    {
        printLine('-', 50);
        cout << "  Booking ID  : " << bookingID  << "\n";
        cout << "  Status      : " << (isCheckedIn ? "Checked In" : "Cancelled/Inactive") << "\n";
        cout << "  Check-In    : " << checkInDate  << "\n";
        cout << "  Check-Out   : " << checkOutDate << "\n";
        cout << "  Nights      : " << nights        << "\n";
        cout << "  Room Type   : " << getRoomType() << "\n";
        cout << "  Room No     : " << getRoomNumber()<< "\n";
        cout << "  Room Total  : Rs. " << roomTotal  << "\n";
        cout << "  Extra Chgs  : Rs. " << extraCharges << "\n";
        cout << "\n  -- Guest Details --\n";
        guest.display();
    }
};

// ════════════════════════════════════════════════════════
//  CLASS : Invoice
//  Demonstrates: Simple billing composition
// ════════════════════════════════════════════════════════

class Invoice
{
private:
    int    invoiceID;
    int    bookingID;
    string guestName;
    double roomCharges;
    double extraCharges;
    double tax;          // 16% GST
    double totalAmount;
    string paymentMethod;

public:
    Invoice() : invoiceID(0), bookingID(0), roomCharges(0),
                extraCharges(0), tax(0), totalAmount(0) {}

    Invoice(int id, int bid, string gn, double room, double extra, string method)
        : invoiceID(id), bookingID(bid), guestName(gn),
          roomCharges(room), extraCharges(extra), paymentMethod(method)
    {
        double subtotal = roomCharges + extraCharges;
        tax         = subtotal * 0.16;
        totalAmount = subtotal + tax;
    }

    int    getInvoiceID()    { return invoiceID;   }
    double getTotalAmount()  { return totalAmount; }
    string getGuestName()    { return guestName;   }

    void display()
    {
        double subtotal = roomCharges + extraCharges;
        printTitle("THE IMPERIAL REGENCY");
        cout << "       Five Star Luxury | Islamabad\n";
        printLine('-', 50);
        cout << "  Invoice No  : " << invoiceID      << "\n";
        cout << "  Booking ID  : " << bookingID      << "\n";
        cout << "  Guest       : " << guestName      << "\n";
        printLine('-', 50);
        cout << "  Room Charges     : Rs. " << setw(8) << roomCharges  << "\n";
        cout << "  Extra Charges    : Rs. " << setw(8) << extraCharges << "\n";
        printLine('-', 50);
        cout << "  Subtotal         : Rs. " << setw(8) << subtotal     << "\n";
        cout << "  GST (16%)        : Rs. " << setw(8) << tax          << "\n";
        printLine('=', 50);
        cout << "  TOTAL AMOUNT     : Rs. " << setw(8) << totalAmount  << "\n";
        printLine('=', 50);
        cout << "  Payment Method   : " << paymentMethod << "\n";
        cout << "  Status           : ** PAID **\n";
        printLine('-', 50);
        cout << "  Thank you for staying at The Imperial Regency.\n";
        printLine();
    }
};

// ════════════════════════════════════════════════════════
//  CLASS : HotelSystem   (Main Controller)
//  Demonstrates: Composition, Vectors, File Handling
// ════════════════════════════════════════════════════════

class HotelSystem
{
private:
    vector<Room*>    rooms;
    vector<Booking>  bookings;
    vector<Guest>    guests;
    vector<Invoice>  invoices;

    Manager manager;
    bool    managerLoggedIn;

    int nextBookingID;
    int nextInvoiceID;
    int nextGuestID;

    // ── File names ─────────────────────────────────────
    const string GUEST_FILE   = "guests.txt";
    const string BOOKING_FILE = "bookings.txt";
    const string INVOICE_FILE = "invoices.txt";

    // ── File Handling ──────────────────────────────────

    void saveGuests()
    {
        ofstream f(GUEST_FILE);
        for (int i = 0; i < (int)guests.size(); i++)
        {
            f << guests[i].getGuestID()   << "|"
              << guests[i].getName()       << "|"
              << guests[i].getPhone()      << "|"
              << guests[i].getEmail()      << "|"
              << guests[i].getCnic()       << "|"
              << guests[i].getNationality()<< "|"
              << guests[i].getIsVIP()      << "\n";
        }
        f.close();
        cout << "  Guests saved to " << GUEST_FILE << "\n";
    }

    void loadGuests()
    {
        ifstream f(GUEST_FILE);
        if (!f.is_open()) return;
        string line;
        while (getline(f, line))
        {
            if (line.empty()) continue;
            // Parse: id|name|phone|email|cnic|nationality|vip
            vector<string> parts;
            string token;
            for (int i = 0; i < (int)line.size(); i++)
            {
                if (line[i] == '|') { parts.push_back(token); token = ""; }
                else                  token += line[i];
            }
            parts.push_back(token);
            if (parts.size() < 7) continue;

            // [VALIDATION] Safe stoi: skip lines with non-numeric ID
            bool idIsNumeric = true;
            for (int i = 0; i < (int)parts[0].size(); i++)
            {
                if (parts[0][i] < '0' || parts[0][i] > '9')
                { idIsNumeric = false; break; }
            }
            if (!idIsNumeric) continue;

            int  id  = stoi(parts[0]);
            bool vip = (parts[6] == "1");
            Guest g(parts[1], parts[2], parts[3], id, parts[4], parts[5], vip);
            guests.push_back(g);
            if (id >= nextGuestID) nextGuestID = id + 1;
        }
        f.close();
    }

    void saveBookings()
    {
        ofstream f(BOOKING_FILE);
        for (int i = 0; i < (int)bookings.size(); i++)
        {
            f << bookings[i].getBookingID()  << "|"
              << bookings[i].getGuestName()  << "|"
              << bookings[i].getRoomNumber() << "|"
              << bookings[i].getNights()     << "|"
              << bookings[i].getRoomTotal()  << "|"
              << bookings[i].getCheckIn()    << "|"
              << bookings[i].getCheckOut()   << "|"
              << bookings[i].getIsCheckedIn()<< "\n";
        }
        f.close();
        cout << "  Bookings saved to " << BOOKING_FILE << "\n";
    }

    void saveInvoices()
    {
        ofstream f(INVOICE_FILE);
        for (int i = 0; i < (int)invoices.size(); i++)
        {
            f << invoices[i].getInvoiceID()   << "|"
              << invoices[i].getGuestName()    << "|"
              << invoices[i].getTotalAmount()  << "\n";
        }
        f.close();
        cout << "  Invoices saved to " << INVOICE_FILE << "\n";
    }

    // ── Room Helpers ───────────────────────────────────

    Room* findRoom(int roomNo)
    {
        for (int i = 0; i < (int)rooms.size(); i++)
            if (rooms[i]->getRoomNumber() == roomNo) return rooms[i];
        return nullptr;
    }

    Booking* findBooking(int id)
    {
        for (int i = 0; i < (int)bookings.size(); i++)
            if (bookings[i].getBookingID() == id) return &bookings[i];
        return nullptr;
    }

    // ── Feature Functions ──────────────────────────────

    void addGuest()
    {
        printTitle("REGISTER GUEST");
        string name, phone, email, cnic, nat;

        // [VALIDATION] All fields require non-empty string input
        name  = getValidString("  Name        : ");
        phone = getValidString("  Phone       : ");
        email = getValidString("  Email       : ");
        cnic  = getValidString("  CNIC/Pass   : ");
        nat   = getValidString("  Nationality : ");

        Guest g(name, phone, email, nextGuestID, cnic, nat, false);
        guests.push_back(g);
        cout << "\n  Guest registered. ID: " << nextGuestID << "\n";
        nextGuestID++;
    }

    void viewGuests()
    {
        printTitle("ALL GUESTS");
        if (guests.empty()) { cout << "  No guests registered.\n"; return; }
        for (int i = 0; i < (int)guests.size(); i++)
        {
            guests[i].display();
            printLine('-', 50);
        }
    }

    void searchGuest()
    {
        printTitle("SEARCH GUEST");
        // [VALIDATION] Reject empty search query
        string query = getValidString("  Enter name or ID to search: ");

        bool found = false;
        for (int i = 0; i < (int)guests.size(); i++)
        {
            string id = to_string(guests[i].getGuestID());
            if (guests[i].getName().find(query) != string::npos || id == query)
            {
                guests[i].display();
                printLine('-', 50);
                found = true;
            }
        }
        if (!found) cout << "  No guest found matching: " << query << "\n";
    }

    void viewRooms(bool availableOnly = false)
    {
        printTitle(availableOnly ? "AVAILABLE ROOMS" : "ALL ROOMS");
        bool any = false;
        for (int i = 0; i < (int)rooms.size(); i++)
        {
            if (availableOnly && !rooms[i]->isAvailable()) continue;
            rooms[i]->display();   // Polymorphic call!
            any = true;
        }
        if (!any) cout << "  No rooms to display.\n";
    }

    void bookRoom()
    {
        printTitle("BOOK A ROOM");
        viewRooms(true);

        // [VALIDATION] Room number must be a valid integer
        int rn = getValidInt("\n  Enter Room Number : ");
        Room* room = findRoom(rn);

        if (room == nullptr)     { cout << "  Room not found.\n";         return; }
        if (!room->isAvailable()){ cout << "  Room is not available.\n";  return; }

        // [VALIDATION] All guest fields require non-empty input
        string name  = getValidString("  Guest Name  : ");
        string phone = getValidString("  Phone       : ");
        string email = getValidString("  Email       : ");
        string cnic  = getValidString("  CNIC/Pass   : ");
        string nat   = getValidString("  Nationality : ");

        // [VALIDATION] Nights must be a positive integer (at least 1 night)
        int nights = getValidPositiveInt("  Nights      : ");

        // [VALIDATION] Dates must be in DD/MM/YYYY format with valid values
        string ci = getValidDate("  Check-In  (DD/MM/YYYY): ");
        string co = getValidDate("  Check-Out (DD/MM/YYYY): ");

        Guest g(name, phone, email, nextGuestID++, cnic, nat, false);
        Booking b(nextBookingID, g, room, nights, ci, co);
        b.confirmBooking();
        bookings.push_back(b);
        nextBookingID++;
    }

    void checkIn()
    {
        printTitle("CHECK-IN");

        // [VALIDATION] Booking ID must be a valid integer
        int id = getValidInt("  Enter Booking ID : ");

        Booking* b = findBooking(id);
        if (b == nullptr)
        {
            cout << "  Booking not found.\n";
            return;
        }
        if (b->getIsCheckedIn())
        {
            cout << "  Guest is already checked in.\n";
            return;
        }
        b->confirmBooking();
        cout << "  Check-in complete for " << b->getGuestName() << ".\n";
    }

    void checkOut()
    {
        printTitle("CHECK-OUT & GENERATE BILL");

        // [VALIDATION] Booking ID must be a valid integer
        int id = getValidInt("  Enter Booking ID : ");

        Booking* b = findBooking(id);
        if (b == nullptr)      { cout << "  Booking not found.\n";         return; }
        if (!b->getIsCheckedIn()){ cout << "  Guest is not checked in.\n"; return; }

        // [VALIDATION] Extra charges must be a valid non-negative number
        double extra = getValidDouble("  Extra charges (room service, etc.) Rs.: ");
        b->addExtraCharge(extra);

        // [VALIDATION] Payment method must be non-empty
        string method = getValidString("  Payment method (Cash/Card): ");

        // Generate invoice
        Invoice inv(nextInvoiceID++, b->getBookingID(),
                    b->getGuestName(), b->getRoomTotal(),
                    b->getExtraCharges(), method);
        invoices.push_back(inv);
        inv.display();

        b->cancelBooking();   // frees the room
    }

    void viewBookings()
    {
        printTitle("ALL BOOKINGS");
        if (bookings.empty()) { cout << "  No bookings yet.\n"; return; }
        for (int i = 0; i < (int)bookings.size(); i++)
            bookings[i].display();
    }

    void viewInvoices()
    {
        printTitle("ALL INVOICES");
        if (invoices.empty()) { cout << "  No invoices yet.\n"; return; }
        for (int i = 0; i < (int)invoices.size(); i++)
            invoices[i].display();
    }

    void viewStatistics()
    {
        printTitle("HOTEL STATISTICS");
        int total = rooms.size(), available = 0, occupied = 0;
        for (int i = 0; i < (int)rooms.size(); i++)
        {
            if (rooms[i]->isAvailable()) available++;
            else                          occupied++;
        }

        double revenue = 0;
        for (int i = 0; i < (int)invoices.size(); i++)
            revenue += invoices[i].getTotalAmount();

        cout << "  Total Rooms     : " << total                   << "\n";
        cout << "  Available       : " << available               << "\n";
        cout << "  Occupied        : " << occupied                << "\n";
        cout << "  Total Guests    : " << guests.size()           << "\n";
        cout << "  Total Bookings  : " << bookings.size()         << "\n";
        cout << "  Total Revenue   : Rs. " << fixed << setprecision(2)
             << revenue                                            << "\n";
    }

    void markVIP()
    {
        printTitle("MARK GUEST AS VIP");

        // [VALIDATION] Guest ID must be a valid integer
        int id = getValidInt("  Enter Guest ID : ");

        for (int i = 0; i < (int)guests.size(); i++)
        {
            if (guests[i].getGuestID() == id)
            {
                guests[i].setVIP(true);
                cout << "  " << guests[i].getName() << " is now a VIP guest.\n";
                return;
            }
        }
        cout << "  Guest ID not found.\n";
    }

    void updateRoomPrice()
    {
        printTitle("UPDATE ROOM PRICE");

        // [VALIDATION] Room number must be a valid integer
        int rn = getValidInt("  Enter Room Number : ");
        Room* room = findRoom(rn);
        if (room == nullptr) { cout << "  Room not found.\n"; return; }

        cout << "  Current Price : Rs. " << room->getPrice() << "\n";

        // [VALIDATION] New price must be a valid non-negative number
        double p = getValidDouble("  New Price     : Rs. ");
        room->setPrice(p);
        cout << "  Price updated.\n";
    }

    void setRoomStatus()
    {
        printTitle("SET ROOM STATUS");

        // [VALIDATION] Room number must be a valid integer
        int rn = getValidInt("  Enter Room Number : ");
        Room* room = findRoom(rn);
        if (room == nullptr) { cout << "  Room not found.\n"; return; }

        cout << "  Current Status : " << room->getStatus() << "\n";

        // [VALIDATION] Status must be non-empty
        string s = getValidString("  New Status (Available/Maintenance/Cleaning): ");
        room->setStatus(s);
        cout << "  Status updated.\n";
    }

    // ── Menus ──────────────────────────────────────────

    bool managerLogin()
    {
        printTitle("MANAGER LOGIN");
        // [VALIDATION] Username and password must be non-empty
        string u = getValidString("  Username : ");
        string p = getValidString("  Password : ");

        if (manager.login(u, p))
        {
            managerLoggedIn = true;
            cout << "\n  Login successful. Welcome, Manager!\n";
            return true;
        }
        cout << "\n  Invalid credentials.\n";
        return false;
    }

    void guestMenu()
    {
        int ch;
        do
        {
            printLine('=', 40);
            cout << "         GUEST PORTAL\n";
            printLine('=', 40);
            cout << "  1.  View All Rooms\n";
            cout << "  2.  View Available Rooms\n";
            cout << "  3.  Book a Room\n";
            cout << "  4.  Check-In\n";
            cout << "  5.  Check-Out & Pay\n";
            cout << "  6.  Register My Profile\n";
            cout << "  0.  Back\n";
            printLine('-', 40);
            ch = getChoice(0, 6);

            switch (ch)
            {
            case 1: viewRooms(false);  break;
            case 2: viewRooms(true);   break;
            case 3: bookRoom();        break;
            case 4: checkIn();         break;
            case 5: checkOut();        break;
            case 6: addGuest();        break;
            case 0: break;
            }
        } while (ch != 0);
    }

    void managerMenu()
    {
        if (!managerLoggedIn)
        {
            if (!managerLogin()) return;
        }

        int ch;
        do
        {
            printLine('=', 40);
            cout << "       MANAGER PORTAL\n";
            printLine('=', 40);
            cout << "  1.  View All Rooms\n";
            cout << "  2.  View All Bookings\n";
            cout << "  3.  View All Guests\n";
            cout << "  4.  Search Guest\n";
            cout << "  5.  View All Invoices\n";
            cout << "  6.  Hotel Statistics\n";
            cout << "  7.  Mark Guest as VIP\n";
            cout << "  8.  Update Room Price\n";
            cout << "  9.  Set Room Status\n";
            cout << "  0.  Logout\n";
            printLine('-', 40);
            ch = getChoice(0, 9);

            switch (ch)
            {
            case 1: viewRooms(false);  break;
            case 2: viewBookings();    break;
            case 3: viewGuests();      break;
            case 4: searchGuest();     break;
            case 5: viewInvoices();    break;
            case 6: viewStatistics();  break;
            case 7: markVIP();         break;
            case 8: updateRoomPrice(); break;
            case 9: setRoomStatus();   break;
            case 0:
                managerLoggedIn = false;
                cout << "\n  Logged out successfully.\n";
                break;
            }
        } while (ch != 0);
    }

public:
    // Constructor — initialise defaults and create manager account
    HotelSystem()
        : managerLoggedIn(false),
          nextBookingID(1001),
          nextInvoiceID(2001),
          nextGuestID(3001)
    {
        manager = Manager("Ahmad Raza", "0300-1234567", "manager@imperialregency.com",
                          1, "Morning", "manager", "hotel123");
        loadGuests();
    }

    void addRoom(Room* r) { rooms.push_back(r); }

    void printBanner()
    {
        printLine('*', 50);
        cout << "      THE IMPERIAL REGENCY\n";
        cout << "   Five Star Luxury | Islamabad, Pakistan\n";
        printLine('*', 50);
        cout << "\n";
    }

    void mainMenu()
    {
        printBanner();
        int ch;
        do
        {
            printLine('=', 40);
            cout << "           MAIN MENU\n";
            printLine('=', 40);
            cout << "  1.  Guest Portal\n";
            cout << "  2.  Manager Portal\n";
            cout << "  3.  Exit\n";
            printLine('-', 40);
            ch = getChoice(1, 3);

            switch (ch)
            {
            case 1: guestMenu();   break;
            case 2: managerMenu(); break;
            case 3:
                saveGuests();
                saveBookings();
                saveInvoices();
                printBanner();
                cout << "  All data saved. Goodbye!\n\n";
                break;
            }
        } while (ch != 3);
    }

    ~HotelSystem()
    {
        for (int i = 0; i < (int)rooms.size(); i++)
            delete rooms[i];
    }
};

// ════════════════════════════════════════════════════════
//  MAIN FUNCTION
// ════════════════════════════════════════════════════════

int main()
{
    HotelSystem hotel;

    // Floor 1 — Standard Rooms
    hotel.addRoom(new StandardRoom(101, 5000, 2, "City",     1));
    hotel.addRoom(new StandardRoom(102, 5000, 2, "Garden",   1));
    hotel.addRoom(new StandardRoom(103, 5500, 2, "City",     1));

    // Floor 2 — Deluxe Rooms
    hotel.addRoom(new DeluxeRoom(201, 9000, 3, "City",     2));
    hotel.addRoom(new DeluxeRoom(202, 9500, 3, "Mountain", 2));
    hotel.addRoom(new DeluxeRoom(203, 9000, 2, "Garden",   2));

    // Floor 3 — Suites
    hotel.addRoom(new SuiteRoom(301, 22000, 4, "Mountain", 3, true));
    hotel.addRoom(new SuiteRoom(302, 18000, 4, "City",     3, false));

    hotel.mainMenu();

    return 0;
}
