#include <iostream>
#include <fstream>
#include <queue>

using namespace std;
 
class Task {
public:
    int p, w, d, C, T, id;

    // assignement operator overload
    Task & operator = (const Task& task) {
        this->p = task.p;
        this->w = task.w;
        this->d = task.d;
        this->C = task.C;
        this->T = task.T;
        this->id = task.id;
        return * this;
    }
};


// simple function to swap two tasks by their ids
void swapTasks(int task1Id, int task2Id, Task taskTable[]) {
    Task temp;
    temp = taskTable[task1Id];
    taskTable[task1Id] = taskTable[task2Id];
    taskTable[task2Id] = temp;
}

int N = 0;


void countC (int n, Task taskTable[]) {
    int t = 0;
    for(int i=0;i<n;i++) {
        t += taskTable[i].p;
        taskTable[i].C = t;
    }
}

void countT (int n, Task taskTable[]) {
    countC(n, taskTable);
    for(int i=0;i<n;i++) {
        if(taskTable[i].C <= taskTable[i].d) {
            taskTable[i].T = 0;
        } else {
            taskTable[i].T = taskTable[i].C - taskTable[i].d;
        }
    }
}

int countWITI (int n, Task taskTable[]) {
    int witi = 0;
    countT(n, taskTable);
    for(int i=0;i<n;i++) {
        witi += taskTable[i].w * taskTable[i].T;
    }
    return witi;
}


void printPermutation(int n, Task taskTable[]) {
    int witi = countWITI(n, taskTable);
    if (witi == 766) {
        for(int i=0;i<n;i++) {
            cout << taskTable[i].id << " ";
        }
        cout << endl;
        N++;
    }
}

void findPermutations(int n, int id, Task taskTable[]) { 
  if( id < n-1 )
  {
    for(int i = id; i < n; i++)
    {
      swapTasks(id, i, taskTable);
      findPermutations(n, id+1, taskTable);
      swapTasks(id, i, taskTable);
    }
  }
  else {
      printPermutation(n, taskTable);
  }
}

int main () {
    ifstream data;
    data.open("witi.data.txt");
    int n, witi;
    string str;
    while(str != "data.10:") {
        data >> str;
    }
    data >> str;
    n = stoi(str);
    Task taskTable[n-1];
    // load dataset to taskTable
    for(int i=0;i<n;i++) {
        data >> taskTable[i].p >> taskTable[i].w >> taskTable[i].d;
        taskTable[i].id = i+1;
    }    

    findPermutations(n, 0, taskTable);
    cout << N << endl;

    witi = countWITI(n, taskTable);
    cout << witi << endl;
   
    // display the results
    for(int i=0;i<n;i++){
        cout << taskTable[i].p << " " << taskTable[i].w << " " 
        << taskTable[i].d << "  " <<  taskTable[i].C << "  " << taskTable[i].T << "  " << endl;
    }
}