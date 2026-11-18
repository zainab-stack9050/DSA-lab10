// Task 1: Max Heap Insertion and Heap Sort
#include <iostream>
using namespace std;

class MaxHeap {
    int* data;
    int size;
    int capacity;

    //swaps two integers with Time Complexity: O(1)
    void swapVals(int& a, int& b) { int t = a; a = b; b = t; }

    //doubles the array capacity when full 
    //Time Complexity: O(n)
    void grow() {
        capacity *= 2;
        int* bigger = new int[capacity];
        for (int i = 0; i < size; i++) bigger[i] = data[i];
        delete[] data;
        data = bigger;
    }

    //moves the element at index i up while it is larger than its parent.
    //Time Complexity: O(log n)
    int siftUp(int i) {
        int swaps = 0;
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (data[i] > data[parent]) {
                swapVals(data[i], data[parent]);
                i = parent;
                swaps++;
            } else break;
        }
        return swaps;          //returns the number of swaps made 
    }

    //moves the element at index i down within arr until the max-heap 
    //Time Complexity: O(log n)
    static void siftDown(int* arr, int n, int i) {
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, largest = i;
            if (l < n && arr[l] > arr[largest]) largest = l;
            if (r < n && arr[r] > arr[largest]) largest = r;
            if (largest == i) break;
            int t = arr[i]; arr[i] = arr[largest]; arr[largest] = t;
            i = largest;
        }
    }

public:
    MaxHeap(int cap = 4) : size(0), capacity(cap) { data = new int[capacity]; }
    ~MaxHeap() { delete[] data; }

    //inserts value at the end, sifts it up, prints the result
    //Time Complexity: O(log n) worst case
    void insert(int value) {
        if (size == capacity) grow();
        data[size] = value;
        int swaps = siftUp(size);
        size++;
        cout << "Insert " << value << " | Swaps: " << swaps << " | Heap: ";
        print(); }

    //prints the heap array with Time Complexity: O(n)
    void print() const {
        for (int i = 0; i < size; i++) cout << data[i] << " ";
        cout << endl; }

    // Heap sort: copies the heap, repeatedly swaps the root with the last
    // element, shrinks the heap and sifts down 
    //Time Complexity: O(n log n)
    int* heapSort() const {
        int* arr = new int[size];
        for (int i = 0; i < size; i++) arr[i] = data[i];
        for (int end = size - 1; end > 0; end--) {
            int t = arr[0]; arr[0] = arr[end]; arr[end] = t;
            siftDown(arr, end, 0);
        }
        return arr;
    }
    int getSize() const { return size; }
};

int main() {
    int input[] = {25, 40, 15, 60, 35, 10, 70, 50};
    int n = sizeof(input) / sizeof(input[0]);

    MaxHeap heap;
    for (int i = 0; i < n; i++) heap.insert(input[i]);

    cout << "\nFinal max heap: ";
    heap.print();

    int* sorted = heap.heapSort();
    cout << "Sorted output: ";
    for (int i = 0; i < heap.getSize(); i++) cout << sorted[i] << " ";
    cout << endl;
    delete[] sorted;

    return 0;
}

 /* Answers:
       1) The new element can go from the last level up to the root, so the number of swaps 
       is the height of the tree which is about log n. So O(log n).

       2) Building the heap by n insertions: each is O(log n), so O(n log n) in the worst case 
       (a tighter bound for average random data is O(n)).
       Heap sort: n times sift down, each O(log n), so O(n log n).
       
    */