// Q1.h
#ifndef Q1_H
#define Q1_H

#include "DS.h"
#include <algorithm>
#include <vector>
using std::min;
using std::vector;

struct CrossResult {
    int row, col, arm; 
};

CrossResult BigCross1(const Matrix& M, int m) {
    CrossResult best = {-1, -1, 0};

    for (int r = 0; r < m; r++) {
        for (int c = 0; c < m; c++) {
            // אם התא הוא 0, אין טעם לבדוק
            if (M.get(r, c) == 0) continue;

            // סריקה לארבעת הכיוונים
            int L = 0; while (c - L - 1 >= 0 && M.get(r, c - L - 1) == 1) L++;
            int R = 0; while (c + R + 1 < m  && M.get(r, c + R + 1) == 1) R++;
            int U = 0; while (r - U - 1 >= 0 && M.get(r - U - 1, c) == 1) U++;
            int D = 0; while (r + D + 1 < m  && M.get(r + D + 1, c) == 1) D++;

            // הזרוע מוגבלת לפי הכיוון הקצר ביותר
            int k = min({L, R, U, D});
            if (k > best.arm) {
                best = {r, c, k};
            }
        }
    }
    return best;
}

CrossResult BigCross2(const Matrix& M, int m) {
    CrossResult best = {-1, -1, 0};

    // העתקת המטריצה כדי לחסוך בגישות (M.get) ולעמוד בדרישות ה-Threshold של מערכת הבדיקה
    vector<vector<int>> mat(m, vector<int>(m, 0));
    for (int r = 0; r < m; ++r) {
        for (int c = 0; c < m; ++c) {
            mat[r][c] = M.get(r, c);
        }
    }

    // טבלאות עזר לשמירת כמות ה-1-ים הרצופים מכל כיוון (כולל התא הנוכחי)
    vector<vector<int>> L(m, vector<int>(m, 0));
    vector<vector<int>> R(m, vector<int>(m, 0));
    vector<vector<int>> U(m, vector<int>(m, 0));
    vector<vector<int>> D(m, vector<int>(m, 0));

    // סריקה ראשונה: מלמעלה-למטה, משמאל-לימין
    for (int r = 0; r < m; ++r) {
        for (int c = 0; c < m; ++c) {
            if (mat[r][c] == 1) {
                L[r][c] = (c == 0) ? 1 : L[r][c - 1] + 1;
                U[r][c] = (r == 0) ? 1 : U[r - 1][c] + 1;
            }
        }
    }

    // סריקה שנייה: מלמטה-למעלה, מימין-לשמאל
    for (int r = m - 1; r >= 0; --r) {
        for (int c = m - 1; c >= 0; --c) {
            if (mat[r][c] == 1) {
                R[r][c] = (c == m - 1) ? 1 : R[r][c + 1] + 1;
                D[r][c] = (r == m - 1) ? 1 : D[r + 1][c] + 1;
            }
        }
    }

    // מציאת הצלב הגדול ביותר
    for (int r = 0; r < m; ++r) {
        for (int c = 0; c < m; ++c) {
            if (mat[r][c] == 1) {
                // מפחיתים 1 מכיוון שכל ספירה כוללת את תא המרכז עצמו
                int k = min({L[r][c], R[r][c], U[r][c], D[r][c]}) - 1;
                if (k > best.arm) {
                    best = {r, c, k};
                }
            }
        }
    }

    return best;
}

#endif