#include <iostream>

class Topological{//拓譜圖
private:
    int check = 999;
public:
    Topological();
    void check(Topological _a[]);
};
using namespace std;
int main(){
    int link;
    Topological a[10];
    cin >> link;
    int temp,x[link],y[link];
    for(int i = 0;i<link;i++){
        cin >> temp >> x[i] >> y[i];
        x[i]--;
        y[i]--;
    }
    for(int i = 0;i<link;i++){
        cout << x[i] << " " << y[i] << endl;
    }
}