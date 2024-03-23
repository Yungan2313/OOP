#include <iostream>

using namespace std;
struct day{
    int month;
    int day;
};
class DayofYear{
public:
    DayofYear(int,int);
    DayofYear();
    static void gettoday();
    void output();
    void intput();
private:
    struct day day;
    static int today_d;
    static int today_m;
};
int DayofYear::today_d = 3;
int DayofYear::today_m = 3;
int main(){
    DayofYear yubirthday,birthday(1,16);
    yubirthday.output();
    birthday.output();
    yubirthday.gettoday();
    birthday.gettoday();
}
void DayofYear::output(){
    cout << day.month << "/" << day.day << endl;
}
void DayofYear::intput(){
    cin >> day.month >> day.day;
}
DayofYear::DayofYear(int m,int d){
    day.month = m;
    day.day = d;
}
DayofYear::DayofYear(){
    day.month = 10;
    day.day = 18;
}
void DayofYear::gettoday(){
    cout << today_m << "/" << today_d << endl;
}