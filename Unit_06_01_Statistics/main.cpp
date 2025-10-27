#include <iostream>
#include <vector>
#include <string>
#include <ranges>
#include <algorithm>
#include <iterator>
#include "Book.h"
#include "Library.h"

using namespace std;

static void pause_and_wait() {
    
    cout << "Press ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

static Book read_book_from_user() {
    
    string title, author, genre;
    int year = 0;

    cout << "Enter title: ";
    getline(cin, title);

    cout << "Enter author: ";
    getline(cin, author);

    cout << "Enter year (Ex:2001): ";
    
    while (!(cin >> year)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a valid integer year: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter genre: ";
    getline(cin, genre);

    return Book(title, author, year, genre);
}

static void print_menu() {
    
    cout << "\n****** Library Menu ******\n";
    cout << "1) Add a book\n";
    cout << "2) Remove a book by title\n";
    cout << "3) Find a book by title (iterator search)\n";
    cout << "4) List ALL books (iterator traversal)\n";
    cout << "5) List books by AUTHOR (iterator-based search)\n";
    cout << "6) List books by GENRE (ranges filter + sort by year)\n";
    cout << "7) List books AFTER a YEAR (ranges filter)\n";
    cout << "8) Sort by year ASCENDING\n";
    cout << "9) Sort by year DESCENDING\n";
    cout << "10) Find via std::find (by title)\n";
    cout << "11) Ranges pipeline: Programming books >= 2000 (sort + transform)\n";
    cout << "0) Exit\n";
    cout << "Users choice: ";
}

int main() {
    
    Library lib;
    
    cout << "Welcome to the Library Manager (user will add books).\n";

    int choice = -1;
    do {
        print_menu();
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            Book b = read_book_from_user();
            lib.add_book(b);
            cout << "Added: ";
            b.display();
        }
        else if (choice == 2) {
            
            string title;
            
            cout << "Title to remove: ";
            getline(cin, title);
            bool removed = lib.remove_book(title);
            cout << (removed ? "Removed." : "No match found.") << endl;
        }
        else if (choice == 3) {
            
            string title;
            
            cout << "Title to find: ";
            getline(cin, title);
            const Book* found = lib.find_book(title);
            
            if (found) {
                cout << "Found: ";
                found->display();
            }
        }
        else if (choice == 4) {
            lib.print_all_with_iterators();
        }
        
        else if (choice == 5) {
            
            string author;
            
            cout << "Author: ";
            getline(cin, author);
            vector<Book> v = lib.get_books_by_author(author);
            
            if (v.empty()) cout << "No books by that author.\n";
            
            for (auto it = v.cbegin(); it != v.cend(); ++it) it->display();
        }
        else if (choice == 6) {
            
            string genre;
            
            cout << "Genre: ";
            getline(cin, genre);
            vector<Book> v = lib.get_books_by_genre(genre);
            sort(v.begin(), v.end(), [](const Book& a, const Book& b) { return a.getYear() < b.getYear(); });
            
            if (v.empty()) cout << "No books in that genre.\n";
            
            for (const Book& b : v) b.display();
        }
        else if (choice == 7) {
            
            int y = 0;
            
            cout << "After which year? ";
            
            while (!(cin >> y)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter a valid integer year: ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            vector<Book> v = lib.get_books_after_year(y);
            
            if (v.empty()) cout << "No books after that year.\n";
            
            for (const Book& b : v) b.display();
        }
        else if (choice == 8) {
            
            lib.sort_by_year(true);
            cout << "Sorted ascending.\n";
            lib.print_all_with_iterators();
        }
        else if (choice == 9) {
            
            lib.sort_by_year(false);
            cout << "Sorted descending.\n";
            lib.print_all_with_iterators();
        }
        else if (choice == 10) {
            
            string title;
            
            cout << "Title to find: ";
            getline(cin, title);
            const Book* p = lib.find_using_std_find(title);
            
            if (p) { cout << "Found Title: "; p->display(); }
            
            else { cout << "Not found.\n"; }
        }
        else if (choice == 11) {
            
            // Full ranges pipeline using sort + transform
            auto progView = lib.all() |
                views::filter([](const Book& v) { return v.getGenre() == "Programming"; });

            vector<Book> programming(progView.begin(), progView.end());

            // Sort with ranges
            ranges::sort(programming, [](const Book& a, const Book& b) {
                return a.getYear() < b.getYear();
                };

            // Filter by year >= 2000, then transform to string
            auto after2000 = programming |
                views::filter([](const Book& v) { return v.getYear() >= 2000; });

            vector<string> lines;
            ranges::transform(after2000, back_inserter(lines),
                [](const Book& b) {
                    return b.getTitle() + " by " + b.getAuthor() +
                        " (" + to_string(b.getYear()) + ") [" + b.getGenre() + "]";
                });

            cout << "Programming (year >= 2000) sorted by year (ranges + transform):\n";
            
            for (const string& s : lines) cout << s << endl;
        }
        else if (choice == 0) {
            
            cout << "Goodbye.\n";
        }
        else {
            
            cout << "Unknown choice.\n";
        }

    } while (choice != 0);

    return 0;
}
