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

enum CommandType {
    CMD_EXIT = 0,
    CMD_SETUP,
    CMD_ADD,
    CMD_REPORT,
    CMD_SEARCH,
    CMD_SORT,
    CMD_FORECAST,
    CMD_CHART,
    CMD_UNKNOWN
};

const int COMMANDS_COUNT = 8;
const char* COMMAND_NAMES[COMMANDS_COUNT] = {
    "exit",
    "setup",
    "add",
    "report",
    "search",
    "sort",
    "forecast",
    "chart"
};

const int MIN_MONTH_COUNT = 1;
const int MAX_MONTH_COUNT = 12;
const int MAX_LENGTH_STR = 50;
const int MIN_INCOME_EXPENSE = 0;

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

int getCommandId(const char* input) {
    for (int i = 0; i < COMMANDS_COUNT; i++) {
        if (strCmp(input, COMMAND_NAMES[i])) {
            return i;
        }
    }
    return CMD_UNKNOWN;
}

bool validateMonthsCount(int input)
{
    return input >= MIN_MONTH_COUNT && input <= MAX_MONTH_COUNT;
}

bool validateMonthRange(int month, int currMax) {
    return month >= MIN_MONTH_COUNT && month <= currMax;
}

bool isSetupDoneAndMonthsC(bool isSetupDone, int monthsCount=1)
{
    if (!isSetupDone || monthsCount == 0)
    {
        cout << "Please run 'setup' first" << '\n';
        return false;
    }
    return true;
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
    double tempIncome, tempExpense;
    cout << "Enter income: ";
    cin >> tempIncome;

    if (!validateIncomeExpense(months[idx].income)) {
        cout << "Income cannot be negative!" << "\n";
        return false;
    }

    cout << "Enter expense: ";
    cin >> tempExpense;

    if (!validateIncomeExpense(months[idx].expense)) {
        cout << "Expense cannot be negative" << '\n';
        return false;
    }

    months[idx].income = tempIncome;
    months[idx].expense = tempExpense;
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
    if (!isSetupDoneAndMonthsC(isSetupDone)) {
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
    if (months[idx].isFilled)
    {
        cout << setw(10) << getMonthName(currMonthCount)
            << " | " << setw(10) << months[idx].income
            << " | " << setw(10) << months[idx].expense
            << " | " << setw(10) << months[idx].balance << '\n';
    }
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
    if (!isSetupDoneAndMonthsC(isSetupDone, monthsCount)) {
        return;
    }

    double totalIncome = 0;
    double totalExpense = 0;

    cout << setw(10) << "Month" << setw(10) << "Income" << setw(10) << "Expense" << setw(10) << "Balance" << '\n';
    cout << "--------------------------------------------------" << '\n';
    cout << fixed << setprecision(2);

    int monthsFilledCount = 0;
    for (int i = 0; i < monthsCount; i++)
    {
        totalIncome += months[i].income;
        totalExpense += months[i].expense;
        if (months[i].isFilled) monthsFilledCount++;

        printMonthsInfo(months, i);
    }

    printReportSummary(totalIncome, totalExpense, monthsFilledCount);
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
    cout << "Expense ratio: " << setprecision(4) << percentageExpense <<"%" << '\n';

}
void executeSearch(const Month* months, int monthsCount, bool isSetupDone)
{
   
    if (!isSetupDoneAndMonthsC(isSetupDone, monthsCount))
    {
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

void swapMonths(Month& a, Month& b) {
    Month temp = a;
    a = b;
    b = temp;
}

int getSortChoice()
{
    cout << "Sort by: " << '\n';
    cout << "1.Income" << '\n';
    cout << "2.Expense " << '\n';
    cout << "3.Balance" << '\n';
    cout << "Choice: ";

    int choice;
    cin >> choice;
    if (choice < 1 || choice > 3)
    {
        cout << "Invalid choice" << '\n';
        return 0;
    }
    return choice;
}

void printTopMonths(const Month* months,int choice, int monthsCount)
{
    cout << " Top 3 months (Sorted by: ";
    if (choice == 1) cout << "Income";
    else if (choice == 2) cout << "Expense";
    else cout << "Balance";
    cout << ") ---\n";

    int limit = (monthsCount < 3) ? monthsCount : 3;
    cout << fixed << setprecision(2);

    for (int i = 0; i < limit; i++)
    {
        double valueToShow;
        if (choice == 1) valueToShow = months[i].income;
        else if (choice == 2) valueToShow = months[i].expense;
        else valueToShow = months[i].balance;

        cout << (i + 1) << ". " << getMonthName(months[i].id) << ": ";
        if (valueToShow > 0)
        {
            cout << "+";
        }
        cout << valueToShow << '\n';
    }
}
void executeSort(Month* months, int monthsCount, bool isSetupDone)
{
    if (!isSetupDoneAndMonthsC(isSetupDone, monthsCount))
    {
        return;
    }

    int choice = getSortChoice();
    if (choice == 0)
    {
        return;
    }

    Month* tempMonths = new Month[monthsCount];
    for (int i = 0; i < monthsCount; i++)
    {
        tempMonths[i] = months[i];
    }
    for (int i = 0; i < monthsCount - 1; i++)
    {
        for (int j = 0; j < monthsCount - i - 1; j++)
        {
            bool shouldSwap=false;
            double firstValue, secondValue;

            if (choice == 1)
            {
                firstValue = tempMonths[j].income;
                secondValue = tempMonths[j + 1].income;
            }
            else if (choice == 2)
            {
                firstValue = tempMonths[j].expense;
                secondValue = tempMonths[j + 1].expense;
            }
            else 
            {
                firstValue = tempMonths[j].balance;
                secondValue = tempMonths[j + 1].balance;
            }

            if (firstValue < secondValue)
            {
                shouldSwap = true;
            }

            if (shouldSwap) {
                swapMonths(tempMonths[j], tempMonths[j + 1]);
            }
        }
    }
    printTopMonths(tempMonths, choice, monthsCount);
    delete[] tempMonths;
}

double calculateAvgChange(const Month* months, int monthsCount)
{
    int firstIdx = -1, lastIdx = -1;
    for (int i = 0; i < monthsCount; i++)
    {
        if (isMonthFilled(months[i]))
        {
            if (firstIdx == -1) firstIdx = i;
            lastIdx = i;
        }
    }

    if (firstIdx == -1 || lastIdx == -1 || firstIdx == lastIdx) {
        return 0.0;
    }

    double TotalChange = months[lastIdx].balance - months[firstIdx].balance;
    int timeDiff = lastIdx - firstIdx;
    return TotalChange / timeDiff;
}
void executeForecast(Month * months, int monthsCount, bool isSetupDone)
{
    if (!isSetupDoneAndMonthsC(isSetupDone, monthsCount))
    {
        return;
    }

    int inputedMonths;
    cin >> inputedMonths;

    if (cin.fail() || inputedMonths < 0)
    {
        cin.clear();
        char temp;
        while (cin.get(temp) && temp != '\n');
        cout << "Invalid months input!" << '\n';
        return;
    }

    int currentSavings = 0;
    for (int i = 0; i < monthsCount; i++)
    {
        currentSavings += months[i].balance;
    }

    double avgChange = calculateAvgChange(months, monthsCount);

    cout << "Current savings: " << currentSavings << '\n';
    cout << "Average monthly change: ";
    if (avgChange > 0)
    {
        cout << "+" << avgChange << '\n';
    }
    else cout << avgChange << '\n';
    double forecastedAmount = currentSavings + (inputedMonths * avgChange);
    cout << "Predicted savings after " << inputedMonths << " months: " <<fixed<< setprecision(2) << forecastedAmount << '\n';
}

void executeChart(const Month* months, int monthsCount, bool isSetupDone)
{
    if (!isSetupDoneAndMonthsC(isSetupDone, monthsCount))
    {
        return;
    }

    double maxIncome = months[0].income;
    for (int i = 1; i < monthsCount; i++)
    {
        if (isMonthFilled(months[i]) && months[i].income > maxIncome)
        {
            maxIncome = months[i].income;
        }
    }

    if (maxIncome == 0)
    {
        cout << "No income data to make a chart." << '\n';
        return;
    }

    int rows = 5;
    int step = (int)maxIncome / rows;
    if (step < 1) step = 1;
    int maxScale = step * rows;

    while (maxScale < maxIncome)
    {
        maxScale += step;
    }

    cout << "\n=== YEARLY FINANCIAL CHART ===\n";
    cout << "      |\n";

    for (int level = maxScale; level >= step; level -= step)
    {
        cout << setw(5) << level << " | ";

        for (int i = 0; i < monthsCount; i++)
        {
            if (isMonthFilled(months[i]))
            {
                if (months[i].income >= level)
                {
                    cout << "#   ";
                }
                else
                {
                    cout << "    ";
                }
            }
        }
        cout << '\n';
    }

    cout << "      -------------------------\n";
    cout << "       ";
    for (int i = 0; i < monthsCount; i++)
    {
        if (isMonthFilled(months[i]))
        {
            const char* name = getMonthName(months[i].id);
            cout << name[0] << name[1] << name[2] << " ";
        }
    }
    cout << "\n\n";
}

void runAplication()
{
    char command[MAX_LENGTH_STR];
    Month* months = nullptr;
    int monthsCount = 0;
    bool isSetupDone = false;

    while (true) {
        cout << ">";
        cin >> command;

        int cmdId = getCommandId(command);

        switch (cmdId) {
        case CMD_EXIT:
            delete[] months;
            return;

        case CMD_SETUP:
            executeSetup(months, monthsCount, isSetupDone);
            break;

        case CMD_ADD:
            executeAdd(months, monthsCount, isSetupDone);
            break;

        case CMD_REPORT:
            executeReport(months, monthsCount, isSetupDone);
            break;

        case CMD_SEARCH:
            executeSearch(months, monthsCount, isSetupDone);
            break;

        case CMD_SORT:
            executeSort(months, monthsCount, isSetupDone);
            break;

        case CMD_FORECAST:
            executeForecast(months, monthsCount, isSetupDone);
            break;

        case CMD_CHART:
            executeChart(months, monthsCount, isSetupDone);
            break;

        default:
            cout << "Invalid command! Try again." << endl;
            break;
        }
    }
}
int main()
{
    runAplication();
}

