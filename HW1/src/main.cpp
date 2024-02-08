#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

int ari = 3; // index of average_rating attribute on the table. for the given table, ari is 3
int swapCount = 0;
int partitionCount = 0;

//turn csv file to vector of string vectors
vector<vector<string>> csvToCpp (string fileName) {
    vector<vector<string>> table;
    vector<string> tuple;
    string line, word;

    fstream file (fileName, ios::in);
    if (file.is_open()) {
        while (getline(file, line)) { //read line by line
            tuple.clear();
            stringstream s(line);
            while (getline(s, word, ',')) { //read word by word in each line
                tuple.push_back(word); //push each word to tuples
            }
            table.push_back(tuple); //push each tuple to the table
        }
    }
    else {
        throw(0); // if the file is not open, throw an exeption
    }
    return table;
}

//turn vector of string vectors to a csv file
void cppToCsv (string fileName, vector<vector<string>> table) {
    fstream file (fileName, ios::out);
    for (int i = 0; i < table.size(); i++) { //for each row of table
        for (int j = 0; j < table[i].size()-1; j++) { //each word in the given row of the table
            file << table[i][j] << ',';
        }
        file << table[i][table[i].size()-1] << '\n';
    }
    file.close();
}

//quicksort implementation
void quickSort (vector<vector<string>> &table, int low, int high) { //table passed by reference
    int i = low;
    int j = low;
    double pivot = stod(table[high][ari]); //stod function to turn strings into doubles
    partitionCount++; //each quicksort call partitions the data once
    if (j >= i) {
        //while the data is less than pivot, increment both i and j until data larger than pivot is found
        while ((stod(table[i][ari]) <= pivot) && (stod(table[j][ari]) <= pivot) && (j < high)) {
            j++;
            i++;
        }
        //at the end of this while loop, i is the index of the leftmost bigger-than-pivot data
        //for each data larger than pivot, swap the next data and the ith data (leftmost bigger-than-pivot data)
        while ((stod(table[j][ari]) > pivot) && (j < high)) {
            j++;
            swap(table[i], table[j]);
            swapCount++;
            //if the j is last data, then pivot was swithched with leftmost bigger-than-pivot data, 
            //so all bigger-than-pivot numbers are on the right of pivot
            if (j == high) {  
                break;
            }
            //if j is not last data, then check if ith data is now less tahn pivot, if yes, increment i so that
            //i shows the index of the leftmost bigger-than-pivot data
            if (stod(table[i][ari]) <= pivot) {
                i++;
            }
        }
    }
    //if the left part is of lenght 1, don't call the quicksort
    if (i-1 > low) {
        quickSort(table, low, i-1);
    }
    //if the right part is of lenght 1, don't call the quicksort
    if (high > i+1) {
        quickSort(table, i+1, high);
    }
    return;
}

int main (int argc, char* argv[]) {
    try {
        //if the name of the ccsv file is not on the command line, then throw an exeption
        if (argc < 2) {
            throw(1);
        }
        else {
            vector<vector<string>> table = csvToCpp(argv[1]); //create and fill the table
            auto start = high_resolution_clock::now(); //start position of the timer
            quickSort(table, 1, table.size()-1); //call quicksort
            auto stop = high_resolution_clock::now(); //end position of the timer
            auto duration = duration_cast<microseconds>(stop-start); //execution time
            cppToCsv("sorted_books.csv", table); //create csv file
            cout << "Execution time: " << duration.count() << "μs\n"; //statistics
            cout << "Number of partitions: " << partitionCount << '\n';
            cout << "Number of swaps: " << swapCount << '\n';
        }
    }
    catch  (int error) {
        if (error == 0) {
            cout << "Could not open file" << '\n';
        }
        if (error == 1) {
            cout << "No file name on command line" << '\n';
        }
    }
    return 0;
}