#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "DS.h"
#include "Q1.h"
using namespace std;

static const int M_SIZE = 16;
static const int BC1_THRESHOLD = 2 * M_SIZE * M_SIZE * M_SIZE; // generous O(m^3) bound
static const int BC2_THRESHOLD = 4 * M_SIZE * M_SIZE;          // generous O(m^2) bound

// Ground truth: runs directly on raw data, does not touch Matrix access counter
CrossResult naiveBestCross(int data[][M_SIZE], int m) {
    CrossResult best = {-1, -1, 0};
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < m; c++) {
            if (data[r][c] == 0) continue;
            int L = 0; while (c-L-1 >= 0 && data[r][c-L-1]) L++;
            int R = 0; while (c+R+1 < m  && data[r][c+R+1]) R++;
            int U = 0; while (r-U-1 >= 0 && data[r-U-1][c]) U++;
            int D = 0; while (r+D+1 < m  && data[r+D+1][c]) D++;
            int k = min({L, R, U, D});
            if (k > best.arm) best = {r, c, k};
        }
    }
    return best;
}



int main() {
    srand(42); // fixed seed for reproducibility

    for (int X = 1; X <= 5; X++) {

        // Initialize raw matrix to zero
        int rawData[M_SIZE][M_SIZE];
        for (int i = 0; i < M_SIZE; i++)
            for (int j = 0; j < M_SIZE; j++)
                rawData[i][j] = 1;
        
        // Punch X random zero-blocks to break up straight lines
        for (int i = 0; i < X * 3; i++) {
            int r = rand() % M_SIZE;
            int c = rand() % M_SIZE;
            rawData[r][c] = 0;
        }        

        // Build int** for Matrix constructor
        int* rows[M_SIZE];
        for (int i = 0; i < M_SIZE; i++)
            rows[i] = rawData[i];

        Matrix M(M_SIZE, rows);

        // Compute ground truth from raw data (no Matrix access counting)
        CrossResult expected = naiveBestCross(rawData, M_SIZE);

        // --- BigCross1 ---
        M.resetAccessCount();
        CrossResult res1 = BigCross1(M, M_SIZE);
        if (res1.arm != expected.arm)
            cout << "BigCross1 returned wrong answer for test " << X << " (-10)" << endl;
        if (M.getAccessCount() > BC1_THRESHOLD)
            cout << "BigCross1 exceeded O(m^3) access limit for test " << X << " (-10)" << endl;

        // --- BigCross2 ---
        M.resetAccessCount();
        CrossResult res2 = BigCross2(M, M_SIZE);
        if (res2.arm != expected.arm)
            cout << "BigCross2 returned wrong answer for test " << X << " (-10)" << endl;
        if (M.getAccessCount() > BC2_THRESHOLD)
            cout << "BigCross2 exceeded O(m^2) access limit for test " << X << " (-10)" << endl;
    }

    cout << "done" << endl;
    return 0;
}