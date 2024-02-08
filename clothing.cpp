#include <sstream>
#include <string>
#include <iomanip>
#include "clothing.h"
#include "util.h"

Clothing::Clothing(const std::string category, const std::string name, double price, int qty,
                   const std::string size, const std::string brand) :
    Product(category, name, price, qty),
    size_(size),
    brand_(brand)
{
	  std::set<std::string> s1 = parseStringToWords(name_);
	  std::set<std::string> s2 = parseStringToWords(brand_);
    clothingRelated = setUnion(s1, s2);
}

Clothing::~Clothing()
{

}

std::set<std::string> Clothing::keywords() const {
    return clothingRelated;
}

std::string Clothing::displayString() const {
    std::stringstream displayStr;
    displayStr << name_ << std::endl;
    displayStr << "Size: " << size_ << " " << "Brand: " << brand_ << std::endl;
    displayStr << std::fixed << std::setprecision(2) << price_ << " " << qty_ << " left.";
    return displayStr.str();
}

void Clothing::dump(std::ostream& os) const {
    Product::dump(os);
    os << size_ << "\n" << brand_ << std::endl;
}
