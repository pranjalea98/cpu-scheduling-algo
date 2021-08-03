#include<bits/stdc++.h>
#include<limits>
using namespace std;

class Process {
public:
    string processName;
    int arrivalTime;
    int inceptionTime;
    int cpuBurstTime[2];
    int ioBurstTime;
    bool ioDone;
    int firstResponseTime;
    int responseTime;
    int completionTime;
    int remainingTime;

    int waitingTime;
    int turnAroundTime;

    void initialize() {
        remainingTime = cpuBurstTime[0];
        inceptionTime = arrivalTime;
        ioDone = false;
    }
};

bool refactorAttributes(Process& p) {
    if (p.ioBurstTime == 0) {
        p.cpuBurstTime[0] += p.cpuBurstTime[1];
        p.cpuBurstTime[1] = 0;
    }
    else if (p.cpuBurstTime[0] == 0) {
        if (p.arrivalTime != p.ioBurstTime) return 0;
        p.cpuBurstTime[0] += p.cpuBurstTime[1];
        p.cpuBurstTime[1] = 0;
    }
    return 1;
}

int main() {
    freopen("sjfin.txt", "r", stdin);
    //freopen("sjfout.txt", "w", stdout);
    freopen("results.txt","a",stdout);

    cout<< "\n================== SJF ====================\n";

    int numOfProcesses;
    cout << "Enter no. of processes: ";
    cin >> numOfProcesses;

    Process processes[numOfProcesses];

    for (int n = 0; n < numOfProcesses; n++) {
        cout << "\nEnter Process Name for " << (n + 1) << ": ";
        cin >> processes[n].processName;
        cout << "Enter Arrival Time for Process " << (n + 1) << ": ";
        cin >> processes[n].arrivalTime;
        cout << "Enter (CPU IO CPU) Burst Time for Process " << (n + 1) << ": ";
        cin >> processes[n].cpuBurstTime[0] >> processes[n].ioBurstTime >> processes[n].cpuBurstTime[1];
        if (!refactorAttributes(processes[n])) {
            cout << "Process " << (n + 1) << " attributes mismatch\n";
            return 1;
        }
        processes[n].initialize();
    }

    cout << "\n" << endl;

    int currentTime = 0;

    while (true) {

        int currentShortestJobIndex = -1;
        int currentShortestJobRemainingTime = numeric_limits<int>::max();
        int currentMinArrivalTime = numeric_limits<int>::max();

        bool isAllCompleted = true;

        for (int i = 0; i < numOfProcesses; i++) {
            if (processes[i].remainingTime > 0) {
                isAllCompleted = false;
                currentMinArrivalTime = min(currentMinArrivalTime, processes[i].arrivalTime);
            }
        }

        if (isAllCompleted) {
            break;
        }

        if (currentTime < currentMinArrivalTime) currentTime = currentMinArrivalTime;

        for (int i = 0; i < numOfProcesses; i++) {
            if (processes[i].remainingTime > 0 && processes[i].arrivalTime <= currentTime) {
                if (processes[i].remainingTime < currentShortestJobRemainingTime) {
                    currentShortestJobRemainingTime = processes[i].remainingTime;
                    currentShortestJobIndex = i;
                }
            }
        }


        Process& cp = processes[currentShortestJobIndex];

        if (cp.remainingTime == cp.cpuBurstTime[0] && !cp.ioDone) {
            cp.firstResponseTime = currentTime;
        }

        if (!cp.ioDone) {
            currentTime += cp.cpuBurstTime[0];
            cp.remainingTime = cp.cpuBurstTime[1];
            if (cp.ioBurstTime > 0) {
                cp.arrivalTime = currentTime + cp.ioBurstTime;
                cp.ioDone = true;
                if (cp.remainingTime == 0) cp.completionTime = cp.arrivalTime;
            }
            else cp.completionTime = currentTime;
        }
        else {
            currentTime += cp.cpuBurstTime[1];
            cp.completionTime = currentTime;
            cp.remainingTime = 0;
        }
    }

    int sumResponseTime = 0;
    int sumCompletionTime = 0;
    int sumWaitingTime = 0;
    int sumTurnAroundTime = 0;
    int maxCompletionTime = -1;

    for (int n = 0; n < numOfProcesses; n++) {
        cout << "\nProcess " << processes[n].processName << ":\n";
        processes[n].responseTime = processes[n].firstResponseTime - processes[n].inceptionTime;
        cout << "Response Time: " << processes[n].responseTime << endl;
        cout << "Completion Time: " << processes[n].completionTime << endl;
        maxCompletionTime = max(maxCompletionTime, processes[n].completionTime);
        processes[n].turnAroundTime = processes[n].completionTime - processes[n].inceptionTime;
        processes[n].waitingTime = processes[n].turnAroundTime - (processes[n].cpuBurstTime[0] + processes[n].cpuBurstTime[1] + processes[n].ioBurstTime);
        cout << "Waiting Time: " << processes[n].waitingTime << endl;
        cout << "Turn Around Time: " << processes[n].turnAroundTime << "\n" << endl;

        sumResponseTime += processes[n].responseTime;
        sumCompletionTime += processes[n].completionTime;
        sumWaitingTime += processes[n].waitingTime;
        sumTurnAroundTime += processes[n].turnAroundTime;
    }

    // AVERAGES

    float avgResponseTime = (float)sumResponseTime / numOfProcesses;
    float avgCompletionTime = (float)sumCompletionTime / numOfProcesses;
    float avgWaitingTime = (float)sumWaitingTime / numOfProcesses;
    float avgTurnAroundTime = (float)sumTurnAroundTime / numOfProcesses;
    float throughPut = (float)numOfProcesses / maxCompletionTime;

    cout << "\n\nAverage Response Time for " << (numOfProcesses) << " Processes: " << avgResponseTime;
    cout << "\n\nAverage Completion Time for " << (numOfProcesses) << " Processes: " << avgCompletionTime;
    cout << "\n\nAverage Waiting Time for " << (numOfProcesses) << " Processes: " << avgWaitingTime;
    cout << "\n\nAverage Turn Around Time for " << (numOfProcesses) << " Processes: " << avgTurnAroundTime;

    cout << "\n\nMaximum Completion Time for " << (numOfProcesses) << " Processes: " << maxCompletionTime;
    cout << "\n\nThroughput for " << (numOfProcesses) << " Processes: " << throughPut;

    //VARIANCES

    float varResponseTime=0;
    float varCompletionTime=0;
    float varWaitingTime=0;
    float varTurnAroundTime=0;

    for (int n = 0; n < numOfProcesses; n++) {
        varResponseTime+= pow(processes[n].responseTime- avgResponseTime, 2);
        varCompletionTime+= pow(processes[n].completionTime- avgCompletionTime, 2);
        varWaitingTime+= pow(processes[n].waitingTime- avgWaitingTime, 2);
        varTurnAroundTime+= pow(processes[n].turnAroundTime- avgTurnAroundTime, 2);
        
    }

    varResponseTime/= numOfProcesses;
    varCompletionTime/= numOfProcesses;
    varWaitingTime/= numOfProcesses;
    varTurnAroundTime/= numOfProcesses;
    
    cout << "\n\nVariance Response Time for " << (numOfProcesses) << " Processes: " << varResponseTime;
    cout << "\n\nVariance Completion Time for " << (numOfProcesses) << " Processes: " << varCompletionTime;
    cout << "\n\nVariance Waiting Time for " << (numOfProcesses) << " Processes: " << varWaitingTime;
    cout << "\n\nVariance Turn Around Time for " << (numOfProcesses) << " Processes: " << varTurnAroundTime;

    return 0;
}