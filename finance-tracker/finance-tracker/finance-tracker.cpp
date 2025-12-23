#include<iostream>
using std::cin;
using std::cout;
using std::endl;

const int MIN_MONTH = 1;
const int MAX_MONTH = 12;
bool validateMonths(int input)
{
    return input >= MIN_MONTH && input <= MAX_MONTH;
}
int main()
{
    cout << "> setup" << '\n';
    cout << "Enter number of months: ";

    int months;
    cin >> months;

    if (!validateMonths(months)) 
    {
        cout << "The profile can not be created! Invalid input!" << '\n';
        return -5;
    }
    cout << "Profile created successfully." << '\n';


}
