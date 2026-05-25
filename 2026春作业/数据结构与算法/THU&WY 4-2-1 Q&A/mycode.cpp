#include "QAServer.h"
#include <algorithm>
#include <cstring>

int* buildBC(char* question) {
    int* bc = new int[256];
    for ( int i = 0; i < 256; i++ ) {
        bc[i] = -1;
    }
    for ( int i = 0, m = static_cast<int>(strlen(question)); i < m; i++ ) {
        bc[static_cast<unsigned char>(question[i])] = i;
    }
    return bc;
}

int* buildSS(char* question) {
    int m = static_cast<int>(strlen(question));
    int* ss = new int[m];
    ss[m - 1] = m;
    for ( int lo = m - 1, hi = m - 1, j = m - 2; j >= 0; j-- ) {
        if ( j > lo && ss[j + m - 1 - hi] < j - lo ) {
            ss[j] = ss[j + m - 1 - hi];
        } else {
            hi = j;
            lo = std::min(lo, hi);
            while ( lo >= 0 && question[lo] == question[lo + m - 1 - hi] ) {
                lo--;
            }
            ss[j] = hi - lo;
        }
    }
    return ss;
}

int* buildGS(char* question) {
    int m = static_cast<int>(strlen(question));
    int* ss = buildSS(question);
    int* gs = new int[m];
    for ( int i = 0; i < m; i++ ) {
        gs[i] = m;
    }
    for ( int i = 0, j = m - 1; j >= 0; j-- ) {
        if ( ss[j] == j + 1 ) {
            while ( i < m - 1 - j ) {
                gs[i++] = m - 1 - j;
            }
        }
    }
    for ( int j = 0; j < m - 1; j++ ) {
        gs[m - 1 - ss[j]] = m - 1 - j;
    }
    delete[] ss;
    return gs;
}

int GetAnswer(char* text, char* question) {
    int n = static_cast<int>(strlen(text));
    int m = static_cast<int>(strlen(question));
    if ( m == 0 ) {
        return 0;
    }
    int* bc = buildBC(question);
    int* gs = buildGS(question);
    int i = 0;
    while ( i <= n - m ) {
        int j = m - 1;
        while ( j >= 0 && text[i + j] == question[j] ) {
            j--;
        }
        if ( j < 0 ) {
            delete[] bc;
            delete[] gs;
            return i;
        }
        unsigned char next_char = static_cast<unsigned char>(text[i + j]);
        i += std::max(gs[j], j - bc[next_char]);
    }
    delete[] bc;
    delete[] gs;
    return n;
}