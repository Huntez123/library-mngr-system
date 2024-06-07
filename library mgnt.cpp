#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

class Book 
{
private:
    string title;
    string author;
    int id;
    bool available;

public:
    Book(string title, string author, int id) : title(title), author(author), id(id), available(true) {}

    string getTitle() const 
    {
        return title;
    }

    string getAuthor() const 
    {
        return author;
    }

    int getId() const 
    {
        return id;
    }

    bool isAvailable() const 
    {
        return available;
    }

    void setAvailability(bool availability) 
    {
        available = availability;
    }
};

class User 
{
private:
    string username;
    vector<Book*> borrowedBooks;

public:
    User(string username) : username(username) {}

    string getUsername() const 
    {
        return username;
    }

    void borrowBook(Book* book)
    {
        borrowedBooks.push_back(book);
    }

    void returnBook(Book* book) 
    {
        auto it = find(borrowedBooks.begin(), borrowedBooks.end(), book);
        if (it != borrowedBooks.end()) 
        {
            borrowedBooks.erase(it);
        }
    }

    bool hasBorrowed(Book* book) const 
    {
        return find(borrowedBooks.begin(), borrowedBooks.end(), book) != borrowedBooks.end();
    }
};

class Library 
{
private:
    vector<Book> books;
    map<string, string> userCredentials; // Map to store username-password pairs
    vector<User> users;

public:
    void addBook(const Book& book) 
    {
        books.push_back(book);
    }

    void removeBook(int id) 
    {
        books.erase(remove_if(books.begin(), books.end(), [id](Book& book) { return book.getId() == id; }), books.end());
    }

    void addUser(const User& user, string password) 
    {
        users.push_back(user);
        userCredentials[user.getUsername()] = password;
    }

    Book* findBookById(int id) 
    {
        for (auto& book : books) 
        {
            if (book.getId() == id) 
            {
                return &book;
            }
        }
        return nullptr;
    }
    
   

    vector<Book*> findBooksByTitle(string title) 
    {
        vector<Book*> result;
        for (auto& book : books) 
        {
            if (book.getTitle() == title) 
            {
                result.push_back(&book);
            }
        }
        return result;
    }

    bool authenticateUser(string username, string password) 
    {
        auto it = userCredentials.find(username); // Find the username in the map
        if (it != userCredentials.end()) // If username found
        {
            return it->second == password; // Check if the password matches
        }
        return false; // Username not found
    }

    bool borrowBook(string username, int bookId) 
    {
        for (auto& user : users) 
        {
            if (user.getUsername() == username) 
            {
                Book* book = findBookById(bookId);
                if (book && book->isAvailable()) 
                {
                    user.borrowBook(book);
                    book->setAvailability(false);
                    return true;
                }
            }
        }
        return false;
    }

    bool returnBook(string username, int bookId) 
    {
        for (auto& user : users) 
        {
            if (user.getUsername() == username) 
            {
                Book* book = findBookById(bookId);
                if (book && user.hasBorrowed(book)) 
                {
                    user.returnBook(book);
                    book->setAvailability(true);
                    return true;
                }
            }
        }
        return false;
    }
};

void displayMenu() {
    cout << "Library Management System" << endl;
    cout << "1. Add Book" << endl;
    cout << "2. Remove Book" << endl;
    cout << "3. Search Book by ID" << endl;
    
    cout << "4. Search Books by Title" << endl;
    cout << "5. Add User" << endl;
    cout << "6. Borrow Book" << endl;
    cout << "7. Return Book" << endl;
    cout << "8. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    Library library;
    int choice;
    
    do {
        displayMenu();
        cin >> choice;
        cin.ignore();  // to ignore the newline character after the choice input
        switch (choice) {
            case 1: {
                string title, author;
                int id;
                cout << "Enter book title: ";
                cin.ignore(); // Ignoring the newline character left in the buffer
                getline(cin, title);
                cout << "Enter book author: ";
                getline(cin, author);
                cout << "Enter book ID: ";
                cin >> id;
                library.addBook(Book(title, author, id));
                cout << "Book added successfully!" << endl;
                break;
            }
            case 2: {
                int id;
                cout << "Enter book ID to remove: ";
                cin >> id;
                library.removeBook(id);
                cout << "Book removed successfully!" << endl;
                break;
            }
            case 3: {
                int id;
                cout << "Enter book ID to search: ";
                cin >> id;
                Book* book = library.findBookById(id);
                if (book) {
                    cout << "Book found: " << book->getTitle() << " by " << book->getAuthor() << endl;
                } else {
                    cout << "Book not found." << endl;
                }
                break;
            }
            
            case 4: {
                string title;
                cout << "Enter title to search: ";
                cin.ignore();
                getline(cin, title);
                vector<Book*> books = library.findBooksByTitle(title);
                if (!books.empty()) {
                    cout << "Books with title \"" << title << "\":" << endl;
                    for (Book* book : books) {
                        cout << "ID: " << book->getId() << ", Title: " << book->getTitle() << ", Author: " << book->getAuthor() << endl;
                    }
                } else {
                    cout << "No books found with this title." << endl;
                }
                break;
            }
            case 5: {
                string username, password;
                cout << "Enter username: ";
                cin.ignore(); // Ignoring the newline character left in the buffer
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                library.addUser(User(username), password); // Adding a user with username and password
                cout << "User added successfully!" << endl;
                break;
            }
            case 6: {
                string username, password;
                cout << "Enter username: ";
                cin.ignore(); // Ignoring the newline character left in the buffer
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                if (library.authenticateUser(username, password)) // Authenticating the user
                {
                    int bookId;
                    cout << "Enter book ID to borrow: ";
                    cin >> bookId;
                    if (library.borrowBook(username, bookId)) {
                        cout << "Book borrowed successfully!" << endl;
                    } else {
                        cout << "Failed to borrow book. It may not be available or user not found." << endl;
                    }
                }
                else
                {
                    cout << "Invalid username or password. Authentication failed." << endl;
                }
                break;
            }
            case 7: {
                string username, password;
                cout << "Enter username: ";
                cin.ignore(); // Ignoring the newline character left in the buffer
                getline(cin, username);
                cout << "Enter password: ";
                getline(cin, password);
                if (library.authenticateUser(username, password)) // Authenticating the user
                {
                    int bookId;
                    cout << "Enter book ID to return: ";
                    cin >> bookId;
                    if (library.returnBook(username, bookId)) {
                        cout << "Book returned successfully!" << endl;
                    } else {
                        cout << "Failed to return book. It may not be borrowed or user not found." << endl;
                    }
                }
                else
                {
                    cout << "Invalid username or password. Authentication failed." << endl;
                }
                break;
            }
            case 8: {
                cout << "Exiting..." << endl;
                break;
            }
            default: {
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }
    } while (choice != 9);

    return 0;
}