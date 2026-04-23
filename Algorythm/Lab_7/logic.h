#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <windows.h>

// 1. Магический квадрат 3x3 (полный перебор)
bool isMagic(const std::vector<int>& sq) {
    int sum = sq[0] + sq[1] + sq[2];
    for (int i = 3;i < 9;i += 3) if (sq[i] + sq[i + 1] + sq[i + 2] != sum) return false;
    for (int i = 0;i < 3;++i) if (sq[i] + sq[i + 3] + sq[i + 6] != sum) return false;
    if (sq[0] + sq[4] + sq[8] != sum) return false;
    if (sq[2] + sq[4] + sq[6] != sum) return false;
    return true;
}
void findMagicSquares() {
    std::vector<int> nums = { 1,2,3,4,5,6,7,8,9 };
    int count = 0;
    do {
        if (isMagic(nums)) {
            count++;
            std::cout << "Магический квадрат:\n";
            for (int i = 0;i < 9;++i) {
                std::cout << nums[i] << " ";
                if (i % 3 == 2) std::cout << "\n";
            }
            std::cout << "\n";
        }
    } while (std::next_permutation(nums.begin(), nums.end()));
    std::cout << "Всего найдено: " << count << "\n";
}

// 2. Преобразуй и властвуй (расстановка чисел в неравенства)
bool solveInequalities(std::vector<int>& nums, std::vector<int>& res, const std::string& signs, int idx, std::vector<bool>& used) {
    if (idx == (int)signs.size() + 1) return true;
    for (int i = 0;i < (int)nums.size();++i) {
        if (!used[i]) {
            if (idx > 0) {
                if (signs[idx - 1] == '<' && res[idx - 1] >= nums[i]) continue;
                if (signs[idx - 1] == '>' && res[idx - 1] <= nums[i]) continue;
            }
            res[idx] = nums[i];
            used[i] = true;
            if (solveInequalities(nums, res, signs, idx + 1, used)) return true;
            used[i] = false;
        }
    }
    return false;
}
void taskInequalities() {
    std::vector<int> nums = { 2,5,1,0 };
    std::string signs = "<><";
    std::vector<int> res(signs.size() + 1);
    std::vector<bool> used(nums.size(), false);
    if (solveInequalities(nums, res, signs, 0, used)) {
        std::cout << "Решение: ";
        for (int x : res) std::cout << x << " ";
        std::cout << "\n";
    }
    else std::cout << "Нет решения\n";
}

// 3. Интервальное разбиение (жадный алгоритм)
struct Interval { int start, end; };
bool operator<(const Interval& a, const Interval& b) { return a.start < b.start; }
int minResources(std::vector<Interval>& intervals) {
    std::sort(intervals.begin(), intervals.end());
    std::vector<int> endTimes;
    for (auto& inv : intervals) {
        bool placed = false;
        for (int& et : endTimes) {
            if (et <= inv.start) {
                et = inv.end;
                placed = true;
                break;
            }
        }
        if (!placed) endTimes.push_back(inv.end);
    }
    return endTimes.size();
}
void taskIntervalPartition() {
    std::vector<Interval> intervals = { {1,3},{2,4},{3,5},{4,6} };
    std::cout << "Минимальное количество ресурсов: " << minResources(intervals) << "\n";
}

// 4-5. Поиск k-ой статистики (линейное в среднем и в худшем)
int partition(std::vector<int>& a, int l, int r) {
    int pivot = a[r];
    int i = l - 1;
    for (int j = l;j < r;++j) if (a[j] <= pivot) std::swap(a[++i], a[j]);
    std::swap(a[i + 1], a[r]);
    return i + 1;
}
int quickSelect(std::vector<int>& a, int l, int r, int k) {
    if (l == r) return a[l];
    int p = partition(a, l, r);
    if (k == p) return a[p];
    else if (k < p) return quickSelect(a, l, p - 1, k);
    else return quickSelect(a, p + 1, r, k);
}
int kthSmallestAvg(std::vector<int> a, int k) {
    return quickSelect(a, 0, a.size() - 1, k);
}
// Медиана медиан (BFPRT) – для гарантии O(n)
int medianOfMedians(std::vector<int>& a, int l, int r, int k);
int medianOfMedians(std::vector<int>& a, int l, int r, int k) {
    if (r - l + 1 <= 5) {
        std::sort(a.begin() + l, a.begin() + r + 1);
        return a[l + k];
    }
    std::vector<int> medians;
    for (int i = l;i <= r;i += 5) {
        int right = std::min(i + 4, r);
        std::sort(a.begin() + i, a.begin() + right + 1);
        medians.push_back(a[i + (right - i) / 2]);
    }
    int median = medianOfMedians(medians, 0, medians.size() - 1, medians.size() / 2);
    // найти позицию median в a
    int pos = l;
    while (a[pos] != median) pos++;
    std::swap(a[pos], a[r]);
    int p = partition(a, l, r);
    if (k == p - l) return median;
    else if (k < p - l) return medianOfMedians(a, l, p - 1, k);
    else return medianOfMedians(a, p + 1, r, k - (p - l + 1));
}
int kthSmallestWorst(std::vector<int> a, int k) {
    return medianOfMedians(a, 0, a.size() - 1, k);
}

// 6. k-я порядковая статистика в двух отсортированных массивах (3 способа)
int kthNaive(const std::vector<int>& a, const std::vector<int>& b, int k) {
    std::vector<int> merged;
    merged.reserve(a.size() + b.size());
    merged.insert(merged.end(), a.begin(), a.end());
    merged.insert(merged.end(), b.begin(), b.end());
    std::sort(merged.begin(), merged.end());
    return merged[k - 1];
}
int kthTwoPointers(const std::vector<int>& a, const std::vector<int>& b, int k) {
    int i = 0, j = 0;
    while (--k) {
        if (i < (int)a.size() && (j >= (int)b.size() || a[i] <= b[j])) i++;
        else j++;
    }
    if (i < (int)a.size() && (j >= (int)b.size() || a[i] <= b[j])) return a[i];
    else return b[j];
}
int kthBinarySearch(const std::vector<int>& a, const std::vector<int>& b, int k) {
    // бинарный поиск по меньшему массиву
    if (a.size() > b.size()) return kthBinarySearch(b, a, k);
    int low = std::max(0, k - (int)b.size()), high = std::min(k, (int)a.size());
    while (low <= high) {
        int cutA = (low + high) / 2;
        int cutB = k - cutA;
        int leftA = (cutA == 0) ? INT_MIN : a[cutA - 1];
        int rightA = (cutA == (int)a.size()) ? INT_MAX : a[cutA];
        int leftB = (cutB == 0) ? INT_MIN : b[cutB - 1];
        int rightB = (cutB == (int)b.size()) ? INT_MAX : b[cutB];
        if (leftA <= rightB && leftB <= rightA) return std::max(leftA, leftB);
        else if (leftA > rightB) high = cutA - 1;
        else low = cutA + 1;
    }
    return -1;
}
void taskKthTwoArrays() {
    std::vector<int> a = { 1,3,5,7,9 };
    std::vector<int> b = { 2,4,6,8,10 };
    int k = 5;
    std::cout << "k=" << k << ": Naive=" << kthNaive(a, b, k)
        << ", TwoPtr=" << kthTwoPointers(a, b, k)
        << ", Binary=" << kthBinarySearch(a, b, k) << "\n";
}

void runLab7() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    std::cout << "=== ЛАБОРАТОРНАЯ РАБОТА №7 ===\n";
    findMagicSquares();
    taskInequalities();
    taskIntervalPartition();
    std::vector<int> arr = { 7, 10, 4, 3, 20, 15 };
    int k = 3;
    std::cout << k << "-я статистика (средн.): " << kthSmallestAvg(arr, k - 1) << "\n";
    std::cout << k << "-я статистика (худш.): " << kthSmallestWorst(arr, k - 1) << "\n";
    taskKthTwoArrays();
}