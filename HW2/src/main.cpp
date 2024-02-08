#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "../include/Heap.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

void doQueries(vector<string> query, Heap& h, vector<double>& answer) {
    for (int i = 0; i < query.size(); i++) { //self explanatory. do the estimations according to query names.
        if (query[i].find("mean") != string::npos) {
            answer[i] = h.getMean();
        } 
        else if (query[i].find("std") != string::npos) {
            answer[i] = h.getSTD();
        } 
        else if (query[i].find("min") != string::npos) {
            answer[i] = h.getMin();
        } 
        else if (query[i].find("firstq") != string::npos) {
            answer[i] = h.getFirstq();
        } 
        else if (query[i].find("median") != string::npos) {
            answer[i] = h.getMedian();
        } 
        else if (query[i].find("thirdq") != string::npos) {
            answer[i] = h.getThirdq();
        } 
        else if (query[i].find("max") != string::npos) {
            answer[i] = h.getMax();
        }
    }
    return;
};

// if the dates were not given in order, this would compare the dates to find the earliest and latest dates. 
// skipped because increases time

// void compareDates(string currentDate, string currentTime, string dateTime[4]) { //compare given date/times to earliest and latest received dates/times and update
//     if (dateTime[0] == "") { //if earliest and latest dates/times do not exist, set to given date/time 
//         dateTime[0] = currentDate;
//         dateTime[1] = currentTime;
//         dateTime[2] = currentDate;
//         dateTime[3] = currentTime;
//         return;
//     }
//     int c[6]; //current
//     int e[6]; //earliest
//     int l[6]; //latest
//     string word; //general use string
//     stringstream cd(currentDate); //current date
//     stringstream ct(currentTime); //current time
//     stringstream ed(dateTime[0]); //earliest date
//     stringstream et(dateTime[1]); //earliest time
//     stringstream ld(dateTime[2]); //latest date
//     stringstream lt(dateTime[3]); //latest time
//     for (int i = 0; i < 3; i++) { //read the strings and save to integer array as D,Mo,Y,H,Mi,S
//         getline(cd, word, '/');
//         c[i] = stoi(word);
//         getline(ct, word, ':');
//         c[i+3] = stoi(word);
//         getline(ed, word, '/');
//         e[i] = stoi(word);
//         getline(et, word, ':');
//         e[i+3] = stoi(word);
//         getline(ld, word, '/');
//         l[i] = stoi(word);
//         getline(lt, word, ':');
//         l[i+3] = stoi(word);
//     }
//     //if the year is lower or (equal and month is lower or (equal and day is lower or (equal and hour is lower or (equal and minute is lower or (equal and second is lower)))))
//     if ((c[2] < e[2]) || ((c[2] == e[2]) && ((c[1] < e[1]) || ((c[1] == e[1]) && ((c[0] < e[0]) || ((c[0] == e[0]) && ((c[3] < e[3]) || ((c[3] == e[3]) && ((c[4] < e[4]) || ((c[4] == e[4]) && ((c[5] < e[5])))))))))))) {
//         dateTime[0] = currentDate;
//         dateTime[1] = currentTime;
//     }
//     //same for higher
//     if ((c[2] > l[2]) || ((c[2] == l[2]) && ((c[1] > l[1]) || ((c[1] == l[1]) && ((c[0] > l[0]) || ((c[0] == l[0]) && ((c[3] > l[3]) || ((c[3] == l[3]) && ((c[4] > l[4]) || ((c[4] == l[4]) && ((c[5] > l[5])))))))))))) {
//         dateTime[2] = currentDate;
//         dateTime[3] = currentTime;
//     }
// };

void updateDates(string currentDate, string currentTime, string dateTime[4]) { //compare given date/times to earliest and latest received dates/times and update
    if (dateTime[0] == "") { //if earliest and latest dates/times do not exist, set to given date/time 
        dateTime[0] = currentDate;
        dateTime[1] = currentTime;
        dateTime[2] = currentDate;
        dateTime[3] = currentTime;
    }
    else {
        dateTime[2] = currentDate;
        dateTime[3] = currentTime;
        return;
    }
}

int main (int argc, char ** argv) {
    try {
        if (argc < 2) {
            throw(0); // if no file is provided at the command line
        }
        bool needSorting = false;
        auto start = high_resolution_clock::now();
        int index; //index of the feature we are interested in
        int number; //general use integer
        string line; //general use strings
        string word;
        string words[6]; //general use string array
        vector<string> queries; //estimators
        vector<double> answers; //estimator results
        string dateTimes[4] = {"", "", "", ""}; //earliest and latest date and time (ed, et, ld, lt)

        fstream file (argv[1], ios::in); //open file
        if (!file.is_open()) {
            throw(1); // if the file is not open, throw an exeption
        }
        getline(file, line);
        number = stoi(line);
        for (int i = 0; i < number; i++) {
            getline(file, line);
            queries.push_back(line);
            answers.push_back(0); //initialize "number" number of elements
            //if there is no need to find firstq, median, thirdq, or min, we don't need to sort the heap
            if (line.find("firstq") != string::npos || line.find("median") != string::npos || line.find("thirdq") != string::npos || line.find("min") != string::npos) {
                needSorting = true;
            }
        }
        getline(file, line); //read empty line
        getline(file, line); 
        stringstream s(line);
        getline(s, word, ',');
        number = stoi(word);
        Heap h(number/2); //create heap with capacity number/2. The maximum possible insertions is number/2, because each insertion takes 2 lines.
        getline(s, word);
        //find the index of desired data
        //had to use find() because when compared, word == "gap" or word == "gap " or word == "gap\n" or word == "gap\0" all returned false for some reason.
        if (word.find("gap") != string::npos) {
            index = 2;
        }
        else if (word.find("grp") != string::npos) {
            index = 3;
        }
        else if (word.find("v") != string::npos) {
            index = 4;
        }
        else if (word.find("gi") != string::npos) {
            index = 5;
        }

        fstream outFile ("output1.txt", ios::out);

        while (getline(file, word)) { //read line by line
            if (word.find("add") != string::npos) { //if the operation is add
                getline(file, line); //read next line
                stringstream s(line);
                for (int i = 0; i < 6; i++) { //read word by word
                    getline(s, words[i], ',');
                }
                updateDates(words[0], words[1], dateTimes); //see if the date is latest or earliest
                h.insert(stod(words[index]));
            }
            else if (word.find("print") != string::npos) { //if the operation is print
                if (needSorting) {
                    h.heapSort(); //sort the heap
                }
                doQueries(queries, h, answers); //find the estimater results
                outFile << dateTimes[0] << "," << dateTimes[1] << "," << dateTimes[2] << "," << dateTimes[3]; //write to the file
                for (int i = 0; i < queries.size(); i++) {
                    outFile << "," << answers[i];
                }
                outFile << "\n";
            }
        }
        outFile.close();
        auto stop = high_resolution_clock::now();
        cout << "Execution time in milliseconds: "  << duration_cast<milliseconds>(stop - start).count() << endl;
        h.printHeapStats();
    }

    catch  (int error) {
        if (error == 0) {
            cout << "No file name on command line\n";
        }
        if (error == 1) {
            cout << "Could not open file\n";
        }
        if (error == 2) {
            cout << "Heap Overflow\n";
        }
    }
    return 0;
}