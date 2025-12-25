#include<iostream>
using std::cin;
using std::cout;
using std::endl;

const int MIN_MONTH_COUNT = 1;
const int MAX_MONTH_COUNT = 12;
const int MAX_LENGTH_STR = 50;
const int MIN_INCOME = 0;

const char SETUP[] = {"setup"};
const char ADD[] = { "add" };

void clearCommand(char *command) {
    if (command) {
        *command = '\0';
    }
}
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

bool validateMonthRange(int month) {
    return month >= MIN_MONTH_COUNT && month <= MAX_MONTH_COUNT;
}

int main()
{
    char command[MAX_LENGTH_STR];

    cout << ">";
    cin >> command;
    if (!strCmp(command, SETUP)) {
        return -1;
    }
    clearCommand(command);

    cout << "Enter number of months: ";

    int monthsCount;
    cin >> monthsCount;

    if (!validateMonthsCount(monthsCount)) 
    {
        cout << "The profile can not be created! Invalid months count!" << '\n';
        return -5;
    }
    cout << "Profile created successfully." << '\n';
    cout << "\n\n";

    cout << "> " << '\n';
    cin >> command;
    if (!strCmp(command, ADD)) {
        return -1;
    }
    clearCommand(command);

    cout << "Enter month (1-"<<monthsCount<<"): ";
    int month;
    cin >> month;
    if (!validateMonthRange(month))
    {
        cout << "Invalid month! The month should be in range from 1 to " << monthsCount<< '\n';
        return -5;
    }

    

}
