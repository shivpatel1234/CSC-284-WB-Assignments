#include <iostream>
#include <string>
#include "BankAccount.h"

using namespace std;

int main() {
    
    cout << "Bank Account Managment Program \n\n";

    string name1, name2;
    double start1 = 0.0;
    double start2 = 0.0;

    cout << "Enter account 1 holder name: ";
    getline(cin, name1);
   
    cout << "Enter initial balance for account 1: ";
    cin >> start1;
    cin.ignore(1000, '\n');

    cout << "Enter account 2 holder name: ";
    getline(cin, name2);
    
    cout << "Enter initial balance for account 2: ";
    cin >> start2;
    cin.ignore(1000, '\n');

    BankAccount account1(name1, start1);
    BankAccount account2(name2, start2);

    cout << "\nInitial account info:\n";
    
    account1.printAccountInfo();
    account2.printAccountInfo();

    cout << "Total active accounts: " << BankAccount::getTotalAccounts() << "\n\n";

    int choice = -1;
    
    while (choice != 0) {
        
        cout << "Choose an action:\n";
        cout << " 1) Deposit into account 1\n";
        cout << " 2) Deposit into account 2\n";
        cout << " 3) Withdraw from account 1\n";
        cout << " 4) Withdraw from account 2\n";
        cout << " 5) Transfer from account 1 to account 2\n";
        cout << " 6) Transfer from account 2 to account 1\n";
        cout << " 7) Show both accounts\n";
        cout << " 8) Show total active accounts\n";
        cout << " 0) Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (!cin) {
            
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n\n";
            
            choice = -1;
            continue;
        }

        double amt = 0.0;
        
        switch (choice) {
        
        case 1:
            cout << "Amount to deposit into account 1: ";
            cin >> amt;
            account1.deposit(amt);
            
            break;
        
        case 2:
            cout << "Amount to deposit into account 2: ";
            cin >> amt;
            account2.deposit(amt);
            
            break;
        
        case 3:
            
            cout << "Amount to withdraw from account 1: ";
            cin >> amt;
            account1.withdraw(amt);
            
            break;
        
        case 4:
            
            cout << "Amount to withdraw from account 2: ";
            cin >> amt;
            account2.withdraw(amt);
            
            break;
        
        case 5:
            
            cout << "Amount to transfer (account 1 -> account 2): ";
            cin >> amt;
            account1.transfer(account2, amt);
            
            break;
        
        case 6:
            
            cout << "Amount to transfer (account 2 -> account 1): ";
            cin >> amt;
            account2.transfer(account1, amt);
            
            break;
        
        case 7:
            
            cout << "\nAccount summaries:\n";
            account1.printAccountInfo();
            account2.printAccountInfo();
            
            break;
        
        case 8:
            
            cout << "Total active accounts: " << BankAccount::getTotalAccounts() << "\n";
            
            break;
        
        case 0:
            cout << "Exiting program...\n";
            
            break;
        
        default:
            
            cout << "Unknown choice.\n";
            
            break;
        }
        
        cout << "\n";
    }

    return 0;
}

