#include <algorithm>
#include <cstdlib>

int INTRO_LIMIT_INSERTION = 10;

namespace introsort_impl {

inline int myrand(int lo, int hi) {
    return lo + std::rand() % (hi - lo);
}

void insertionSort(int array[], int L, int R) {
    for (int i = L + 1; i < R; ++i) {
        for (int j = i; j > L; --j) {
            if (array[j] < array[j - 1]) {
                std::swap(array[j], array[j - 1]);
            } else {
                break;
            }
        }
    }
}

void siftDown(int array[], int offset, int start, int end) {
    int root = start;
    while (true) {
        int child = root * 2 + 1;
        if (child > end) {
            break;
        }
        int swapIndex = root;
        if (array[offset + swapIndex] < array[offset + child]) {
            swapIndex = child;
        }
        if (child + 1 <= end && array[offset + swapIndex] < array[offset + child + 1]) {
            swapIndex = child + 1;
        }
        if (swapIndex == root) {
            return;
        }
        std::swap(array[offset + root], array[offset + swapIndex]);
        root = swapIndex;
    }
}

void heapSortRange(int array[], int lo, int hi) {
    int n = hi - lo;
    if (n <= 1) {
        return;
    }
    for (int start = (n - 2) / 2; start >= 0; --start) {
        siftDown(array, lo, start, n - 1);
    }
    for (int end = n - 1; end > 0; --end) {
        std::swap(array[lo], array[lo + end]);
        siftDown(array, lo, 0, end - 1);
    }
}

int partition(int array[], int lo, int hi) {
    std::swap(array[lo], array[myrand(lo, hi)]);
    int pivot = array[lo];
    int mi = lo;
    for (int k = lo + 1; k < hi; ++k) {
        if (array[k] < pivot) {
            std::swap(array[++mi], array[k]);
        }
    }
    std::swap(array[lo], array[mi]);
    return mi;
}

void intro_sort_impl(int array[], int lo, int hi, int depthLimit) {
    int n = hi - lo;
    if (n <= 1) {
        return;
    }
    if (n <= INTRO_LIMIT_INSERTION) {
        insertionSort(array, lo, hi);
        return;
    }
    if (depthLimit <= 0) {
        heapSortRange(array, lo, hi);
        return;
    }
    int mi = partition(array, lo, hi);
    intro_sort_impl(array, lo, mi, depthLimit - 1);
    intro_sort_impl(array, mi + 1, hi, depthLimit - 1);
}

} // namespace introsort_impl

void intro_sort(int array[], int lo, int hi) {
    int n = hi - lo;
    if (n <= 1) {
        return;
    }
    int depthLimit = 0;
    for (int m = n; m > 1; m >>= 1) {
        ++depthLimit;
    }
    depthLimit *= 2;
    introsort_impl::intro_sort_impl(array, lo, hi, depthLimit);
}
