#include <sstream>
#include <string>
#include <iomanip>
#include "book.h"
#include "util.h"

Book::Book(const std::string category, const std::string name, double price, int qty,
           const std::string isbn, const std::string author) :
    Product(category, name, price, qty),
    isbn_(isbn),
    author_(author)
{
    std::set<std::string> s1 = parseStringToWords(convToLower(name_));
    std::set<std::string> s2 = parseStringToWords(convToLower(author_));
    bookRelated = setUnion(s1, s2);
    bookRelated.insert(isbn_);
}

Book::~Book()
{

}

std::set<std::string> Book::keywords() const {
    return bookRelated;
}

std::string Book::displayString() const {
    std::stringstream displayStr;
    displayStr << name_ << std::endl;
    displayStr << "Author: " << author_ << " " << "ISBN: " << isbn_ << std::endl;
    displayStr << std::fixed << std::setprecision(2) << price_ << " " << qty_ << " left.";
    return displayStr.str();
}

void Book::dump(std::ostream& os) const {
    Product::dump(os);
    os << isbn_ << "\n" << author_ << std::endl;
}
