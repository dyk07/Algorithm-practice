#include <algorithm>
#include <cstdlib>

int DUAL_LIMIT_INSERTION = 10;

namespace dualpivot_impl {

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

void dual_pivot_quicksort_impl(int array[], int lo, int hi) {
    int n = hi - lo;
    if (n <= 1) {
        return;
    }
    if (n <= DUAL_LIMIT_INSERTION) {
        insertionSort(array, lo, hi);
        return;
    }

    int p1 = myrand(lo, hi);
    int p2 = myrand(lo, hi);
    if (p1 == p2) {
        p2 = (p1 + 1 < hi) ? p1 + 1 : p1 - 1;
    }
    if (p1 > p2) {
        std::swap(p1, p2);
    }

    std::swap(array[lo], array[p1]);
    if (p2 == lo) {
        p2 = p1;
    }
    std::swap(array[hi - 1], array[p2]);

    if (array[lo] > array[hi - 1]) {
        std::swap(array[lo], array[hi - 1]);
    }

    int pivot1 = array[lo];
    int pivot2 = array[hi - 1];
    int lt = lo + 1;
    int gt = hi - 2;
    int i = lt;

    while (i <= gt) {
        if (array[i] < pivot1) {
            std::swap(array[i], array[lt]);
            ++lt;
            ++i;
        } else if (array[i] > pivot2) {
            std::swap(array[i], array[gt]);
            --gt;
        } else {
            ++i;
        }
    }

    --lt;
    ++gt;
    std::swap(array[lo], array[lt]);
    std::swap(array[hi - 1], array[gt]);

    dual_pivot_quicksort_impl(array, lo, lt);
    if (pivot1 < pivot2) {
        dual_pivot_quicksort_impl(array, lt + 1, gt);
    }
    dual_pivot_quicksort_impl(array, gt + 1, hi);
}

} // namespace dualpivot_impl

void dual_pivot_quicksort(int array[], int lo, int hi) {
    dualpivot_impl::dual_pivot_quicksort_impl(array, lo, hi);
}
