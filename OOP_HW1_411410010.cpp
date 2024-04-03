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
    int logq_ID;
    int lock = 0;
    // int frequency;
    vector<int> adjacent;
    vector<vector<int>> shortypath;
public:
    phyqubits();
    int adj_count = 0;
    void initialize(int id){ID = id;logq_ID = id;};
    void amend_path(){shortypath.push_back({-1});};
    void adj_add(int node){adjacent.push_back(node);adj_count++;};
    vector<int> BFS(phyqubits (&now)[],int destination,int node);
    int lock_BFS(int node,phyqubits phyq[]);
    void path(int destination);
    int lock_get(){return lock;};
    int logq_get(){return logq_ID;};
    void swap(int id,phyqubits (&phyq)[]);
    int adj_check(int node);
    int distance(int x,phyqubits (&phyq)[],int node);
    // int frequency_check(){return frequency;};
};
class logqubits{//移動的點
private:
    int ID;
    int position;
    int used_time;
public:
    logqubits();
    queue<int> gate_case;
    void initialize(int id){ID = id;position = id;};
    void used_time_add(){used_time++;};
    int used_time_check(){return used_time;};
    void set_position(int pos){position = pos;};
    int position_check(){return position;};
    void swap(int id,logqubits (&logq)[]);
    int ID_get(){return ID;};
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
    void sequence(vector<pair<int,int>> &topo,Topological (&topmap)[],int gates);
    void show(){cout << wait_count << ":" << ID[0] << " " << ID[1] << endl;};
    void link_show(){for(int i = 0;i < link.size();i++){cout << link[i] << " ";}};
    void logq_gate_get(logqubits (&logq)[],vector<pair<int,int>> sequence);
};



void initialize(int node,int gates,int precedence,int link,phyqubits (&phyq)[],logqubits (&logq)[],Topological (&topmap)[],int (&logq_used_table)[]);
void swap(int x,int y);

int main(){
    int node,gates,precedence,link;
    cin >> node >> gates >> precedence >> node >> link;
    int logq_used_table[node],logq_table_count = node,count = 0;//用作減少initialize position時花費的時間
    vector<pair<int,int>> Topo_sequence;
    phyqubits phyq[node];
    logqubits logq[node];
    Topological topmap[gates];
    initialize(node,gates,precedence,link,phyq,logq,topmap,logq_used_table);
    #ifdef TIME
    auto start_time = high_resolution_clock::now();
    #endif
    //--------------------initilaize BFS(with node lock)
    //-----log used table 
    #ifdef DEBUG
    for(int i = 0;i<gates;i++){
        cout << i << ":" << endl;
        topmap[i].link_show();
        cout << endl;
    }
    for(int i = 0;i<gates;i++){
        topmap[i].show();
    }
    #endif
    topmap[0].sequence(Topo_sequence,topmap,gates);
    topmap[0].logq_gate_get(logq,Topo_sequence);
    #ifdef DEBUG
    for(int i = 0;i<gates;i++){
        cout << Topo_sequence[i].first << " " << Topo_sequence[i].second << endl;
    }
    #endif
    //-----graph initialize
    while(logq_table_count){
        int start,dest,new_pos,logq_change,logq_base;
        if(logq_used_table[Topo_sequence[count].first] == -1 || logq_used_table[Topo_sequence[count].second] == -1){
            // cout << logq_table_count << ":" <<Topo_sequence[count].first << " " << Topo_sequence[count].second << endl;
            logq_used_table[Topo_sequence[count].first] = 0;
            logq_used_table[Topo_sequence[count].second] = 0;
            if(phyq[logq[Topo_sequence[count].second].position_check()].lock_get() == 0){//確定基底是誰
                start = logq[Topo_sequence[count].first].position_check();
                dest = logq[Topo_sequence[count].second].position_check();//dest = 將會移動的位置
                logq_base = Topo_sequence[count].second;//logq_base = 將會移動的logq
                if(phyq[logq[Topo_sequence[count].first].position_check()].lock_get() == 0){
                    logq_table_count-=2;
                }
                else{
                    logq_table_count--;
                }
            }
            else{
                start = logq[Topo_sequence[count].second].position_check();
                dest = logq[Topo_sequence[count].first].position_check();
                logq_base = Topo_sequence[count].first;
                logq_table_count--;
            }
            new_pos = phyq[start].lock_BFS(node,phyq);
            logq_change = phyq[new_pos].logq_get();
            phyq[dest].swap(new_pos,phyq);
            logq[logq_base].swap(logq_change,logq);
        }
        
        count++;
        if(count == gates){//有logq 沒用到的情況
            break;
        }
    }
    count = 0;
    for(int i = 0;i<node;i++){
        cout << logq[i].ID_get()+1 << " " << logq[i].position_check()+1 << endl;
    }
    //----------------------------------------------------------do cnot gate
    for(int i = 0;i<gates;i++){
        int x,y;//logq
        int adj,path;
        int x_pos,y_pos;
        x = Topo_sequence[i].first;
        y = Topo_sequence[i].second;
        x_pos = logq[Topo_sequence[i].first].position_check();
        y_pos = logq[Topo_sequence[i].second].position_check();
        adj = phyq[x_pos].adj_check(y_pos);
        if(adj != -1){ //相鄰
            // cout << "CNOT q" << x+1 << " q" << y+1 << endl; 
            logq[x].gate_case.pop();
            logq[y].gate_case.pop();
        }
        else{
            vector<int> trace = phyq[x_pos].BFS(phyq, y_pos, node);//從x到y
            int x_d = phyq[x_pos].distance(logq[logq[x].gate_case.front()].position_check(),phyq,node) + phyq[x_pos].distance(logq[logq[y].gate_case.front()].position_check(),phyq,node);
            int y_d = phyq[y_pos].distance(logq[logq[x].gate_case.front()].position_check(),phyq,node) + phyq[y_pos].distance(logq[logq[y].gate_case.front()].position_check(),phyq,node);
            cout << x_d << " " << y_d << endl;
            if(y_d < x_d){
                cout << "ok" << endl;
                swap(x,y);
                swap(x_pos,y_pos);
                vector<int> reverse;
                for(int i = 0;i<trace.size();i++){
                    reverse[trace.size()-1-i] = trace[i];
                }
                for(int i = 0;i<trace.size();i++){
                    trace[i] = reverse[i];
                }
            }
            #ifdef DEBUG
            for(int i = 0;i<trace.size();i++){
                cout << trace[i]+1 << " ";
            }
            cout << endl;
            #endif
            for(int i = 0;i<trace.size();i++){//x是位置(基底) trace[i]是要換的路線
                // cout << trace[i]+1 << " ";
                // cout << "SWAP q" << x+1 << " q" << phyq[trace[i]].logq_get()+1 << endl;
                path = phyq[trace[i]].logq_get();
                phyq[x_pos].swap(trace[i],phyq);
                logq[x].swap(path,logq);
                x_pos = logq[x].position_check();
                count++;
                #ifdef DEBUG
                cout << "debug ";
                for(int i = 0;i<node;i++){
                    cout << phyq[i].logq_get()+1;
                }
                cout << endl;
                #endif
            }
            // cout << endl;
            // cout << "CNOT q" << x+1 << " q" << y+1 << endl;  
            logq[x].gate_case.pop();
            logq[y].gate_case.pop();
            // cout << "CNOT q" << phyq[x].logq_get()+1 << " q" << phyq[y].logq_get()+1 << endl; 
        }
        // adj = phyq[x].adj_check(y);
        // if(adj != -1){ //相鄰
        //     cout << "CNOT q" << phyq[x].logq_get()+1 << " q" << phyq[y].logq_get()+1 << endl; 
        // }
        // else{
        //     vector<int> trace = phyq[x].BFS(phyq, y, node);//從x到y
        //     for(int i = 0;i<trace.size();i++){//x是位置(基底) trace[i]是要換的路線
        //         // cout << trace[i]+1 << " ";
        //         cout << "SWAP q" << phyq[x].logq_get()+1 << " q" << phyq[trace[i]].logq_get()+1 << endl;
        //         logqpath = phyq[trace[i]].logq_get();
        //         phyq[x].swap(trace[i],phyq);
        //         logq[phyq[x].logq_get()].swap(logqpath,logq);
        //     }
        //     // cout << endl;
        //     cout << "CNOT q" << phyq[x].logq_get()+1 << " q" << phyq[y].logq_get()+1 << endl; 
        // }
    }
    #ifdef DEBUG
        for(int i = 0;i<node;i++){
            cout << phyq[i].logq_get()+1 << endl;
        }
    #endif
    #ifdef TIME
    cout << "swap:" << count << endl;
    auto end_time = high_resolution_clock::now();
    duration<double> elapsed_seconds = duration_cast<duration<double>>(end_time - start_time);
    cout << "Execution time: " << elapsed_seconds.count() << " seconds" << endl;
    #endif
}
//----------------------------------
phyqubits::phyqubits(){
    // frequency = 0;
}
logqubits::logqubits(){
    // used_time = 0;
}
Topological::Topological(){
}
void initialize(int node,int gates,int precedence,int link,phyqubits (&phyq)[],logqubits (&logq)[],Topological (&topmap)[],int (&logq_used_table)[]){
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
        logq[i].initialize(i);
        logq_used_table[i] = -1;
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
vector<int> phyqubits::BFS(phyqubits (&phyq)[],int destination,int node){
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
    // phyq[now].frequency++;//尾巴
    // frequency++;//頭
    for(int i = table[destination]-1;i>0;i--){
        int j = 0;
        while(1){
            if(table[phyq[now].adjacent[j]] == i){
                stemp.push(phyq[now].adjacent[j]);
                queue.push(phyq[now].adjacent[j]);
                now = phyq[now].adjacent[j];
                // phyq[now].frequency++;
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
    return trace;
    // if(ID == shortypath.size()){
    //     shortypath.push_back({-1});
    //     shortypath.push_back(trace);
    // }
    // else{
    //     shortypath.push_back(trace);
    // }
    // if(phyq[destination].ID == phyq[destination].shortypath.size()){
    //     phyq[destination].shortypath.push_back({-1});
    //     phyq[destination].shortypath.push_back(back_trace);
    // }
    // else{
    //     phyq[destination].shortypath.push_back(back_trace);
    // }
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
void Topological::sequence(vector<pair<int,int>> &topo,Topological (&topmap)[],int gates){
    int top,temp,count = gates;
    for(int i = 0;i<gates;i++){
        if(topmap[i].wait_count == 0){
            top = i;
            topmap[i].wait_count = -1;
            break;
        }
    }
    topo.push_back(make_pair(topmap[top].ID[0],topmap[top].ID[1]));
    while(count){
        while(!topmap[top].link.empty()){
            temp = topmap[top].link.back();
            topmap[temp].wait_count--;
            topmap[top].link.pop_back();
        }
        for(int i = 0;i<gates;i++){
            if(topmap[i].wait_count == 0){
                top = i;
                topmap[top].wait_count = -1;
                break;
            }
        }
        topo.push_back(make_pair(topmap[top].ID[0],topmap[top].ID[1]));
        count--;
        #ifdef DEBUG
        cout << "------------------------------" << endl;
        for(int i = 0;i<gates;i++){
            topmap[i].show();
        }
        cout << "------------------------------" << endl;
        #endif
    }
}

int phyqubits::lock_BFS(int node,phyqubits phyq[]){
    queue<int> queue;
    int now,table[node],temp;
    for(int i = 0;i<node;i++){
        table[i] = -1;
    }
    queue.push(ID);
    lock = 1;
    table[ID] = 0;
    while(!queue.empty()){
        now = queue.front();
        queue.pop();
        if(phyq[now].lock == 0){
            phyq[now].lock = 1;
            return now;
        }
        else{
            for(int i = 0;i<phyq[now].adjacent.size();i++){
                if(table[phyq[now].adjacent[i]]==-1){
                    queue.push(phyq[now].adjacent[i]);
                    table[phyq[now].adjacent[i]] = 0;
                }
            }
        }
    }
    return -1;//-----------------------------------------------------------------可能出現記憶體錯誤
}
void phyqubits::swap(int id,phyqubits (&phyq)[]){
    int temp = logq_ID;
    logq_ID = phyq[id].logq_ID;
    phyq[id].logq_ID = temp;
}
void logqubits::swap(int id,logqubits (&logq)[]){
    int temp = position;
    position = logq[id].position;
    logq[id].position = temp;
}
int phyqubits::adj_check(int node){
    for(int i = 0;i<adjacent.size();i++){
        // cout << node+1 << " " << adjacent[i]+1 << endl; 
        if(node == adjacent[i]){
            return adjacent[i];
        }
    }
    return -1;
}
int phyqubits::distance(int x,phyqubits (&phyq)[],int node){
    vector<int> distance = BFS(phyq,x,node);
    return distance.size();
}
void Topological::logq_gate_get(logqubits (&logq)[],vector<pair<int,int>> sequence){
    for(int i = 0;i<sequence.size();i++){
        int x = sequence[i].first;
        int y = sequence[i].second;
        logq[x].gate_case.push(y);
        logq[y].gate_case.push(x);
    }
}
void swap(int x,int y){
    int temp = x;
    x = y;
    y = temp;
}