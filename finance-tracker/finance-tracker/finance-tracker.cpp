#include<iostream>
using std::cin;
using std::cout;
using std::endl;

struct Month {
    int id;     
    double income;    
    double expense;  
};
const int MIN_MONTH_COUNT = 1;
const int MAX_MONTH_COUNT = 12;
const int MAX_LENGTH_STR = 50;
const int MIN_INCOME = 0;

const char SETUP[] = "setup";
const char ADD[] =  "add" ;
const char EXIT[] = "exit";

bool strCmp(const char* input, const char* wantedCommand) {
    if (!input) {
        return false;
    }
    int i = 0;
    while (input[i] != '\0' && wantedCommand[i] != '\0')
    {
        if (input[i] != wantedCommand[i])
        {
            return false;
        }
        i++;
    }
    if (input[i] == '\0' && wantedCommand[i] == '\0') {
        return true;
    }
    return false;

}
bool validateMonthsCount(int input)
{
    return input >= MIN_MONTH_COUNT && input <= MAX_MONTH_COUNT;
}

bool validateMonthRange(int month, int currMax) {
    return month >= MIN_MONTH_COUNT && month <= currMax;
}

const char* getMonthName(int id) {
    switch (id) {
    case 1: return "January";
    case 2: return "February";
    case 3: return "March";
    case 4: return "April";
    case 5: return "May";
    case 6: return "June";
    case 7: return "July";
    case 8: return "August";
    case 9: return "September";
    case 10: return "October";
    case 11: return "November";
    case 12: return "December";
    default: return "Unknown";
    }
}
int main()
{
    char command[MAX_LENGTH_STR];
    Month* months = nullptr;
    int monthsCount = 0;
    bool isSetupDone = false;

    while (true) {
        cout << ">";
        cin >> command;
        if (strCmp(command, EXIT)) 
        {
            break;
        }
        else if (strCmp(command, SETUP))
        {
            cout << "Enter number of months: ";
            int inputCount;
            cin >> inputCount;
            if (!validateMonthsCount(inputCount))
            {
                cout << "The profile can not be created! Invalid months count! Must be between 1 and 12." << '\n';
                continue;
            }
            
            if (months != nullptr)
            {
                delete[] months;
            }

            monthsCount = inputCount;
            months = new Month[monthsCount];

            for (int i = 0; i < monthsCount; i++) 
            {
                months[i].id = i + 1;
                months[i].income = 0.0;
                months[i].expense = 0.0;
            }
            isSetupDone = true;
            cout << "Profile created successfully."<<"\n\n";
        }

        else if (strCmp(command, ADD)) {

            if (!isSetupDone)
            {
                cout << "Please run 'setup' first" << '\n';
                continue;
            }

            cout << "Enter month (1-" << monthsCount << "): ";
            int inputMonth;
            cin >> inputMonth;

            if (!(validateMonthRange(inputMonth, monthsCount))) {
                cout << "Invalid month! Please enter a number between 1 and " << monthsCount << endl;
                continue;
            }
            int idx = inputMonth - 1;

            cout << "Enter income: ";
            cin >> months[idx].income;
            if (months[idx].income < MIN_INCOME)
            {
                cout << "Income cannot be negative!" << "\n";
                continue;
            }
            cout << "Enter expense: ";
            cin >> months[idx].expense;

            double balance = months[idx].income - months[idx].expense;
                if (balance > 0)
                {
                   cout << "Result: Balance for " << getMonthName(inputMonth)<< ": +" << balance << endl;
                }
                else 
                {
                   cout << "Result: Balance for month " << inputMonth << " = " << balance << endl;
                }

        }
    }
    delete[] months;
}
