#include "User.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;



User::User(){}
User::User(string id, string pin, double initialAmount, vector<string> _FriendlyAccounts) {
    ID = id;
    PIN = pin;
    amount = initialAmount;
    friendlyAccounts = _FriendlyAccounts;
}
User::~User(){}

string User::getID() const {
    return ID;
}
string User::getPIN() const {
    return PIN;
}
double User::getAmount() const {
    return amount;
}
const vector<string>& User::getFriendlyAccounts() const {
    return friendlyAccounts;
}

void User::addFriendlyAccount(const string& accID) {
    friendlyAccounts.push_back(accID);
}

void User::deposit(double money) {
    amount += money;
}
bool User::withdraw(double money) {
    if (amount >= money) {
        amount -= money;
        return true;
    }
    return false;
}
bool User::transfer(double money, User& recipient) {
    if (amount >= money) {
        amount -= money;
        return true;
    }
    return false;
}


