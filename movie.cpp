#include <sstream>
#include <string>
#include <iomanip>
#include "movie.h"
#include "util.h"

Movie::Movie(const std::string category, const std::string name, double price, int qty,
             const std::string genre, const std::string rating) :
    Product(category, name, price, qty),
    genre_(genre),
    rating_(rating)
{
    movieRelated = parseStringToWords(name_);
    movieRelated.insert(convToLower(genre_));
}

Movie::~Movie()
{

}

std::set<std::string> Movie::keywords() const {
    return movieRelated;
}

std::string Movie::displayString() const {
    std::stringstream displayStr;
    displayStr << name_ << std::endl; 
    displayStr << "Genre: " << genre_ << " " << "Rating: " << rating_ << std::endl;
    displayStr << std::fixed << std::setprecision(2) << price_ << " " << qty_ << " left.";
    return displayStr.str();
}

void Movie::dump(std::ostream& os) const {
    Product::dump(os);
    os << genre_ << "\n" << rating_ << std::endl;
}
