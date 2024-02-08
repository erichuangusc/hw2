#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include "mydatastore.h"
#include "product.h"
#include "user.h"
#include "util.h"

using namespace std;

MyDataStore::~MyDataStore() { 
    // Delete products
    vector<Product*>::iterator it1;
    for (it1 = productList.begin(); it1 != productList.end(); ++it1) {
      delete *it1;
    }

    // Delete users
    vector<User*>::iterator it2;
    for (it2 = userList.begin(); it2 != userList.end(); ++it2) {
      delete *it2;
    }
}

void MyDataStore::addProduct(Product* p) {
    productList.push_back(p);
    std::set<std::string> pWords = p->keywords();
    std::set<std::string>::iterator it;
    
    for (it = pWords.begin(); it != pWords.end(); ++it) {
      if (keywordToProduct.find(*it) == keywordToProduct.end()) {
        std::set<Product*> temp;
        keywordToProduct.insert(std::pair<string, set<Product*>> (*it, temp));      
      }
      keywordToProduct[*it].insert(p);
    }
}

/**
 * Adds a user to the data store
 */
void MyDataStore::addUser(User* u) {
    userList.push_back(u);
    usernameToUser.insert(std::make_pair(convToLower(u->getName()), u));
    std::vector<Product*> temp;
    cart.insert(std::make_pair(u, temp));
}

/**
 * Performs a search of products whose keywords match the given "terms"
 *  type 0 = AND search (intersection of results for each term) while
 *  type 1 = OR search (union of results for each term)
 */
// std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type) {
//   std::vector<std::set<Product *>> termToProduct;
//   for (std::string term: terms) {
//     // get relevant products
//     termToProduct.push_back(this->keywordToProduct[term]);
//   }
  
//   std::set<Product *> result = termToProduct.back();
//   termToProduct.pop_back();

//   if (type) {
//     for (std::set<Product *> termSet : termToProduct) {
//       result = setUnion(result, termSet);
//     }
//   }
//   else {
//     for (std::set<Product *> termSet : termToProduct) {
//       result = setIntersection(result, termSet);
//     }
//   }

//   std::vector<Product *> answer(result.begin(), result.end());
//   return answer;
// }

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type) {
    vector<Product*> answer;
    map<string, set<Product*>>::iterator it1;
    set<Product*> s1;
    int count = 0;
    bool done = false;

    while (!done) {
   	  it1 = keywordToProduct.find(terms[count]);
   	  if (it1 == keywordToProduct.end()) {
   		  count++;
   	  }
   	  else {
   		  s1 = it1->second;
   		  done = true;
   	  }
    }

    if (type == 0) {
   	  for (unsigned int i = count; i < terms.size(); i++) {
   		  map<string, set<Product*>>::iterator it2 = keywordToProduct.find(terms[i]);

   		  if (it2 != keywordToProduct.end()) {
   			  set<Product*> s2 = it2->second;
   			  s1 = setIntersection(s1, s2);
   		  }
   	  }
    }
    if (type == 1) {
   	  for (unsigned int i = count; i < terms.size(); i++) {
   		  map<string, set<Product*>>::iterator it2 = keywordToProduct.find(terms[i]);

   		  if (it2 != keywordToProduct.end()) {
   			  set<Product*> s2 = it2->second;
   			  s1 = setUnion(s1, s2);
   		  }
   	  }
    }

    set<Product*>::iterator it3;
    for (it3 = s1.begin(); it3 != s1.end(); ++it3) {
   	  answer.push_back(*it3);
    }
    return answer;
}

/**
 * Reproduce the database file from the current Products and User values
 */
void MyDataStore::dump(std::ostream& ofile) {
    ofile << "<products>" << std::endl;
    std::map<std::string, std::set<Product*>>::iterator it1 = keywordToProduct.begin();
    while (it1 != keywordToProduct.end()) {
        for (Product* p : it1->second) {
            p->dump(ofile);
        }
        ++it1;
    }
    ofile << "</products>" << std::endl;
    ofile << "<users>" << std::endl;
    std::map<std::string, User*>::iterator it2 = usernameToUser.begin();
    while (it2 != usernameToUser.end()) {
        it2->second->dump(ofile);
        ++it2;
    }
    ofile << "</users>" << std::endl;
}

/**
  * Add to cart for the user
  */
void MyDataStore::addtoCart(std::string username, Product* p) {
    // check if the username exists
    if (usernameToUser.find(convToLower(username)) == usernameToUser.end()) {
      std::cout << "Invalid request" << std::endl;
      return;
    } 
    if (cart.find(usernameToUser[convToLower(username)]) == cart.end()) {
      vector <Product*> temp;
      cart.insert(make_pair(usernameToUser[convToLower(username)], temp)); 
    }
    cart[usernameToUser[convToLower(username)]].push_back(p);
}

void MyDataStore::viewCart(std::string username) {
    // check if the username exists
    if (usernameToUser.find(convToLower(username)) != usernameToUser.end()) {
      User* currentUser = usernameToUser[convToLower(username)];
      std::vector<Product*> currentUserCart = cart[currentUser];
      for (int i = 0; i < currentUserCart.size(); i++) {
        cout << "Item " << i+1 << endl;
        cout << currentUserCart[i]->displayString();
        cout << endl;
      }
    }
    else{
      cout << "Invalid username" << endl;
    }
}

// void MyDataStore::viewCart(std::string username) {
//     map<string, User*>::iterator it1 = usernameToUser.find(convToLower(username));

//     // check if the username exists
//     if (it1 != usernameToUser.end()) {
// 	    User* currentUser = it1->second;
//       std::map<User*, vector<Product*>>::iterator it2 = cart.find(currentUser);
//       std::vector<Product*>::iterator it3; 
//       int count = 0;
//       for (it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) {
//         std::cout << "Item " << count +1 << "\n" << (*it3)->displayString() << "\n";
//         count++;
//       }
//     }
//     else{ 
//       cout << "Invalid username" << endl;
//     }
// }


void MyDataStore::buyCart(std::string username) {
	  map<string, User*>::iterator it1 = usernameToUser.find(convToLower(username));

    // check if the username exists
    if (it1 != usernameToUser.end()) {
        User* currentUser = it1->second;
        std::map<User*, vector<Product*>>::iterator it2 = cart.find(currentUser);

        std::vector<int> temp;
        int count = 0;
        vector<Product*>::iterator it3;
        for (it3 = it2->second.begin(); it3 != it2->second.end(); ++it3) {
            Product* currentProduct = *it3;
            if ((currentUser->getBalance() >= currentProduct->getPrice()) && (currentProduct->getQty() > 0)) {
                temp.push_back(count);
                currentProduct->subtractQty(1);
                currentUser->deductAmount(currentProduct->getPrice());
                count += 1;
            }
        }
        // delete after looping
        for (int i = temp.size() - 1; i >= 0; --i) {
            it2->second.erase(temp[i] + it2->second.begin());
        }
    }
    else { 
        cout << "Invalid username" << endl;
    }
}

