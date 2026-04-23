#define NOMINMAX
#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <windows.h>
#include <climits>
#include <limits>

struct Stats {
    size_t comp_elem = 0;
    size_t comp_other = 0;
    size_t swaps = 0;
};

std::vector<int> generateSorted(int n) {
    std::vector<int> v(n);
    for (int i = 0; i < n; ++i) v[i] = i;
    return v;
}
std::vector<int> generateReversed(int n) {
    std::vector<int> v(n);
    for (int i = 0; i < n; ++i) v[i] = n - i;
    return v;
}
std::vector<int> generateRandom(int n) {
    std::vector<int> v(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, n);
    for (int i = 0; i < n; ++i) v[i] = dis(gen);
    return v;
}

void selectionSort(std::vector<int>& a, Stats& s) {
    int n = static_cast<int>(a.size());
    for (int i = 0; i < n - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < n; ++j) {
            s.comp_elem++;
            if (a[j] < a[min_idx]) min_idx = j;
        }
        if (min_idx != i) {
            std::swap(a[i], a[min_idx]);
            s.swaps++;
        }
    }
}

void bubbleSort(std::vector<int>& a, Stats& s) {
    int n = static_cast<int>(a.size());
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            s.comp_elem++;
            if (a[j] > a[j + 1]) {
                std::swap(a[j], a[j + 1]);
                s.swaps++;
            }
        }
    }
}

void improvedBubbleSort(std::vector<int>& a, Stats& s) {
    int n = static_cast<int>(a.size());
    bool swapped;
    for (int i = 0; i < n - 1; ++i) {
        swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            s.comp_elem++;
            if (a[j] > a[j + 1]) {
                std::swap(a[j], a[j + 1]);
                s.swaps++;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

void shakerSort(std::vector<int>& a, Stats& s) {
    int n = static_cast<int>(a.size());
    bool swapped = true;
    int start = 0, end = n - 1;
    while (swapped) {
        swapped = false;
        for (int i = start; i < end; ++i) {
            s.comp_elem++;
            if (a[i] > a[i + 1]) {
                std::swap(a[i], a[i + 1]);
                s.swaps++;
                swapped = true;
            }
        }
        if (!swapped) break;
        end--;
        for (int i = end - 1; i >= start; --i) {
            s.comp_elem++;
            if (a[i] > a[i + 1]) {
                std::swap(a[i], a[i + 1]);
                s.swaps++;
                swapped = true;
            }
        }
        start++;
    }
}

void insertionSort(std::vector<int>& a, Stats& s) {
    int n = static_cast<int>(a.size());
    for (int i = 1; i < n; ++i) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0) {
            s.comp_elem++;
            if (a[j] > key) {
                a[j + 1] = a[j];
                s.swaps++;
                j--;
            }
            else break;
        }
        a[j + 1] = key;
        if (j + 1 != i) s.swaps++;
    }
}

void barrierInsertionSort(std::vector<int>& a, Stats& s) {
    int n = static_cast<int>(a.size());
    int min_idx = 0;
    for (int i = 1; i < n; ++i) {
        s.comp_elem++;
        if (a[i] < a[min_idx]) min_idx = i;
    }
    if (min_idx != 0) {
        std::swap(a[0], a[min_idx]);
        s.swaps++;
    }
    for (int i = 2; i < n; ++i) {
        int key = a[i];
        int j = i - 1;
        while (true) {
            s.comp_elem++;
            if (a[j] > key) {
                a[j + 1] = a[j];
                s.swaps++;
                j--;
            }
            else break;
        }
        a[j + 1] = key;
        if (j + 1 != i) s.swaps++;
    }
}

void merge(std::vector<int>& a, int left, int mid, int right, Stats& s) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; ++i) L[i] = a[left + i];
    for (int i = 0; i < n2; ++i) R[i] = a[mid + 1 + i];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        s.comp_elem++;
        if (L[i] <= R[j]) {
            a[k++] = L[i++];
        }
        else {
            a[k++] = R[j++];
        }
        s.swaps++;
    }
    while (i < n1) { a[k++] = L[i++]; s.swaps++; }
    while (j < n2) { a[k++] = R[j++]; s.swaps++; }
}
void mergeSort(std::vector<int>& a, int left, int right, Stats& s) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(a, left, mid, s);
        mergeSort(a, mid + 1, right, s);
        merge(a, left, mid, right, s);
    }
}
void mergeSortWrapper(std::vector<int>& a, Stats& s) {
    mergeSort(a, 0, static_cast<int>(a.size()) - 1, s);
}

int partition(std::vector<int>& a, int low, int high, Stats& s) {
    int pivot = a[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        s.comp_elem++;
        if (a[j] <= pivot) {
            i++;
            std::swap(a[i], a[j]);
            s.swaps++;
        }
    }
    std::swap(a[i + 1], a[high]);
    s.swaps++;
    return i + 1;
}
void quickSort(std::vector<int>& a, int low, int high, Stats& s) {
    if (low < high) {
        int pi = partition(a, low, high, s);
        quickSort(a, low, pi - 1, s);
        quickSort(a, pi + 1, high, s);
    }
}
void quickSortWrapper(std::vector<int>& a, Stats& s) {
    quickSort(a, 0, static_cast<int>(a.size()) - 1, s);
}

void printSortingTable() {
    const int N = 100000;
    enum Type { SORTED, REVERSED, RANDOM };
    std::string typeNames[] = { "По неубыванию", "По невозрастанию", "Случайно" };

    std::cout << "\n===== СРАВНЕНИЕ АЛГОРИТМОВ СОРТИРОВКИ (размер " << N << ") =====\n\n";
    for (int t = 0; t < 3; ++t) {
        std::cout << "Исходный порядок: " << typeNames[t] << "\n";
        std::cout << std::left << std::setw(3) << "№"
            << std::setw(25) << "Алгоритм"
            << std::setw(15) << "Сравн. с эл."
            << std::setw(15) << "Сравн. др."
            << std::setw(15) << "Перестановки\n";
        std::cout << std::string(70, '-') << "\n";

        std::vector<int> original;
        if (t == SORTED) original = generateSorted(N);
        else if (t == REVERSED) original = generateReversed(N);
        else original = generateRandom(N);

        struct Algo { const char* name; void (*func)(std::vector<int>&, Stats&); };
        std::vector<Algo> algos = {
            {"Выбор", selectionSort},
            {"Пузырёк", bubbleSort},
            {"Пузырёк+флаг", improvedBubbleSort},
            {"Шейкерная", shakerSort},
            {"Вставки", insertionSort},
            {"Вставки+барьер", barrierInsertionSort},
            {"Слияние", mergeSortWrapper},
            {"Быстрая", quickSortWrapper}
        };

        for (size_t i = 0; i < algos.size(); ++i) {
            std::vector<int> arr = original;
            Stats s;
            algos[i].func(arr, s);
            std::cout << std::left << std::setw(3) << (i + 1)
                << std::setw(25) << algos[i].name
                << std::setw(15) << s.comp_elem
                << std::setw(15) << s.comp_other
                << std::setw(15) << s.swaps << "\n";
        }
        std::cout << "\n";
    }
}

double measureTime(void (*sortFunc)(std::vector<int>&, Stats&), std::vector<int> arr, int repeats = 10) {
    double total = 0.0;
    for (int i = 0; i < repeats; ++i) {
        std::vector<int> copy = arr;
        Stats dummy;
        auto start = std::chrono::high_resolution_clock::now();
        sortFunc(copy, dummy);
        auto end = std::chrono::high_resolution_clock::now();
        total += std::chrono::duration<double>(end - start).count();
    }
    return total / repeats;
}

void printTimeTable() {
    std::vector<int> lengths = { 100000, 150000, 200000, 300000, 400000, 500000 };
    std::cout << "\n===== ЗАВИСИМОСТЬ ВРЕМЕНИ ОТ ДЛИНЫ (случайный массив) =====\n\n";
    std::cout << std::left << std::setw(12) << "Длина";
    std::vector<std::string> algoNames = { "Выбор","Пузырёк","Пузырёк+","Шейкер","Вставки","Вставки+","Слияние","Быстрая" };
    for (const auto& name : algoNames) std::cout << std::setw(12) << name;
    std::cout << "\n" << std::string(12 + 12 * algoNames.size(), '-') << "\n";

    for (int n : lengths) {
        std::cout << std::left << std::setw(12) << n;
        std::vector<int> arr = generateRandom(n);
        for (auto& name : algoNames) {
            double t = 0.0;
            if (name == "Выбор") t = measureTime(selectionSort, arr, 3);
            else if (name == "Пузырёк") t = measureTime(bubbleSort, arr, 3);
            else if (name == "Пузырёк+") t = measureTime(improvedBubbleSort, arr, 3);
            else if (name == "Шейкер") t = measureTime(shakerSort, arr, 3);
            else if (name == "Вставки") t = measureTime(insertionSort, arr, 5);
            else if (name == "Вставки+") t = measureTime(barrierInsertionSort, arr, 5);
            else if (name == "Слияние") t = measureTime(mergeSortWrapper, arr, 10);
            else if (name == "Быстрая") t = measureTime(quickSortWrapper, arr, 10);
            std::cout << std::fixed << std::setprecision(4) << std::setw(12) << t;
        }
        std::cout << "\n";
    }
}

void taskMaxProduct() {
    std::ifstream in("maxproduct.txt");
    if (!in) {
        std::cerr << "Ошибка: файл maxproduct.txt не найден!\n";
        std::cerr << "Создайте файл с числами, например:\n4 1 0 3 2\n";
        return;
    }
    std::vector<int> arr;
    int x;
    while (in >> x) arr.push_back(x);
    if (arr.size() < 2) {
        std::cout << "В массиве меньше двух элементов, произведение не определить.\n";
        return;
    }
    int max1 = std::numeric_limits<int>::min();
    int max2 = std::numeric_limits<int>::min();
    int min1 = std::numeric_limits<int>::max();
    int min2 = std::numeric_limits<int>::max();

    for (int num : arr) {
        if (num > max1) { max2 = max1; max1 = num; }
        else if (num > max2) max2 = num;
        if (num < min1) { min2 = min1; min1 = num; }
        else if (num < min2) min2 = num;
    }
    long long prod1 = (long long)max1 * max2;
    long long prod2 = (long long)min1 * min2;
    long long ans = std::max(prod1, prod2);
    std::cout << "Максимальное произведение двух элементов: " << ans << "\n";
}

void runLab3() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    std::cout << "Лабораторная работа №3: Алгоритмы сортировки + Вариант 7\n";
    printSortingTable();
    printTimeTable();
    std::cout << "\n===== ВАРИАНТ 7: МАКСИМАЛЬНОЕ ПРОИЗВЕДЕНИЕ ДВУХ ЭЛЕМЕНТОВ =====\n";
    taskMaxProduct();
}