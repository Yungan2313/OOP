#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>//檢查運算時間
// #define DEBUG
#define TIME
using namespace std;
using namespace std::chrono;

class phyqubits{//位置
private:
    int ID;
    int frequency;
    vector<int> adjacent;
    vector<vector<int>> shortypath;
public:
    phyqubits();
    int adj_count = 0;
    void initialize(int id){ID = id;};
    void amend_path(){shortypath.push_back({-1});};
    void adj_add(int node){adjacent.push_back(node);adj_count++;};
    int adj_check(int position){return adjacent[position];};
    void BFS(phyqubits (&now)[],int destination,int node);
    void path(int destination);
    int frequency_check(){return frequency;};
};
class logqubits{//移動的點
private:
    int ID;
    int position;
    int used_time;
    vector<int> gate_case;
public:
    logqubits();
    void used_time_add(){used_time++;};
    int used_time_check(){return used_time;};
    void set_position(int pos){position = pos;};
    int position_check(){return position;};
};
class Topological{//拓譜圖
private:
    int ID[2];
    int wait_count = 0;
    vector<int> link;
public:
    Topological();
    void initialize(int ID,logqubits (&logq)[]);
    void build(int wait){link.push_back(wait);};
    void countplus(){wait_count++;};
};



void initialize(int node,int gates,int precedence,int link,phyqubits (&phyq)[],logqubits (&logq)[],Topological (&topmap)[]);

int main(){
    int node,gates,precedence,link;
    priority_queue<pair<int,int>> phyq_freq_table,logq_ut_table;//1.(frequency,ID)(phyq) 2.(used time,ID)
    cin >> node >> gates >> precedence >> node >> link;
    phyqubits phyq[node];
    logqubits logq[node];
    Topological topmap[gates];
    initialize(node,gates,precedence,link,phyq,logq,topmap);
    #ifdef DEBUG
    for(int i = 0;i<node;i++){
        cout << "ID:" << i << "adjacent:" << endl;
        for(int j = 0;j<phyq[i].adj_count;j++){
            cout << phyq[i].adj_check(j) << " ";
        }
        cout << endl;
    }
    #endif

    //---------------------BFS全部點到點的距離+路徑
    #ifdef TIME
    auto start_time = high_resolution_clock::now();
    #endif
    for(int i = 0;i<node;i++){
        for(int j = i+1;j<node;j++){
            phyq[i].BFS(phyq,j,node);
        }
    }
    phyq[node-1].amend_path();
    for(int i = 0;i<node;i++){
        phyq_freq_table.push(make_pair(phyq[i].frequency_check(),i));
        logq_ut_table.push(make_pair(logq[i].used_time_check(),i));
    }
    #ifdef DEBUG
    for(int i = 0;i<node;i++){
        cout << i << ":";
        // cout << phyq[i].frequency_check() << endl;

        // cout << logq[i].used_time_check() << endl;

        // for(int j = 0;j<node;j++){
        //     cout << j << "=>";
        //     phyq[i].path(j);
        // }

        // cout << phyq_freq_table.top().first << phyq_freq_table.top().second << endl;
        // phyq_freq_table.pop();
        cout << logq_ut_table.top().first << logq_ut_table.top().second << endl;
        logq_ut_table.pop();
    }
    #endif
    //--------------------set the orgin position
    for(int i = 0;i<node;i++){
        logq[logq_ut_table.top().second].set_position(phyq_freq_table.top().second);
        logq_ut_table.pop();
        phyq_freq_table.pop();
    }
    #ifdef DEBUG
    for(int i = 0;i<node;i++){
        cout << i << ":" << logq[i].position_check()<< endl;
    }
    #endif
    //--------------------initilaize BFS(with node lock)


    #ifdef TIME
    auto end_time = high_resolution_clock::now();
    duration<double> elapsed_seconds = duration_cast<duration<double>>(end_time - start_time);
    cout << "Execution time: " << elapsed_seconds.count() << " seconds" << endl;
    #endif
}
//----------------------------------
phyqubits::phyqubits(){
    frequency = 0;
}
logqubits::logqubits(){
    used_time = 0;
    position = -1;
}
Topological::Topological(){
}
void initialize(int node,int gates,int precedence,int link,phyqubits (&phyq)[],logqubits (&logq)[],Topological (&topmap)[]){
    int temp,topnode,wait;
    int pq1,pq2;
    for(int i = 0;i<gates;i++){
        topmap[i].initialize(i,logq);
    }
    for(int i = 0;i<precedence;i++){
        cin >> temp >> topnode >> wait;
        topnode--;
        wait--;
        topmap[topnode].build(wait);
        topmap[wait].countplus();
    }
    for(int i = 0;i<link;i++){
        cin >> temp >> pq1 >> pq2;
        pq1--;
        pq2--;
        phyq[pq1].adj_add(pq2);
        phyq[pq2].adj_add(pq1);
    }
    for(int i = 0;i<node;i++){
        phyq[i].initialize(i);
    }
}

//------------------------------
void Topological::initialize(int id,logqubits (&logq)[]){
    int temp;
    cin >> temp >> ID[0] >> ID[1];
    ID[0]--;
    ID[1]--;
    logq[ID[0]].used_time_add();
    logq[ID[1]].used_time_add();
    
}
void phyqubits::BFS(phyqubits (&phyq)[],int destination,int node){
    queue<int> queue;
    vector<int> trace,back_trace;
    stack<int> stemp;
    int now,table[node],temp;
    for(int i = 0;i<node;i++){
        table[i] = -1;
    }
    queue.push(ID);
    table[ID] = 0;
    while(!queue.empty()){
        now = queue.front();
        queue.pop();
        if(now == destination){
            break;
        }
        else{
            for(int i = 0;i<phyq[now].adjacent.size();i++){
                if(table[phyq[now].adjacent[i]]==-1){
                    queue.push(phyq[now].adjacent[i]);
                    table[phyq[now].adjacent[i]] = table[now]+1;
                }
            }
        }
    }
    #ifdef DEBUG
    cout << "dest:" << destination << endl;
    for(int i = 0;i<node;i++){
        cout << table[i] << " ";
    }
    cout << endl;
    #endif
    for(int i = 0;i<queue.size();i++){
        queue.pop();
    }
    //-------------------------trace
    now = destination;
    phyq[now].frequency++;//尾巴
    frequency++;//頭
    stemp.push(now);
    queue.push(now);
    for(int i = table[destination]-1;i>0;i--){
        int j = 0;
        while(1){
            if(table[phyq[now].adjacent[j]] == i){
                stemp.push(phyq[now].adjacent[j]);
                queue.push(phyq[now].adjacent[j]);
                now = phyq[now].adjacent[j];
                phyq[now].frequency++;
                break;
            }
            j++;
        }
    }
    temp = stemp.size();
    for(int i = 0;i<temp;i++){
        trace.push_back(stemp.top());
        back_trace.push_back(queue.front());
        stemp.pop();
        queue.pop();
    }
    if(ID == shortypath.size()){
        shortypath.push_back({-1});
        shortypath.push_back(trace);
    }
    else{
        shortypath.push_back(trace);
    }
    if(phyq[destination].ID == phyq[destination].shortypath.size()){
        phyq[destination].shortypath.push_back({-1});
        phyq[destination].shortypath.push_back(back_trace);
    }
    else{
        phyq[destination].shortypath.push_back(back_trace);
    }
    // vector<int>  :: iterator iter = shortypath[destination].begin();
    // for(int ix = 0; iter != shortypath[destination].end(); ++iter, ++ix){
    //     cout << *iter;
    // }
    // cout << endl;
}
void phyqubits::path(int destination){
    vector<int>  :: iterator iter = shortypath[destination].begin();
    for(int ix = 0; iter != shortypath[destination].end(); ++iter, ++ix){
        cout << *iter << " ";
    }
    cout << endl;
}