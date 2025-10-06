#include "BankAccount.h"
#include <iostream>
#include <iomanip>

using namespace std;

int BankAccount::totalAccounts = 0;
int BankAccount::nextAccountNumber = 1001;

BankAccount::BankAccount()
    : accountHolder("Unknown"), accountNumber(nextAccountNumber), balance(0.0)
{
    ++totalAccounts;
    ++nextAccountNumber;
    
    cout << "Account created: #" << accountNumber;
    cout << " (" << accountHolder << ")\n";
}

BankAccount::BankAccount(const string& name, double initialBalance)
    : accountHolder(name), accountNumber(nextAccountNumber), balance(0.0)
{
    if (initialBalance > 0.0) {
        balance = initialBalance;
    }
    
    ++totalAccounts;
    ++nextAccountNumber;

    cout << "Account created: #" << accountNumber;
    cout << " (" << accountHolder << "), starting balance $";
    cout << fixed << setprecision(2) << balance << "\n";
}

BankAccount::~BankAccount() {
    
    --totalAccounts;
    
    cout << "Destructor called for account " << accountNumber << "\n";
}

string BankAccount::getAccountHolder() const { return accountHolder; }
int BankAccount::getAccountNumber() const { return accountNumber; }
double BankAccount::getBalance() const { return balance; }

void BankAccount::deposit(double amount) {
    if (amount > 0.0) {
        
        balance += amount;
        cout << accountHolder << " deposited $";
        cout << fixed << setprecision(2) << amount << "\n";

    }
    
    else {
        
        cout << "Deposit must be positive.\n";

    }
}

bool BankAccount::withdraw(double amount) {
    
    if (amount <= 0.0) {
        
        cout << "Withdrawal must be positive.\n";
        
        return false;
    }
    
    if (amount > balance) {
        
        cout << "Insufficient funds for " << accountHolder << ".\n";
        
        return false;
    }
    
    balance -= amount;
    cout << accountHolder << " withdrew $";
    cout << fixed << setprecision(2) << amount << "\n";
    
    return true;
}

bool BankAccount::transfer(BankAccount& toAccount, double amount) {
    
    if (amount <= 0.0) {
        
        cout << "Transfer amount must be positive.\n";
        
        return false;
    }
    
    if (amount > balance) {
        
        cout << "Transfer failed: insufficient funds in ";
        cout << accountHolder << "'s account.\n";
        
        return false;
    }

    balance -= amount;
    toAccount.balance += amount;

    cout << "Transferred $" << fixed << setprecision(2) << amount;
    cout << " from " << accountHolder;
    cout << " (#" << accountNumber << ") to ";
    cout << toAccount.accountHolder;
    cout << " (#" << toAccount.accountNumber << ")\n";
    
    return true;
}

void BankAccount::printAccountInfo() const {
    
    cout << "Account Holder: " << accountHolder << "\n";
    cout << "Account Number: " << accountNumber << "\n";
    cout << "Balance: $" << fixed << setprecision(2) << balance << "\n\n";

}

int BankAccount::getTotalAccounts() {
    
    return totalAccounts;
}
