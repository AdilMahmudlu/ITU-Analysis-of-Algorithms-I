#include <vector>
using namespace std;

//Max Heap
class Heap {
    private:
        vector<double> heap;
        int capacity;
        int heapSize;
        bool sorted;
        double sum;
        int heapifyCount;
        int buildHeapCount;
        int heapSortCount;

    public:
        Heap(int);
        void heapify(int);
        void buildHeap();
        void heapSort();
        void insert(double);
        double getMean();
        double getSTD();
        double getMin();
        double getFirstq();
        double getMedian();
        double getThirdq();
        double getMax();
        void printHeapStats();
};