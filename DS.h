// DS.h
#ifndef DS_H
#define DS_H

#include <cassert>

class Matrix {
private:
    int m;
    int** data;
    mutable int accessCount;

    Matrix(const Matrix&);
    Matrix& operator=(const Matrix&);

public:
    Matrix(int m, int** values) : m(m), accessCount(0) {
        data = new int*[m];
        for (int i = 0; i < m; i++) {
            data[i] = new int[m];
            for (int j = 0; j < m; j++)
                data[i][j] = values[i][j];
        }
    }

    ~Matrix() {
        for (int i = 0; i < m; i++)
            delete[] data[i];
        delete[] data;
    }

    int get(int r, int c) const {
        assert(r >= 0 && r < m && c >= 0 && c < m);
        accessCount++;
        return data[r][c];
    }

    int getSize()        const { return m; }
    int getAccessCount() const { return accessCount; }
    void resetAccessCount()    { accessCount = 0; }
};

#endif