#include <iostream>
#include <vector>
#include <string>
#include <limits> // For numeric_limits

using namespace std;

// -------------------- Utility Functions --------------------
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getValidatedInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            clearInputBuffer();
            cout << "Invalid input. Please enter a number.\n";
        } else {
            clearInputBuffer();
            return value;
        }
    }
}

double getValidatedDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            clearInputBuffer();
            cout << "Invalid input. Please enter a number.\n";
        } else {
            clearInputBuffer();
            return value;
        }
    }
}

// -------------------- Abstraction + Polymorphism --------------------
class LibraryItem {
protected:
    int id;
    string title;
public:
    LibraryItem(int id, string title) : id(id), title(title) {}
    virtual void display() const = 0;
    virtual ~LibraryItem() {}
    int getId() const { return id; }
    string getTitle() const { return title; }
};

// -------------------- Book class --------------------
class Book : public LibraryItem {
private:
    string author;
    bool isAvailable;
public:
    Book(int id, string title, string author)
        : LibraryItem(id, title), author(author), isAvailable(true) {}

    void display() const override {
        cout << "Book ID: " << getId() << " | Title: " << getTitle()
             << " | Author: " << author
             << " | Status: " << (isAvailable ? "Available" : "Checked Out") << endl;
    }

    bool checkAvailability() const { return isAvailable; }
    void setAvailability(bool status) { isAvailable = status; }
    string getAuthor() const { return author; }
};

// -------------------- Person base class --------------------
class Person {
protected:
    int id;
    string name;
    string email;
public:
    Person(int id, string name, string email)
        : id(id), name(name), email(email) {}

    virtual void display() const {
        cout << "ID: " << id << " | Name: " << name << " | Email: " << email << endl;
    }

    int getId() const { return id; }
    string getName() const { return name; }
    string getEmail() const { return email; }
};

// -------------------- Member class --------------------
class Member : public Person {
private:
    vector<Book*> borrowedBooks;
    double totalFines;
    static const int MAX_BOOKS = 5;

    double calculateFine(int daysLate) const {
        return daysLate * 0.50;
    }

public:
    Member(int id, string name, string email)
        : Person(id, name, email), totalFines(0) {}

    void display() const override {
        Person::display();
        cout << "Borrowed Books: " << borrowedBooks.size()
             << " | Total Fines: $" << totalFines << endl;
    }

    bool borrowBook(Book* book) {
        if (borrowedBooks.size() >= MAX_BOOKS) {
            cout << "Maximum book limit reached!" << endl;
            return false;
        }
        if (!book->checkAvailability()) {
            cout << "Book is not available!" << endl;
            return false;
        }
        borrowedBooks.push_back(book);
        book->setAvailability(false);
        cout << "Book borrowed successfully!" << endl;
        return true;
    }

    bool returnBook(Book* book, int daysLate = 0) {
        for (size_t i = 0; i < borrowedBooks.size(); ++i) {
            if (borrowedBooks[i]->getId() == book->getId()) {
                borrowedBooks.erase(borrowedBooks.begin() + i);
                book->setAvailability(true);
                if (daysLate > 0) {
                    double fine = calculateFine(daysLate);
                    totalFines += fine;
                    cout << "Late return! Fine: $" << fine << endl;
                }
                cout << "Book returned successfully!" << endl;
                return true;
            }
        }
        cout << "You didn't borrow this book!" << endl;
        return false;
    }

    void payFine(double amount) {
        if (amount <= 0) {
            cout << "Invalid amount!" << endl;
            return;
        }
        if (amount >= totalFines) {
            cout << "Paying more than owed. Change: $" << (amount - totalFines) << endl;
            totalFines = 0;
        } else {
            totalFines -= amount;
        }
        cout << "Payment processed. Remaining fines: $" << totalFines << endl;
    }

    double getTotalFines() const { return totalFines; }
    const vector<Book*>& getBorrowedBooks() const { return borrowedBooks; }
};

// -------------------- Library class --------------------
class Library {
private:
    vector<Book> books;
    vector<Member> members;

public:
    void addBook() {
        int id = getValidatedInt("Enter Book ID: ");
        string title, author;
        cout << "Enter Book Title: "; 
        getline(cin, title);
        cout << "Enter Author: "; 
        getline(cin, author);
        books.emplace_back(id, title, author);
        cout << "Book added successfully!" << endl;
    }

    void addMember() {
        int id = getValidatedInt("Enter Member ID: ");
        string name, email;
        cout << "Enter Name: "; 
        getline(cin, name);
        cout << "Enter Email: "; 
        getline(cin, email);
        members.emplace_back(id, name, email);
        cout << "Member registered successfully!" << endl;
    }

    void displayAllBooks() const {
        if (books.empty()) cout << "No books in the library!" << endl;
        else {
            cout << "\n--- Library Books ---" << endl;
            for (const auto& book : books) book.display();
        }
    }

    void displayAllMembers() const {
        if (members.empty()) cout << "No members registered!" << endl;
        else {
            cout << "\n--- Library Members ---" << endl;
            for (const auto& member : members) member.display();
        }
    }

    Book* findBook(int id) {
        for (auto& book : books)
            if (book.getId() == id) return &book;
        return nullptr;
    }

    Member* findMember(int id) {
        for (auto& member : members)
            if (member.getId() == id) return &member;
        return nullptr;
    }

    void processBorrow() {
        int memberId = getValidatedInt("Enter Member ID: ");
        int bookId = getValidatedInt("Enter Book ID: ");
        Member* m = findMember(memberId);
        Book* b = findBook(bookId);
        if (m && b) m->borrowBook(b);
        else cout << "Member or book not found!" << endl;
    }

    void processReturn() {
        int memberId = getValidatedInt("Enter Member ID: ");
        int bookId = getValidatedInt("Enter Book ID: ");
        int daysLate = getValidatedInt("Days late (0 if on time): ");
        Member* m = findMember(memberId);
        Book* b = findBook(bookId);
        if (m && b) {
            m->returnBook(b, daysLate);
        } else cout << "Member or book not found!" << endl;
    }

    void processFinePayment() {
        int memberId = getValidatedInt("Enter Member ID: ");
        double amount = getValidatedDouble("Enter payment amount: $");
        Member* m = findMember(memberId);
        if (m) {
            m->payFine(amount);
        } else cout << "Member not found!" << endl;
    }

    void run() {
        while (true) {
            cout << "\n=== Library Management System ===\n"
                 << "1. Add Book\n2. Add Member\n3. Display All Books\n4. Display All Members\n"
                 << "5. Borrow Book\n6. Return Book\n7. Pay Fines\n8. Exit\n"
                 << "Enter your choice: ";
            
            int choice;
            cin >> choice;
            if (cin.fail()) {
                cin.clear();
                clearInputBuffer();
                cout << "Invalid choice! Please enter a number between 1-8.\n";
                continue;
            }
            clearInputBuffer();
            
            switch (choice) {
                case 1: addBook(); break;
                case 2: addMember(); break;
                case 3: displayAllBooks(); break;
                case 4: displayAllMembers(); break;
                case 5: processBorrow(); break;
                case 6: processReturn(); break;
                case 7: processFinePayment(); break;
                case 8: cout << "Exiting system. Goodbye!" << endl; return;
                default: cout << "Invalid choice! Please enter a number between 1-8.\n";
            }
        }
    }
};

int main() {
    Library library;
    library.run();
    return 0;
}