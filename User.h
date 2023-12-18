#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class User {
private:
    string ID;
    string PIN;
    double amount;
    vector<string> friendlyAccounts;

public:
    User();
    User(string id, string pin, double initialAmount, vector<string> FriendlyAccount);
    ~User();

    string getID() const;
    string getPIN() const;
    double getAmount() const;
    const vector<string>& getFriendlyAccounts() const;

    void addFriendlyAccount(const string& accID);
    void deposit(double money);
    bool withdraw(double money);
    bool transfer(double money, User& recipient);
    
};
