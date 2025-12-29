#include<iostream>
#include<iomanip>
using std::cin;
using std::cout;
using std::endl;
using std::setw;
using std::setprecision;
using std::fixed;

struct Month {
    int id;     
    double income;    
    double expense;  
    double balance;
    bool isFilled;
};
const int MIN_MONTH_COUNT = 1;
const int MAX_MONTH_COUNT = 12;
const int MAX_LENGTH_STR = 50;
const int MIN_INCOME_EXPENSE = 0;

const char SETUP[] = "setup";
const char ADD[] =  "add" ;
const char EXIT[] = "exit";
const char REPORT[] = "report";
const char SEARCH[] = "search";

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

bool isMonthFilled(const Month month) {
    return month.isFilled;
}

bool validateIncomeExpense(double sum)
{
    return sum >= MIN_INCOME_EXPENSE;
}
void executeSetup(Month*& months, int& monthsCount, bool& isSetupDone) {
    cout << "Enter number of months: ";
    int inputCount;
    cin >> inputCount;

    if (!validateMonthsCount(inputCount)) {
        cout << "The profile can not be created! Invalid months count! Must be between 1 and 12." << '\n';
        return;
    }

    if (months != nullptr) {
        delete[] months;
    }

    monthsCount = inputCount;
    months = new Month[monthsCount];

    for (int i = 0; i < monthsCount; i++) {
        months[i].id = i + 1;
        months[i].income = 0.0;
        months[i].expense = 0.0;
        months[i].balance = 0.0;
        months[i].isFilled = false;
    }

    isSetupDone = true;
    cout << "Profile created successfully." << "\n\n";
}

bool shouldOverwrite(int monthId) {
    char choice;
    cout << "Data for " << getMonthName(monthId) << " already exists! Overwrite? (y/n): ";
    cin >> choice;

    if (choice != 'y') {
        cout << "Operation cancelled." << '\n';
        return false;
    }
    return true;
}

bool processMonthEntry(Month* months, int idx, int displayMonthId) {
    cout << "Enter income: ";
    cin >> months[idx].income;

    if (!validateIncomeExpense(months[idx].income)) {
        cout << "Income cannot be negative!" << "\n";
        return false;
    }

    cout << "Enter expense: ";
    cin >> months[idx].expense;

    if (!validateIncomeExpense(months[idx].expense)) {
        cout << "Expense cannot be negative" << '\n';
        return false;
    }

    months[idx].isFilled = true;
    months[idx].balance = months[idx].income - months[idx].expense;

    if (months[idx].balance > 0) {
        cout << "Result: Balance for " << getMonthName(displayMonthId) << ": +" << months[idx].balance << endl;
    }
    else {
        cout << "Result: Balance for month " << getMonthName(displayMonthId) << " = " << months[idx].balance << endl;
    }

    return true;
}

void executeAdd(Month* months, int monthsCount, bool isSetupDone) {
    if (!isSetupDone) {
        cout << "Please run 'setup' first" << '\n';
        return; 
    }

    cout << "Enter month (1-" << monthsCount << "): ";
    int inputMonth;
    cin >> inputMonth;

    if (!(validateMonthRange(inputMonth, monthsCount))) {
        cout << "Invalid month! Please enter a number between 1 and " << monthsCount << endl;
        return;
    }

    int idx = inputMonth - 1;

    if (isMonthFilled(months[idx])) {
        if (!shouldOverwrite(inputMonth)) {
            return;
        } 
    }

    if (!processMonthEntry(months, idx, inputMonth))
    {
        return;
    }

    cout << '\n';
}

void printMonthsInfo(const Month* months, int idx) {
    int currMonthCount = idx + 1;
        cout << setw(10) << getMonthName(currMonthCount)
            << " | " << setw(10) << months[idx].income
            << " | " << setw(10) << months[idx].expense
            << " | " << setw(10) << months[idx].balance << '\n';
}

void printReportSummary(double totalIncome, double totalExpense, int count) {

    double averageBalance = (count > 0) ? (totalIncome - totalExpense) / count : 0.0;

    cout << "-------------------------------------" << '\n';
    cout << "Total income:    " << totalIncome << '\n';
    cout << "Total expense:   " << totalExpense << '\n';
    cout << "Average balance: " << averageBalance << '\n';
    cout << "\n\n";
}

void executeReport(const Month* months, int monthsCount, bool isSetupDone) {
    if (!isSetupDone || monthsCount == 0) {
        cout << "Please run 'setup' first to generate report." << '\n';
        return;
    }

    double totalIncome = 0;
    double totalExpense = 0;

    cout << setw(10) << "Month" << setw(10) << "Income" << setw(10) << "Expense" << setw(10) << "Balance" << '\n';
    cout << "--------------------------------------------------" << '\n';
    cout << fixed << setprecision(2);

    for (int i = 0; i < monthsCount; i++)
    {
        totalIncome += months[i].income;
        totalExpense += months[i].expense;

        printMonthsInfo(months, i);
    }

    printReportSummary(totalIncome, totalExpense, monthsCount);
}

int getMonthIdByName(const char* monthName)
{
    int monthsInAYear = 12;
    for (int i = 1; i <= monthsInAYear; i++)
    {
        if (strCmp(monthName, getMonthName(i)))
        {
            return i;
        }
    }
    return 0;
}

void printMonthInfo(const Month* months, int idx)
{
    double percentageExpense;
    if (months[idx].income == 0)
    {
        percentageExpense = 100;
    }
    else
    {
        percentageExpense = (months[idx].expense / months[idx].income) * 100;
    }
    cout << fixed << setprecision(2);
    cout << "Income: " << months[idx].income << '\n';
    cout << "Expense: " << months[idx].expense << '\n';
    cout << "Balance: " << months[idx].balance << '\n';
    cout << "Expense ratio: " << setprecision(4) << percentageExpense << '\n';

}
void executeSearch(const Month* months, int monthsCount, bool isSetupDone)
{
   
    if (!isSetupDone || monthsCount==0)
    {
        cout << "Please run 'setup first and add data first' " << '\n';
        return;
    }

    char wantedMonth[MAX_LENGTH_STR];
    cin >> wantedMonth;

    int monthID = getMonthIdByName(wantedMonth);

    if (monthID == 0)
    {
        cout << "Invalid month name!"<<'\n';
        return;
    }

    if (!validateMonthRange(monthID, monthsCount))
    {
        cout<<"Month "<<wantedMonth<<" is outside of the configured range (1 - " << monthsCount << ")." << '\n';
        return;
    }
   
    int monthIdx = monthID - 1;

    if (!months[monthIdx].isFilled)
    {
        cout << "No data found for " << getMonthName(monthID) << '\n';
    }
    else
    {
        printMonthInfo(months, monthIdx);
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
            executeSetup(months, monthsCount, isSetupDone);
        }

        else if (strCmp(command, ADD))
        {
            executeAdd(months, monthsCount, isSetupDone);
        }
        
        else if (strCmp(command, REPORT))
        {
            executeReport(months, monthsCount, isSetupDone);
        }
        
        else if (strCmp(command, SEARCH))
        {
            executeSearch(months, monthsCount, isSetupDone);
        }

    }
    delete[] months;
}
