#ifndef LIBRARY_H
#define LIBRARY_H

#include <vector>
#include <string>
#include "Book.h"

using namespace std;

class Library {

private:
    
    vector<Book> books;

public:
    void add_book(const Book& book);
    bool remove_book(const string& title);
    const Book* find_book(const string& title) const;
    vector<Book> get_books_by_author(const string& author) const;
    vector<Book> get_books_by_genre(const string& genre) const;
    vector<Book> get_books_after_year(int minYear) const;
    void sort_by_year(bool ascending = true);
    const Book* find_using_std_find(const string& title) const;
    void print_all_with_iterators() const;
    const vector<Book>& all() const { return books; }
};

#endif // LIBRARY_H
