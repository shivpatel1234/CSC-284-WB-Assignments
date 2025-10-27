#ifndef BOOK_H
#define BOOK_H

#include <string>
#include <iostream>

using namespace std;

class Book {

private:
    
    string title;
    string author;
    int year;
    string genre;

public:
    
    Book() : title(""), author(""), year(0), genre("") {}
    Book(const string& t, const string& a, int y, const string& g)
        : title(t), author(a), year(y), genre(g) {}

    void display() const {
        
        cout << title << " by " << author << " (" << year << ") [" << genre << "]" << endl;
    }

    const string& getTitle()  const { return title;  }
    const string& getAuthor() const { return author; }
    int getYear()             const { return year;   }
    const string& getGenre()  const { return genre;  }

    void setTitle(const string& v)  { title = v;  }
    void setAuthor(const string& v) { author = v; }
    void setYear(int v)             { year = v;   }
    void setGenre(const string& v)  { genre = v;  }

    bool operator==(const Book& rhs) const { return this->title == rhs.title; }
};

#endif // BOOK_H
