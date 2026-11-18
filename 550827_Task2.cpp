// Task 2: Hospital ER Wait-Time Monitor 
#include <iostream>
#include <iomanip>
using namespace std;

//heap supporting both max and min ordering through a flag
class Heap {
    int* data;
    int size;
    int capacity;
    bool isMax;

    //true if a should be closer to the root than b. Time: O(1)
    bool before(int a, int b) const { return isMax ? a > b : a < b; }

    //doubles capacity when full. Time: O(n)
    void grow() {
        capacity *= 2;
        int* bigger = new int[capacity];
        for (int i = 0; i < size; i++) bigger[i] = data[i];
        delete[] data;
        data = bigger;
    }

    // Time: O(log n)
    void siftUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (before(data[i], data[p])) {
                int t = data[i]; data[i] = data[p]; data[p] = t;
                i = p;
            } else break;
        }
    }

    // Time: O(log n)
    void siftDown(int i) {
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, best = i;
            if (l < size && before(data[l], data[best])) best = l;
            if (r < size && before(data[r], data[best])) best = r;
            if (best == i) break;
            int t = data[i]; data[i] = data[best]; data[best] = t;
            i = best;
        }
    }

public:
    Heap(bool maxHeap) : size(0), capacity(4), isMax(maxHeap) { data = new int[capacity]; }
    ~Heap() { delete[] data; }

    // Adds x and restores heap order with sift-up. Time: O(log n)
    void push(int x) {
        if (size == capacity) grow();
        data[size] = x;
        siftUp(size);
        size++;
    }

    // Removes and returns the root; sift-down restores order. Time: O(log n)
    int pop() {
        int top = data[0];
        data[0] = data[--size];
        if (size > 0) siftDown(0);
        return top;
    }

    int top() const { return data[0]; }          // O(1)
    int getSize() const { return size; }         // O(1)
    bool empty() const { return size == 0; }     // O(1)
};

class ERMonitor {
    Heap low;   // max heap: smaller half
    Heap high;  // min heap: larger half
    double threshold;

public:
    ERMonitor(double t) : low(true), high(false), threshold(t) {}

    // Inserts the wait into the right heap, then rebalances so sizes differ by at most one
    // Time: O(log n)
    void addWait(int x) {
        if (low.empty() || x <= low.top()) low.push(x);
        else high.push(x);

        if (low.getSize() > high.getSize() + 1) high.push(low.pop());
        else if (high.getSize() > low.getSize()) low.push(high.pop());
    }

    // Median from the two roots. Time: O(1)
    double getMedian() const {
        if (low.getSize() > high.getSize()) return low.top();
        return (low.top() + high.top()) / 2.0;
    }

    // Prints one report line. Time: O(1)
    void report(int arrival, int wait) const {
        double med = getMedian();
        cout << setw(7) << arrival << setw(6) << wait << setw(10) << low.top();
        if (high.empty()) cout << setw(10) << "none";
        else cout << setw(10) << high.top();
        cout << setw(9) << fixed << setprecision(1) << med << "   "
             << (med > threshold ? "ALERT: call extra staff" : "Normal") << endl;
    }
};

int main() {
    int waits[] = {45, 20, 60, 15, 30, 90, 10, 55};
    int n = sizeof(waits) / sizeof(waits[0]);

    ERMonitor monitor(35.0);
    cout << "Arrival  Wait  MaxRoot   MinRoot   Median   Status" << endl;
    for (int i = 0; i < n; i++) {
        monitor.addWait(waits[i]);
        monitor.report(i + 1, waits[i]);
    }
    return 0;
}
  /* Answers:
       1) The median ignores extremes; the mean does not. At arrival 6 the waits are 15 20 30 45 60 90. 
       Median = (30+45)/2 = 37.5 (ALERT), and the mean = 260/6 = 43.3, pulled up by the single 90-minute outlier.
       The median reflects what a typical patient experiences, so one extreme case cannot trigger or hide an alert on its own.

       2) If sizes differ by 2 or more, the roots are no longer the middle elements of the sorted data, so the median 
       computed from them is wrong. The rebalance step keeps the sizes within 1 so the roots are always the middle values.

       3) Re-sorting after every arrival: about n sorts of O(k log k) each, roughly O(n^2 log n) total. 
       Heap method: O(log n) per arrival and O(1) median lookup, so O(n log n) total.
    */