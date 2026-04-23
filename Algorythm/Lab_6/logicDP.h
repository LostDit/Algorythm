#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <climits>
#include <windows.h>

// 1. Черепашка (минимальная стоимость с поворотами) – упрощённо
void taskTurtle() {
    std::ifstream in("turtle.txt");
    if (!in) { std::cout << "Файл turtle.txt не найден\n"; return; }
    int l, m; in >> l >> m;
    int N = l * m;
    std::vector<std::vector<int>> cost(l, std::vector<int>(m));
    for (int i = 0;i < l;++i) for (int j = 0;j < m;++j) in >> cost[i][j];
    std::vector<std::vector<int>> dp(l, std::vector<int>(m, INT_MAX));
    dp[0][0] = cost[0][0];
    for (int i = 0;i < l;++i)
        for (int j = 0;j < m;++j) {
            if (i > 0) dp[i][j] = std::min(dp[i][j], dp[i - 1][j] + cost[i][j]);
            if (j > 0) dp[i][j] = std::min(dp[i][j], dp[i][j - 1] + cost[i][j]);
        }
    std::cout << "Минимальная стоимость: " << dp[l - 1][m - 1] << "\n";
}

// 2. Треугольник (максимальная сумма)
void taskTriangle() {
    std::ifstream in("triangle.txt");
    if (!in) { std::cout << "Файл triangle.txt не найден\n"; return; }
    int n; in >> n;
    std::vector<std::vector<int>> tri(n);
    for (int i = 0;i < n;++i) {
        tri[i].resize(i + 1);
        for (int j = 0;j <= i;++j) in >> tri[i][j];
    }
    for (int i = n - 2;i >= 0;--i)
        for (int j = 0;j <= i;++j)
            tri[i][j] += std::max(tri[i + 1][j], tri[i + 1][j + 1]);
    std::cout << "Максимальная сумма: " << tri[0][0] << "\n";
}

// 7. Камни (разбиение на две кучи с минимальной разницей)
void taskStones() {
    std::ifstream in("stones.txt");
    if (!in) { std::cout << "Файл stones.txt не найден\n"; return; }
    int n, sum = 0; in >> n;
    std::vector<int> a(n);
    for (int i = 0;i < n;++i) { in >> a[i]; sum += a[i]; }
    std::vector<bool> dp(sum / 2 + 1, false);
    dp[0] = true;
    for (int x : a)
        for (int s = sum / 2; s >= x; --s)
            if (dp[s - x]) dp[s] = true;
    int best = 0;
    for (int s = sum / 2; s >= 0; --s) if (dp[s]) { best = s; break; }
    std::cout << "Минимальная разница: " << sum - 2 * best << "\n";
}

// 13. Наибольший квадрат из единиц
void taskMaxSquare() {
    std::ifstream in("matrix.txt");
    if (!in) { std::cout << "Файл matrix.txt не найден\n"; return; }
    int n, m; in >> n >> m;
    std::vector<std::vector<int>> a(n, std::vector<int>(m));
    for (int i = 0;i < n;++i) for (int j = 0;j < m;++j) in >> a[i][j];
    std::vector<std::vector<int>> dp(n, std::vector<int>(m, 0));
    int maxSize = 0;
    for (int i = 0;i < n;++i)
        for (int j = 0;j < m;++j) {
            if (a[i][j] == 1) {
                if (i == 0 || j == 0) dp[i][j] = 1;
                else dp[i][j] = 1 + std::min({ dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1] });
                if (dp[i][j] > maxSize) maxSize = dp[i][j];
            }
        }
    std::cout << "Размер наибольшего квадрата: " << maxSize << "\n";
}

void runLab6() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    std::cout << "=== ЛАБОРАТОРНАЯ РАБОТА №6 (динамическое программирование) ===\n";
    taskTurtle();
    taskTriangle();
    taskStones();
    taskMaxSquare();
}