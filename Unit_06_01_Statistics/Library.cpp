#include "Library.h"
#include <algorithm>
#include <iostream>
#include <ranges>

using namespace std;

void Library::add_book(const Book& book) {
    books.push_back(book);
}

bool Library::remove_book(const string& title) {
    size_t before = books.size();
    books.erase(remove_if(books.begin(), books.end(),
                          [&](const Book& b){ return b.getTitle() == title; }),
                books.end());
    
    return books.size() != before;
}

const Book* Library::find_book(const string& title) const {
    
    for (vector<Book>::const_iterator it = books.cbegin(); it != books.cend(); ++it) {
        
        if (it->getTitle() == title) {
            
            return &(*it);
        }
    }
    cout << "Error: book titled \"" << title << "\" not found.\n";
    
    return nullptr;
}

vector<Book> Library::get_books_by_author(const string& author) const {
    vector<Book> result;
    
    for (vector<Book>::const_iterator it = books.cbegin(); it != books.cend(); ++it) {
        
        if (it->getAuthor() == author) result.push_back(*it);
    }
    
    return result;
}

vector<Book> Library::get_books_by_genre(const string& genre) const {
    
    vector<Book> result;
    
    auto view = books | views::filter([&](const Book& b){ return b.getGenre() == genre; });
    
    for (const Book& b : view) result.push_back(b);
    
    return result;
}

vector<Book> Library::get_books_after_year(int minYear) const {
    
    vector<Book> result;
    
    auto view = books | views::filter([&](const Book& b){ return b.getYear() > minYear; });
    
    for (const Book& b : view) result.push_back(b);
    
    return result;
}

void Library::sort_by_year(bool ascending) {
    
    if (ascending) {
        sort(books.begin(), books.end(), [](const Book& a, const Book& b){
            
            return a.getYear() < b.getYear();
        });
    } 
    else {
        sort(books.begin(), books.end(), [](const Book& a, const Book& b){
            
            return a.getYear() > b.getYear();
        });
    }
}

const Book* Library::find_using_std_find(const string& title) const {
    
    Book key(title, "", 0, "");
    
    vector<Book>::const_iterator it = find(books.cbegin(), books.cend(), key);
    
    if (it != books.cend()) return &(*it);
    
    return nullptr;
}

void Library::print_all_with_iterators() const {
    
    cout << "--- Library Books (iterator traversal) ---" << endl;
    
    for (vector<Book>::const_iterator it = books.cbegin(); it != books.cend(); ++it) {
        it->display();
    }
}
