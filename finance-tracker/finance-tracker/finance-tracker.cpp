#include<iostream>
using std::cin;
using std::cout;
using std::endl;

const int MIN_MONTH_COUNT = 1;
const int MAX_MONTH_COUNT = 12;
bool validateMonthsCount(int input)
{
    return input >= MIN_MONTH_COUNT && input <= MAX_MONTH_COUNT;
}
int main()
{
    cout << "> setup" << '\n';
    cout << "Enter number of months: ";

    int monthsCount;
    cin >> monthsCount;

    if (!validateMonthsCount(monthsCount)) 
    {
        cout << "The profile can not be created! Invalid input!" << '\n';
        return -5;
    }
    cout << "Profile created successfully." << '\n';
    cout << "\n\n";



}
