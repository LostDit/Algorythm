#ifndef COLUMN_PRODUCT_H
#define COLUMN_PRODUCT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>

inline std::vector<int> computeColumnProduct(const std::vector<std::vector<int>>& matrix) {
    int n = matrix.size();
    if (n == 0) return {};
    int m = matrix[0].size();
    std::vector<int> result(m, 0);
    for (int j = 0; j < m; ++j) {
        long long prod = 1;
        bool found = false;
        for (int i = 0; i < n; ++i) {
            int val = matrix[i][j];
            if (val > 0 && val % 2 == 0) {
                prod *= val;
                found = true;
            }
        }
        result[j] = found ? static_cast<int>(prod) : 0;
    }
    return result;
}

inline void run() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::ifstream fin("input.txt");
    if (!fin) {
        std::cerr << "Не удалось открыть файл input.txt" << std::endl;
        return;
    }

    int n, m;
    fin >> n >> m;
    std::vector<std::vector<int>> matrix(n, std::vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            fin >> matrix[i][j];
    fin.close();

    std::vector<int> result = computeColumnProduct(matrix);

    std::cout << "Результирующий массив:" << std::endl;
    for (int val : result)
        std::cout << val << " ";
    std::cout << std::endl;
}

#endif