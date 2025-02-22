#include <iostream>
#include <vector>
#include <map>


using namespace std;
int main(){
    map<int,int> i;
    i.insert(pair<int,int>(0,1));
    i.insert(pair<int,int>(1,3));
    cout << i[1] << " " << i[0]<< endl;

}