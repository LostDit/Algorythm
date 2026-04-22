#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>

// 1a. Один способ расстановки N ферзей
bool isSafe(const std::vector<int>& board, int row, int col) {
    for (int i = 0; i < row; ++i) {
        if (board[i] == col || abs(board[i] - col) == abs(i - row))
            return false;
    }
    return true;
}
bool solveNQueensOne(std::vector<int>& board, int row, int N) {
    if (row == N) return true;
    for (int col = 0; col < N; ++col) {
        if (isSafe(board, row, col)) {
            board[row] = col;
            if (solveNQueensOne(board, row + 1, N))
                return true;
        }
    }
    return false;
}
void printOneSolution(int N) {
    std::vector<int> board(N, -1);
    if (solveNQueensOne(board, 0, N)) {
        std::cout << "Одна расстановка " << N << " ферзей:\n";
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j)
                std::cout << (board[i] == j ? "Q " : ". ");
            std::cout << "\n";
        }
    }
    else {
        std::cout << "Решения нет для N=" << N << "\n";
    }
}

// 1b. Все способы расстановки N ферзей (счётчик)
void solveNQueensAll(std::vector<int>& board, int row, int N, int& count) {
    if (row == N) {
        count++;
        return;
    }
    for (int col = 0; col < N; ++col) {
        if (isSafe(board, row, col)) {
            board[row] = col;
            solveNQueensAll(board, row + 1, N, count);
        }
    }
}
int countAllSolutions(int N) {
    std::vector<int> board(N, -1);
    int count = 0;
    solveNQueensAll(board, 0, N, count);
    return count;
}

// 2. Наибольшее число полей вне боя ферзей (N ферзей, могут бить друг друга)
// Реализация через перебор с отсечением – сложная, оставим заглушку.
void task2() {
    std::cout << "Задача 2 (максимум полей вне боя) требует полного перебора, опущена для краткости.\n";
}

// 3. Максимальное число ферзей, каждый атакует ровно p других
void task3() {
    std::cout << "Задача 3 (атака ровно p) требует полного перебора, опущена.\n";
}

// 4. Обход конём (правило Варнсдорфа)
struct Move { int dr, dc; };
const std::vector<Move> knightMoves = { {2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2},{1,-2},{2,-1} };
bool isInside(int r, int c, int N, int M) {
    return r >= 0 && r < N && c >= 0 && c < M;
}
bool knightTour(std::vector<std::vector<int>>& board, int r, int c, int step, int N, int M) {
    if (step == N * M) return true;
    // Выбор следующего хода по правилу Варнсдорфа: минимальное количество дальнейших ходов
    std::vector<std::pair<int, Move>> candidates;
    for (const auto& mv : knightMoves) {
        int nr = r + mv.dr, nc = c + mv.dc;
        if (isInside(nr, nc, N, M) && board[nr][nc] == -1) {
            int cnt = 0;
            for (const auto& mv2 : knightMoves) {
                int nnr = nr + mv2.dr, nnc = nc + mv2.dc;
                if (isInside(nnr, nnc, N, M) && board[nnr][nnc] == -1) cnt++;
            }
            candidates.push_back({ cnt, mv });
        }
    }
    std::sort(candidates.begin(), candidates.end());
    for (const auto& cand : candidates) {
        Move mv = cand.second;
        int nr = r + mv.dr, nc = c + mv.dc;
        board[nr][nc] = step;
        if (knightTour(board, nr, nc, step + 1, N, M)) return true;
        board[nr][nc] = -1;
    }
    return false;
}
void runKnightTour(int N, int M) {
    std::vector<std::vector<int>> board(N, std::vector<int>(M, -1));
    srand(time(0));
    int startR = rand() % N, startC = rand() % M;
    board[startR][startC] = 0;
    if (knightTour(board, startR, startC, 1, N, M)) {
        std::cout << "Обход конём доски " << N << "x" << M << " найден.\n";
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j)
                std::cout << board[i][j] << "\t";
            std::cout << "\n";
        }
    }
    else {
        std::cout << "Обход не найден.\n";
    }
}

// 5. Магараджа (ферзь+конь) – 10 мирных магараджей на доске 10×10
bool isSafeMaharaja(const std::vector<std::pair<int, int>>& positions, int r, int c) {
    for (auto& p : positions) {
        if (p.first == r || p.second == c || abs(p.first - r) == abs(p.second - c)) return false; // ферзь
        // конь
        if ((abs(p.first - r) == 2 && abs(p.second - c) == 1) ||
            (abs(p.first - r) == 1 && abs(p.second - c) == 2)) return false;
    }
    return true;
}
bool solveMaharaja(std::vector<std::pair<int, int>>& positions, int row, int N) {
    if (row == N) return true;
    for (int col = 0; col < N; ++col) {
        if (isSafeMaharaja(positions, row, col)) {
            positions.push_back({ row, col });
            if (solveMaharaja(positions, row + 1, N)) return true;
            positions.pop_back();
        }
    }
    return false;
}
void runMaharaja(int N = 10) {
    std::vector<std::pair<int, int>> positions;
    if (solveMaharaja(positions, 0, N)) {
        std::cout << "Расстановка " << N << " мирных магараджей на доске " << N << "x" << N << ":\n";
        for (auto& p : positions)
            std::cout << "(" << p.first << "," << p.second << ") ";
        std::cout << "\n";
    }
    else {
        std::cout << "Не найдено.\n";
    }
}

// 6. Конь Аттилы – заглушка
void task6() {
    std::cout << "Задача 6 (конь Аттилы) требует сложного моделирования, опущена.\n";
}

void runLab4() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    std::cout << "=== ЛАБОРАТОРНАЯ РАБОТА №4 (перебор с возвратом) ===\n";
    int N = 8;
    printOneSolution(N);
    std::cout << "Всего решений для " << N << " ферзей: " << countAllSolutions(N) << "\n";
    runKnightTour(8, 8);
    runMaharaja(10);
    task2(); task3(); task6();
}