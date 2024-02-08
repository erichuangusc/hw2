#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include <string>
#include <set>
#include <vector>
#include <map>
#include <queue>
#include "datastore.h"
#include "product.h"
#include "user.h"

class MyDataStore : public DataStore {
public:
    MyDataStore() { }

    virtual ~MyDataStore();

    /**
     * Adds a product to the data store
     */
    virtual void addProduct(Product* p);

    /**
     * Adds a user to the data store
     */
    virtual void addUser(User* u);

    /**
     * Performs a search of products whose keywords match the given "terms"
     *  type 0 = AND search (intersection of results for each term) while
     *  type 1 = OR search (union of results for each term)
     */
    virtual std::vector<Product*> search(std::vector<std::string>& terms, int type);

    /**
     * Reproduce the database file from the current Products and User values
     */
    virtual void dump(std::ostream& ofile);

    void addtoCart(std::string username, Product* product);

    void viewCart(std::string username);
    
    void buyCart(std::string username);

private:
    std::vector<User*> userList;
    std::vector<Product*> productList;
    std::map<std::string, std::set<Product *>> keywordToProduct;
    std::map<std::string, User*> usernameToUser;
    std::map<User*, std::vector<Product *>> cart;
};
#endif