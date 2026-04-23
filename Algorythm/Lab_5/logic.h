#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>
#include <climits>
#include <windows.h>

// 1. Лабиринт (BFS)
struct Point { int r, c; };
int bfs(const std::vector<std::vector<int>>& maze, Point start, Point end) {
    int n = maze.size(), m = maze[0].size();
    std::vector<std::vector<int>> dist(n, std::vector<int>(m, -1));
    std::queue<Point> q;
    dist[start.r][start.c] = 0;
    q.push(start);
    int dr[] = { 0,1,0,-1 }, dc[] = { 1,0,-1,0 };
    while (!q.empty()) {
        Point cur = q.front(); q.pop();
        if (cur.r == end.r && cur.c == end.c) return dist[cur.r][cur.c];
        for (int i = 0; i < 4; ++i) {
            int nr = cur.r + dr[i], nc = cur.c + dc[i];
            if (nr >= 0 && nr < n && nc >= 0 && nc < m && maze[nr][nc] == 0 && dist[nr][nc] == -1) {
                dist[nr][nc] = dist[cur.r][cur.c] + 1;
                q.push({ nr, nc });
            }
        }
    }
    return -1;
}
void taskMaze() {
    std::ifstream in("maze.txt");
    if (!in) { std::cout << "Файл maze.txt не найден\n"; return; }
    int n, m; in >> n >> m;
    std::vector<std::vector<int>> maze(n, std::vector<int>(m));
    for (int i = 0;i < n;++i) for (int j = 0;j < m;++j) in >> maze[i][j];
    Point start, end; in >> start.r >> start.c >> end.r >> end.c;
    int len = bfs(maze, start, end);
    if (len != -1) std::cout << "Кратчайший путь: " << len << "\n";
    else std::cout << "Пути нет\n";
}

// 2. Рюкзак (неограниченные предметы) – динамическое программирование
void knapsackUnbounded() {
    std::ifstream in("knapsack.txt");
    if (!in) { std::cout << "Файл knapsack.txt не найден\n"; return; }
    int W, n; in >> W >> n;
    std::vector<int> w(n), v(n);
    for (int i = 0;i < n;++i) in >> w[i] >> v[i];
    std::vector<int> dp(W + 1, 0);
    for (int cap = 0; cap <= W; ++cap)
        for (int i = 0;i < n;++i)
            if (w[i] <= cap)
                dp[cap] = std::max(dp[cap], dp[cap - w[i]] + v[i]);
    std::cout << "Максимальная стоимость: " << dp[W] << "\n";
}

// 3. Коммивояжёр (перебор)
int tsp(const std::vector<std::vector<int>>& graph, int pos, int visited, std::vector<std::vector<int>>& memo) {
    int n = graph.size();
    if (visited == (1 << n) - 1) return graph[pos][0];
    if (memo[pos][visited] != -1) return memo[pos][visited];
    int ans = INT_MAX;
    for (int nxt = 0; nxt < n; ++nxt) {
        if (!(visited & (1 << nxt))) {
            int newCost = graph[pos][nxt] + tsp(graph, nxt, visited | (1 << nxt), memo);
            ans = std::min(ans, newCost);
        }
    }
    return memo[pos][visited] = ans;
}
void taskTSP() {
    std::ifstream in("tsp.txt");
    if (!in) { std::cout << "Файл tsp.txt не найден\n"; return; }
    int n; in >> n;
    std::vector<std::vector<int>> graph(n, std::vector<int>(n));
    for (int i = 0;i < n;++i) for (int j = 0;j < n;++j) in >> graph[i][j];
    std::vector<std::vector<int>> memo(n, std::vector<int>(1 << n, -1));
    int result = tsp(graph, 0, 1, memo);
    std::cout << "Минимальная длина маршрута: " << result << "\n";
}

// 4. Судоку (поиск всех решений)
bool isValidSudoku(const std::vector<std::vector<int>>& board, int row, int col, int num) {
    for (int i = 0;i < 9;++i) if (board[row][i] == num || board[i][col] == num) return false;
    int sr = row / 3 * 3, sc = col / 3 * 3;
    for (int i = 0;i < 3;++i) for (int j = 0;j < 3;++j) if (board[sr + i][sc + j] == num) return false;
    return true;
}
void solveSudoku(std::vector<std::vector<int>>& board, std::vector<std::vector<std::vector<int>>>& solutions) {
    for (int i = 0;i < 9;++i) {
        for (int j = 0;j < 9;++j) {
            if (board[i][j] == 0) {
                for (int num = 1;num <= 9;++num) {
                    if (isValidSudoku(board, i, j, num)) {
                        board[i][j] = num;
                        solveSudoku(board, solutions);
                        board[i][j] = 0;
                    }
                }
                return;
            }
        }
    }
    solutions.push_back(board);
}
void taskSudoku() {
    std::ifstream in("sudoku.txt");
    if (!in) { std::cout << "Файл sudoku.txt не найден\n"; return; }
    std::vector<std::vector<int>> board(9, std::vector<int>(9));
    for (int i = 0;i < 9;++i) for (int j = 0;j < 9;++j) in >> board[i][j];
    std::vector<std::vector<std::vector<int>>> solutions;
    solveSudoku(board, solutions);
    std::cout << "Найдено решений: " << solutions.size() << "\n";
    if (!solutions.empty()) {
        std::cout << "Первое решение:\n";
        for (int i = 0;i < 9;++i) {
            for (int j = 0;j < 9;++j) std::cout << solutions[0][i][j] << " ";
            std::cout << "\n";
        }
    }
}

void runLab5() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    std::cout << "Лабараторная работа №5\n";
    taskMaze();
    knapsackUnbounded();
    taskTSP();
    taskSudoku();
}