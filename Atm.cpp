#include "Atm.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;



string generateRandomID() {
    srand(static_cast<unsigned int>(time(NULL)));
    string randomIDchar = "";
    randomIDchar.push_back(char(65+ (rand() % 26)));  // char(65+ (rand() % 26))
    randomIDchar.push_back(char(65+ (rand() % 26)));
    string randomID1 = to_string(1000 + rand() % 1000);
    string randomID2 = to_string(1000 + rand() % 1000);

    return randomIDchar + randomID1 + randomID2;
}

bool ATM::fileExists(const string& filename) {
    ifstream file(filename.c_str());
    return file.good();
}

void ATM::createNewAccount() {
    string id = generateRandomID();
    string pin;
    cout << "Your new ID is: " << id << endl;
    cout << "Set your PIN (6 digits): ";
    cin >> pin;
    while (pin.size() != 6 || pin[0] < '0' || pin[0] > '9' || pin[1] < '0' || pin[1] > '9' ||
                              pin[2] < '0' || pin[2] > '9' || pin[3] < '0' || pin[3] > '9' ||
           pin[4] < '0' || pin[4] > '9' || pin[5] < '0' || pin[5] > '9' ){
        cout << "Pin must be 6 digits" << endl;
        cout << "Set your PIN (6 digits): ";
        cin >> pin;
    }
    ofstream outFile(id + ".txt");
    outFile << pin << " " << "0.0" << endl;
    outFile.close();
    cout << "Account created successfully!" << endl;
}

bool ATM::login(User& user) {
    string id;
    string pin;
    cout << "Enter ID: ";
    cin >> id;
    cout << "Enter PIN: ";
    cin >> pin;

    string filename = id + ".txt";
    if (!fileExists(filename)) {
        cout << "Invalid ID or PIN." << endl;
        return false;
    }

    ifstream inFile(filename);
        string storedPIN;
        double amount;
        vector <string> _FriendlyAccount;
        inFile >> storedPIN >> amount;
        string FriendAcc;
        while (inFile >> FriendAcc){
            _FriendlyAccount.push_back(FriendAcc);
        }

    if (storedPIN != pin) {
        cout << "Invalid ID or PIN." << endl;
        return false;
    }
    user = User(id, pin, amount,_FriendlyAccount);
    return true;
}

void ATM::displayAccountInfo(const User& user) {
    cout << "Account ID: " << user.getID() << endl;
    cout << "Account Balance: " << user.getAmount() << endl;
    cout << "Friendly Accounts: " << endl;
    const vector<string>& friendlyAccounts = user.getFriendlyAccounts();
    if (friendlyAccounts.size() != 0){
        for (int i = 0; i < friendlyAccounts.size(); i++) {
            cout << friendlyAccounts[i] <<endl;
        }
    }
    else cout << "NO FRIENDLY ACCOUNT";
    cout << endl;
    
}

void ATM::withdrawMoney(User& user) {
    double withdrawAmount;
    const vector<string>& friendlyAccounts = user.getFriendlyAccounts();
    cout << "Select amount to withdraw:" << endl;
    cout << "1. 10" << endl;
    cout << "2. 20" << endl;
    cout << "3. 50" << endl;
    cout << "4. 100" << endl;
    cout << "5. Other" << endl;
    int choice;
    cin >> choice;
    switch (choice) {
        case 1: withdrawAmount = 10; break;
        case 2: withdrawAmount = 20; break;
        case 3: withdrawAmount = 50; break;
        case 4: withdrawAmount = 100; break;
        case 5:
            cout << "Enter amount to withdraw: ";
            cin >> withdrawAmount;
            while (withdrawAmount <= 0)
            {
                cout << "Withdraw amount must be positive" << endl;
                cout << "Enter the amount to withdraw: ";
                cin >> withdrawAmount;
                break;
            default:
                cout << "Invalid choice." << endl;
                return;
            }
            
            if (user.withdraw(withdrawAmount)) {
                ofstream outFile(user.getID() + ".txt");
                outFile << user.getPIN() << " " << user.getAmount() << endl;
                for (int i = 0; i < friendlyAccounts.size(); i++)
                {
                    outFile << friendlyAccounts[i] << endl;
                }
                outFile.close();
                cout << "Withdrawal successful! New balance: " << user.getAmount() << endl;
            }
            else {
                cout << "Not Enough Money!" << endl;
            }
    }
}

void ATM::depositMoney(User& user){
    double depositAmount;
    const vector<string>& friendlyAccounts = user.getFriendlyAccounts();
    cout << "Enter the amount to deposit: ";
    cin >> depositAmount;
        
    user.deposit(depositAmount);
    ofstream outFile(user.getID() + ".txt");
    outFile << user.getPIN() << " " << user.getAmount() << endl;
    for (int i = 0; i < friendlyAccounts.size(); i++){
        outFile << friendlyAccounts[i] << endl;
    }
    outFile.close();
    cout << "Deposit successful! New balance: " << user.getAmount() << endl;
}
    
void ATM::transferMoney(User& user, User& recipient) {
    string recipientID;
    double transferAmount;
    int optRecipient = 1;
    const vector<string>& friendlyAccounts = user.getFriendlyAccounts();
    if (friendlyAccounts.size() != 0){
        cout << "Your Friendly Accounts List" << endl;
        for (int i = 0; i < friendlyAccounts.size(); i++) {
            cout << i+1 << ". " << friendlyAccounts[i] << endl;
        }
        cout << friendlyAccounts.size() + 1 << ". Other" << endl;
        cout << "Choose your option: ";
        cin >> optRecipient;
        if (optRecipient == friendlyAccounts.size() + 1){
            cout << "Enter recipient's ID: ";
            cin >> recipientID;
        }
        else{
            recipientID = friendlyAccounts[optRecipient - 1];
        }
    }
    else {
        cout << "Enter recipient's ID: ";
        cin >> recipientID;
    }
    if (recipientID != user.getID()) {
        if (fileExists(recipientID + ".txt")) {
            if (optRecipient == friendlyAccounts.size() + 1){
                cout <<"Do you want to save?" << endl;
                cout <<"1. YES" << endl;
                cout <<"0. NO" << endl;
                int optsave;
                cout << "Choose your option: ";
                cin >> optsave;
                if (optsave == 1){
                    user.addFriendlyAccount(recipientID);
                    cout <<"Additon Successful!" << endl;
                }
            }
            cout << "Enter amount to transfer: ";
            cin >> transferAmount;
            if (user.transfer(transferAmount, recipient)) {
                ofstream outFile(user.getID() + ".txt");
                outFile << user.getPIN() << " " << user.getAmount() << endl;
                for (int i = 0; i < friendlyAccounts.size(); i++){
                    outFile << friendlyAccounts[i] << endl;
                }
                outFile.close();
                string filename = recipientID + ".txt";
                ifstream inFile(filename);
                string storedPIN;
                double _amount;
                vector <string> _FriendlyAccounts;
                inFile >> storedPIN >> _amount;
                string FriendAcc;
                while (inFile >> FriendAcc){
                    _FriendlyAccounts.push_back(FriendAcc);
                }
                _amount += transferAmount;
                ofstream recipientFile(recipientID + ".txt");
                recipientFile << storedPIN << " " << _amount << endl;
                for (int i = 0; i < _FriendlyAccounts.size(); i++){
                    recipientFile << _FriendlyAccounts[i] << endl;
                }
                recipientFile.close();
                cout << "Transfer successful!" << endl;
                cout << "New balance: " <<user.getAmount()<<endl;
                return;
            }
        }
        else {
            cout << "Recipient ID not found." << endl;
        }
    }
    else {
        cout << "Cannot transfer to yourself." << endl;
    }
    cout << "Not Enough Money or Invalid Recipient ID!" << endl;
}
    
void ATM::performActions(User& user) {
    int choice;
    vector<string> _FriendlyAccounts;
    User recipient("", "000000", 0.0,_FriendlyAccounts);
    while (true) {
        cout << "Logged in successfully! Choose an option:" << endl;
        cout << "1. Account Information" << endl;
        cout << "2. Withdraw money" << endl;
        cout << "3. Deposit money" << endl;
        cout << "4. Transfer money" << endl;
        cout << "5. Logout" << endl;
        cin >> choice;
        switch (choice) {
            case 1:
                displayAccountInfo(user);
                break;
            case 2:
                withdrawMoney(user);
                break;
            case 3:
                depositMoney(user);
                break;
            case 4:
                transferMoney(user, recipient);
                break;
            case 5:
                cout << "Logging out..." << endl;
                return;
            default:
                cout << "Invalid option. Try again." << endl;
        }
    }
}

void ATM::startATM() {
    int option;
    vector<string> _FriendlyAccounts;
    User currentUser("", "000000", 0.0, _FriendlyAccounts);
    while (true) {
        cout << "Main Menu" << endl;
        cout << "1. Login" << endl;
        cout << "2. Create new account" << endl;
        cout << "3. Exit" << endl;
        cin >> option;
        switch (option) {
            case 1:
                if (login(currentUser)) {
                    performActions(currentUser);
                }
                break;
            case 2:
                createNewAccount();
                break;
            case 3:
                cout << "Exiting..." << endl;
                return;
            default:
                cout << "Invalid option. Try again." << endl;
        }
    }
}
