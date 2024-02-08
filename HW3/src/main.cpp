#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include "../include/RBTree.h"
#include <chrono>

using namespace std;
using namespace chrono;

vector<vector<string>> readInputs (string fileName) { //read inputs from txt file
    vector<vector<string>> lines;
    string numProcesses, simulatorRunTime, processID, timeOfArrival, burstTime;
    fstream inputs (fileName, ios::in);
    if (inputs.is_open()) { //open the file
        inputs >> numProcesses >> simulatorRunTime; //read line by line
        lines.push_back({numProcesses, simulatorRunTime});
        for (int i = 0; i < stoi(numProcesses); i++) { //for each process read it's data and stor in vector of string vectors
            inputs >> processID >> timeOfArrival >> burstTime;
            lines.push_back({processID, timeOfArrival, burstTime});
        }
        inputs.close();
    }
    else {
        throw(0); // if the file is not open, throw an exeption
    }
    return lines;
}

int main (int argc, char* argv[]) {
    try{
        if (argc < 2) { //if no txt file is in terminal, throw exception
            throw(2);
        }
        auto start = high_resolution_clock::now(); //start timer
        vector<vector<string>> lines = readInputs(argv[1]);
        vector<string> completions; //names of completed processes
        fstream outputs("output.txt", ios::out);
        RBTree tree;
        int time = 0; //current time
        int completeProcesses = 0; //number of completed processes
        int processIndex = 0; //the line number we are
        int numProcesses = stoi(lines[0][0]); 
        int simulatorTime = stoi(lines[0][1]);
        while (time < simulatorTime && completeProcesses < numProcesses) { //while simulator time is not exceeded and all processes are not completed
            string status = ",Incomplete\n";
            for (int i = processIndex; i < numProcesses; i++) { //starting from current line, until end
                if (stoi(lines[1+i][1]) == time) { //if timeOfArrival is current time, create node and add to tree
                    Node* x = new Node(lines[1+i][0], stoi(lines[1+i][1]), stoi(lines[1+i][2]), 0);
                    tree.RBTinsert(x);
                    processIndex++; //next line
                }
                else { //no more nodes this in time
                    break;
                }
            }
            if (tree.getRoot() != tree.leaf) { //if root is not leaf
                Node* currentProcess = tree.minNode(tree.getRoot()); //current process is leftmost node
                if (currentProcess->vRunTime + 1 == currentProcess->burstTime) { //if this time process is completed
                    completions.push_back(currentProcess->processID); //add it to completions vector and change status to complete
                    completeProcesses++;
                    status = ",Complete\n";
                }
                //write to output file
                outputs << time << "," << currentProcess->processID << "," << currentProcess->vRunTime << "," << tree.getMinVrt() << ",";
                outputs << tree.inorderTraversal(tree.getRoot());
                outputs << status;
                if (status == ",Complete\n") { //if it was completed, delete
                    tree.RBTdelete(currentProcess);
                }
                //if after this time minvrt won't change, delete and insert updated node
                else if (currentProcess != tree.getRoot() && (currentProcess->parent->vRunTime == currentProcess->vRunTime || tree.minNode(currentProcess->right)->vRunTime == currentProcess->vRunTime)) {
                    Node* updatedProcess = new Node(currentProcess->processID, currentProcess->timeOfArrival, currentProcess->burstTime, currentProcess->vRunTime+1);
                    tree.RBTdelete(currentProcess);
                    tree.RBTinsert(updatedProcess);
                }
                else {//else continue doing the process and new minvrt is this node's vrt
                    currentProcess->vRunTime++;
                    tree.setMinVrt(currentProcess->vRunTime);
                }
            }
            else { //if tree is leaf, no nodes are added
                outputs << time << ",-,-,-,-,-,-\n";
            }
            time++;
        }
        auto stop = high_resolution_clock::now(); //stop timer
        auto duration = duration_cast<milliseconds>(stop - start); //read timer
        outputs << "\nScheduling finished in " << duration.count() << "ms.\n"; //write to output file
        outputs << completeProcesses << " of " << numProcesses << " processes completed.\n";
        if (completeProcesses > 0) { //if there are completions, write to output file
            outputs << "The order of completion of the tasks: " << completions[0];
            for (int i = 1; i < completeProcesses; i++) {
                outputs << "-" << completions[i];
            }
        }
    }
    catch (int error) {
        if (error == 0) {
            cout << "File cannot be opened.\n";
        }
        else if (error == 1) {
            cout << "Deletion failed: Node not found.\n";
        }
        else if (error == 2) {
            cout << "No file name on command line.\n";
        }
    }
    return 0;
}