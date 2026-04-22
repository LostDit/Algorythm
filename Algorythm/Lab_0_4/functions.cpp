#include "functions.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <limits>
#include <set>

// Ввод положительного целого числа с проверкой
int inputPositiveInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value <= 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка: необходимо положительное целое число.\n";
        }
        else {
            return value;
        }
    }
}

// Заполнение матрицы случайными числами в диапазоне [-8, 26]
void fillRandom(std::vector<std::vector<int>>& matrix) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-8, 26);

    for (auto& row : matrix) {
        for (auto& val : row) {
            val = dist(gen);
        }
    }
}

// Вывод матрицы
void printMatrix(const std::vector<std::vector<int>>& matrix, const std::string& title) {
    if (!title.empty())
        std::cout << title << ":\n";
    for (const auto& row : matrix) {
        for (int val : row)
            std::cout << val << '\t';
        std::cout << '\n';
    }
    std::cout << '\n';
}

// Проверка наличия строки с ровно двумя отрицательными элементами
bool hasRowWithTwoNegatives(const std::vector<std::vector<int>>& matrix) {
    for (const auto& row : matrix) {
        int count = 0;
        for (int val : row) {
            if (val < 0) ++count;
        }
        if (count == 2) return true;
    }
    return false;
}

// Замена максимальных элементов каждой строки на противоположные по знаку
// и запоминание столбцов, в которых была замена (для вставки)
void replaceMaxElements(std::vector<std::vector<int>>& matrix, std::vector<int>& columnsWithMax) {
    std::set<int> colSet; // используем set, чтобы избежать дубликатов

    for (auto& row : matrix) {
        int maxVal = *std::max_element(row.begin(), row.end());
        for (size_t j = 0; j < row.size(); ++j) {
            if (row[j] == maxVal) {
                row[j] = -row[j];
                colSet.insert(static_cast<int>(j));
            }
        }
    }

    columnsWithMax.assign(colSet.begin(), colSet.end());
}

// Вставка нулевых столбцов после указанных столбцов
void insertZeroColumns(std::vector<std::vector<int>>& matrix, const std::vector<int>& columns) {
    // Сортируем по убыванию, чтобы вставка не сбивала индексы
    std::vector<int> sorted = columns;
    std::sort(sorted.begin(), sorted.end(), std::greater<int>());

    for (int col : sorted) {
        for (auto& row : matrix) {
            // Вставляем после столбца col
            row.insert(row.begin() + col + 1, 0);
        }
    }
}

// Удаление средней строки (если количество строк нечётное)
void removeMiddleRow(std::vector<std::vector<int>>& matrix) {
    size_t n = matrix.size();
    if (n % 2 == 1) {
        matrix.erase(matrix.begin() + n / 2);
    }
}

// Обмен средних столбцов (если количество столбцов чётное)
void swapMiddleColumns(std::vector<std::vector<int>>& matrix) {
    if (matrix.empty()) return;
    size_t m = matrix[0].size();
    if (m % 2 == 0) {
        size_t mid = m / 2;
        for (auto& row : matrix) {
            std::swap(row[mid - 1], row[mid]);
        }
    }
}

void task5_variant1() {
    std::cout << "Лабораторная работа 0, вариант 1, задача 5\n";
    std::cout << "--------------------------------------------\n";

    int n = inputPositiveInt("Введите количество строк n: ");
    int m = inputPositiveInt("Введите количество столбцов m: ");

    std::vector<std::vector<int>> matrix(n, std::vector<int>(m));
    fillRandom(matrix);
    printMatrix(matrix, "Исходная матрица");

    if (hasRowWithTwoNegatives(matrix)) {
        std::cout << "Найдена строка с ровно двумя отрицательными элементами.\n";
        std::cout << "Выполняем замену максимальных элементов каждой строки "
            "и вставку нулевых столбцов.\n\n";

        std::vector<int> columnsWithMax;
        replaceMaxElements(matrix, columnsWithMax);
        printMatrix(matrix, "После замены максимальных элементов");

        if (!columnsWithMax.empty()) {
            insertZeroColumns(matrix, columnsWithMax);
            printMatrix(matrix, "После вставки нулевых столбцов");
        }
        else {
            std::cout << "Нет столбцов для вставки.\n";
        }
    }
    else {
        std::cout << "Нет строки с ровно двумя отрицательными элементами.\n";
        std::cout << "Выполняем удаление средней строки (если n нечётное) "
            "и обмен средних столбцов (если m чётное).\n\n";

        removeMiddleRow(matrix);
        if (matrix.size() != static_cast<size_t>(n)) {
            printMatrix(matrix, "После удаления средней строки");
        }

        swapMiddleColumns(matrix);
        if (!matrix.empty() && matrix[0].size() != static_cast<size_t>(m)) {
            printMatrix(matrix, "После обмена средних столбцов");
        }
        else if (!matrix.empty() && matrix[0].size() == static_cast<size_t>(m) && m % 2 == 0) {
            printMatrix(matrix, "После обмена средних столбцов");
        }
    }

    std::cout << "Итоговая матрица:\n";
    printMatrix(matrix, "");
}