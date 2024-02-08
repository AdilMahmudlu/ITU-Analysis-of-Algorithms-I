#include "../include/Heap.h"
#include <iostream>
#include <cmath>

using namespace std;

//heap indexes start from 0 instead of 1

Heap::Heap(int capacity) { //initialize heap
    this->capacity = capacity;
    this->heapSize = 0;
    this->sorted = false;
    this->sum = 0;
    int heapifyCount = 0;
    int buildHeapCount = 0;
    int heapSortCount = 0;
}   

void Heap::heapify(int i) { //heapify as given in the lecture slides
    int l = 2*i+1;
    int r = 2*i+2;
    int largest = i;
    if ((l < heapSize) && (heap[l] > heap[i])) {
        largest = l;
    }
    if ((r < heapSize) && (heap[r] > heap[largest])) {
        largest = r;
    }
    if (largest != i) {
        swap(heap[i], heap[largest]);
        heapify(largest);
    }
    heapifyCount++;
    return;
}

void Heap::buildHeap() { //buildHeap as given in the lecture slides
    for (int i = (heapSize-2)/2; i >= 0; i--) {
        heapify(i);
    }
    buildHeapCount++;
}

void Heap::heapSort() { //heapsort as given in the lecture slides
    if (sorted) {
        return;
    }
    int size = heapSize; //save the old heapSize
    buildHeap();
    for (int i = heapSize-1; i > 0; i--) {
        swap(heap[0], heap[i]);
        heapSize--;
        heapify(0);
    }
    heapSize = size; //retrieve the old heapSize
    sorted = true;
    heapSortCount++;
    return;
}

void Heap::insert(double k) { //insert as given in the lecture slides
    if (heapSize == capacity) {
        throw(2); //if heap overflow. shouldn't happen if the number of lines is given correctly
        return;
    }
    heapSize++;
    int i = heapSize-1;
    heap.push_back(k);
    while (i > 0 && heap[(i-1)/2] < heap[i]) {
        swap(heap[i], heap[(i-1)/2]);
        i = (i-1)/2;
    }
    sorted = false;
    sum += k;
    return;
}

// Estimator functions. Called only after heapSort()

double Heap::getMean() {
    return (sum/heapSize);
}

double Heap::getSTD() {
    double mean = getMean();
    double total = 0;
    for (int i = 0; i < heapSize; i++) {
        total += pow(heap[i]-mean, 2);
    }
    return sqrt(total/(heapSize-1));
}

//after sorted, the heap is in ascending order
double Heap::getMin() {
    return heap[0];
}

double Heap::getFirstq() {
    int low = (heapSize-1)/4;
    int high = (heapSize-1)/4+1;
    double part = (heapSize-1)/4.0 - low;
    return heap[low]*(1-part) + heap[high]*part;
}

double Heap::getMedian() {
    if (heapSize%2) {
        return heap[heapSize/2];
    }
    else {
        return (heap[heapSize/2] + heap[heapSize/2-1])/2; 
    }
}

double Heap::getThirdq() {
    int low = 3*(heapSize-1)/4;
    int high = 3*(heapSize-1)/4+1;
    double part = 3.0*(heapSize-1)/4.0 - low;
    return heap[low]*(1-part) + heap[high]*part;
}

double Heap::getMax() {
    if (sorted) {//in sorted heap, max element is at the end
        return heap[heapSize-1];
    }
    else {//if not sorted, max element is in the beginning
        return heap[0];
    }
}

void Heap::printHeapStats() {
    cout << "heapify called: " << heapifyCount << endl;
    cout << "buildHeap called: " << buildHeapCount << endl;
    cout << "heapSort called: " << heapSortCount << endl;
}