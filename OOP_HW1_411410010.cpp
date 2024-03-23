#include <iostream>
#include <vector>

class phyqubits{//位置
private:
    int ID;
    int frequency;
    vector<int> adjacent;
    vector<vector<int>> shortypath;
public:
    phyqubits();
    void adj_add(int node){adjacent.push_back(node);};
};
class logqubits{//移動的點
private:
    int ID;
    int used_time;
    vector<int> gate_case;
public:
    logqubits();
};
class Topological{//拓譜圖
private:
    int ID[2];
    int wait_count = 0;
    vector<int> link;
public:
    Topological();
    void initialize(int ID);
    void build(int wait){link.push_back(wait);};
    void countplus(){wait_count++;};
};



void initialize(int gates,int precedence,int link,phyqubits (&phyq)[],Topological (&topmap)[]);


using namespace std;
int main(){
    int node,gates,precedence,link;
    cin >> node >> gates >> precedence >> node >> link;
    phyqubits phyq[node];
    logqubits logq[node];
    Topological topmap[gates];
    initialize(gates,precedence,link,phyq,topmap);
    
}
//----------------------------------
phyqubits::phyqubits(){

}
logqubits::logqubits(){

}
Topological::Topological(){
}
void initialize(int gates,int precedence,int link,phyqubits (&phyq)[],Topological (&topmap)[]){
    int temp,topnode,wait;
    int pq1,pq2;
    for(int i = 0;i<gates;i++){
        topmap[i].initialize(i);
    }
    for(int i = 0;i<precedence;i++){
        cin >> temp >> topnode >> wait;
        topmap[topnode].build(wait);
        topmap[wait].countplus();
    }
    for(int i = 0;i<link;i++){
        cin >> pq1 >> pq2;
        phyq[pq1].adj_add(pq2);
        phyq[pq2].adj_add(pq1);
    }
}

//------------------------------
void Topological::initialize(int id){
    int temp;
    cin >> temp >> ID[0] >> ID[1];
}