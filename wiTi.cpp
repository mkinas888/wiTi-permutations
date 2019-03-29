#include <iostream>
#include <fstream>
#include <queue>
#include <cmath>

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

class Sum {
public:
    int sum, id;
    Sum () {
        sum = 0;
        id = 0;
    }

    Sum (int _id, int _sum) {
        id = _id;
        sum = _sum;
    }
};

class Subtotal {
public:
    Sum sums[20];
    int  sumMin, sumNum, id;
    Subtotal() {
        id = 0;
    }
};


// simple function to swap two tasks by their ids
void swapTasks(int task1Id, int task2Id, Task taskTable[]) {
    Task temp;
    temp = taskTable[task1Id];
    taskTable[task1Id] = taskTable[task2Id];
    taskTable[task2Id] = temp;
}

int N = 0, OPT = 0;


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

int countMarshalledWITI (int n, Task taskTable[]) {
    int witi = 0;
    countT(n, taskTable);
    for(int i=0;i<n;i++) {
        witi += taskTable[i].w * taskTable[i].T;
    }
    return witi;
}


void printPermutation(int n, int opt, Task taskTable[]) {
    int witi = countMarshalledWITI(n, taskTable);
    if (witi == opt) {
        for(int i=0;i<n;i++) {
            cout << taskTable[i].id << " ";
        }
        cout << endl;
        N++;
    }
}

void findPermutations(int n, int id, int opt, Task taskTable[]) { 
  if( id < n-1 )
  {
    for(int i = id; i < n; i++)
    {
      swapTasks(id, i, taskTable);
      findPermutations(n, id+1, opt, taskTable);
      swapTasks(id, i, taskTable);
    }
  }
  else {
      printPermutation(n, opt, taskTable);
  }
}

int checkWhichPowerOfTwo(int number) {
    int tmp = 0, id;
    for(int i=0;i<30;i++) {
        tmp = pow(2, i);
        if (tmp == number) {
            id = i;
            break;
        }
    }
    return id;
}

void findOptimalWITI(int n, Task taskTable[]) {
    int order[n-1];
    int initialSumNum = n, initialId = 0, elem;
    for(int i=0, b=1; i<n; i++, b*=2) {
        initialId += b;
    }
    int C, N = 1 << n, stepsCounter = 0, current;
    Subtotal * steps = new Subtotal[N];
    steps[0].sumMin = 0;
    steps[0].id = 0;
    for(int set = 1; set < N; set++) {
        C = 0;
        for(int i=0, b=1; i<n; i++, b*=2) {
            if(b&set) {
                C+=taskTable[i].p;
            }
        }
        steps[set].sumMin = 999999999;
        stepsCounter = 0;
        for(int i=0, b=1; i<n; i++, b*=2) {
            if(b&set) {
                steps[set].id += b; 
                steps[set].sums[stepsCounter].id = set-b;
                steps[set].sums[stepsCounter].sum = steps[set-b].sumMin + taskTable[i].w * max(0, C - taskTable[i].d);
                steps[set].sumMin = min(steps[set].sumMin, steps[set-b].sumMin + taskTable[i].w * max(0, C - taskTable[i].d));
                stepsCounter++;
            }
        }
        steps[set].sumNum = stepsCounter; 
    }  

    int parsedOrder = steps[N-1].id, tmp = 0;
    for(int i=n-1;i>=0;i--) {
        for(int x=N-1;x>=0;x--) {
            current = 999999999;
            if(initialId == steps[x].id && initialSumNum == steps[x].sumNum) {
                for(int id=0;id<steps[x].sumNum;id++) {
                    if(steps[x].sums[id].sum <= current) {
                        current = steps[x].sums[id].sum;
                        elem = steps[x].sums[id].id;
                    }
                }
                tmp = parsedOrder - elem;
                parsedOrder = elem;
                order[i] = checkWhichPowerOfTwo(tmp) + 1;
                initialSumNum--;
                initialId = elem;
                break;
            }
        }
    }
    for(int i=0;i<n;i++) {
        cout << order[i] << " ";
    }    
    cout << endl;
    cout << steps[N-1].sumMin << endl;
    OPT = steps[N-1].sumMin;
    delete [] steps;
}



int main () {
    ifstream data;
    data.open("witi.data.txt");
    int n;
    string str;
    string dataNames[] = {"data.10:", "data.11:", "data.12:", "data.13:", "data.14:","data.15:",
                          "data.16:", "data.17:", "data.18:", "data.19:", "data.20:" };
    for(int i=0;i<11;i++) {
        while(str != dataNames[i]) {
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
        cout << dataNames[i] << endl;
        findOptimalWITI(n, taskTable);
        cout << endl;
        // findPermutations(n, 0, OPT, taskTable);
    }    
}