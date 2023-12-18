#pragma once
#include "User.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;


class ATM {
public:
    bool fileExists(const string& filename);
    bool login(User& user);
    void createNewAccount();

    void displayAccountInfo(const User& user);
    void withdrawMoney(User& user);
    void depositMoney(User& user);
    void transferMoney(User& user, User& recipient);

    void performActions(User& user); // Function user
    void startATM(); // Menu

};
