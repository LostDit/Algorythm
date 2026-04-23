#define NOMINMAX
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <random>
#include <functional>
#include <windows.h>
#include <iomanip>
#include <sstream>

long long linearSearch(const std::string& text, const std::string& pattern) {
    long long cmp = 0;
    int n = text.size(), m = pattern.size();

    for (int i = 0; i <= n - m; i++) {
        int j = 0;
        while (j < m) {
            cmp++;
            if (text[i + j] != pattern[j]) break;
            j++;
        }
        if (j == m) return cmp;
    }
    return cmp;
}

long long boyerMoore(const std::string& text, const std::string& pattern) {
    long long cmp = 0;
    int n = text.size(), m = pattern.size();

    int* bad = new int[256];
    for (int i = 0; i < 256; i++) bad[i] = m;
    for (int i = 0; i < m - 1; i++)
        bad[(unsigned char)pattern[i]] = m - 1 - i;

    int i = 0;
    while (i <= n - m) {
        int j = m - 1;
        while (j >= 0) {
            cmp++;
            if (pattern[j] != text[i + j]) break;
            j--;
        }
        if (j < 0) {
            delete[] bad;
            return cmp;
        }
        i += bad[(unsigned char)text[i + m - 1]];
    }

    delete[] bad;
    return cmp;
}

long long kmp(const std::string& text, const std::string& pattern) {
    long long cmp = 0;
    int n = text.size(), m = pattern.size();

    int* lps = new int[m];
    lps[0] = 0;

    for (int i = 1, len = 0; i < m; ) {
        if (pattern[i] == pattern[len]) lps[i++] = ++len;
        else if (len) len = lps[len - 1];
        else lps[i++] = 0;
    }

    for (int i = 0, j = 0; i < n; ) {
        cmp++;
        if (text[i] == pattern[j]) {
            i++; j++;
            if (j == m) {
                delete[] lps;
                return cmp;
            }
        }
        else {
            if (j) j = lps[j - 1];
            else i++;
        }
    }

    delete[] lps;
    return cmp;
}

long long rabinKarp(const std::string& text, const std::string& pattern) {
    long long cmp = 0;
    const long long p = 31, mod = 1000000007;

    int n = text.size(), m = pattern.size();
    if (m > n) return 0;

    long long hashP = 0, hashT = 0, powP = 1;

    for (int i = 0; i < m; i++) {
        hashP = (hashP * p + pattern[i]) % mod;
        hashT = (hashT * p + text[i]) % mod;
        if (i < m - 1) powP = (powP * p) % mod;
    }

    for (int i = 0; i <= n - m; i++) {
        if (hashP == hashT) {
            for (int j = 0; j < m; j++) {
                cmp++;
                if (text[i + j] != pattern[j]) break;
                if (j == m - 1) return cmp;
            }
        }
        if (i < n - m) {
            hashT = (hashT - text[i] * powP % mod + mod) % mod;
            hashT = (hashT * p + text[i + m]) % mod;
        }
    }
    return cmp;
}

void generateLongString(const std::string& filename, const std::string& pattern) {
    const int N = 100000000;
    std::string s;
    s.reserve(N);

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(0, 61);

    auto rnd = [&](int x) {
        if (x < 26) return char('a' + x);
        if (x < 52) return char('A' + x - 26);
        return char('0' + x - 52);
        };

    for (int i = 0; i < N - (int)pattern.size(); i++)
        s.push_back(rnd(dist(rng)));

    s += pattern;

    std::ofstream out(filename, std::ios::binary);
    out << s;
}

double measureTime(const std::function<void()>& f) {
    auto t1 = std::chrono::high_resolution_clock::now();
    f();
    auto t2 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(t2 - t1).count();
}

void runLab() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    const std::string green = "\033[32m";
    const std::string red = "\033[31m";
    const std::string reset = "\033[0m";

    std::pair<std::string, std::string> tests[8] = {
        {"aaaaab",      "aab"},
        {"abcabcabc",   "abc"},
        {"zzzzzzzzzz",  "zzz"},
        {"abcdefg",     "fg"},
        {"ababababab",  "bab"},
        {"mississippi", "issi"},
        {"qwertyuiop",  "tyu"},
        {"aaaaaaaaab",  "aaab"}
    };

    // Заголовки (сокращённые)
    const std::string colHeaders[4] = { "Лин", "БМ", "КМП", "РК" };

    // Сначала вычислим все значения сравнений и сохраним их,
    // чтобы корректно подобрать ширины столбцов под данные и заголовки.
    long long results[8][4];
    for (int i = 0; i < 8; ++i) {
        results[i][0] = linearSearch(tests[i].first, tests[i].second);
        results[i][1] = boyerMoore(tests[i].first, tests[i].second);
        results[i][2] = kmp(tests[i].first, tests[i].second);
        results[i][3] = rabinKarp(tests[i].first, tests[i].second);
    }

    // Вычисляем ширины столбцов: учитываем длину заголовка и максимальную длину данных
    int colWidth[4];
    for (int j = 0; j < 4; ++j) {
        int maxDataLen = 0;
        for (int i = 0; i < 8; ++i) {
            int len = static_cast<int>(std::to_string(results[i][j]).size());
            if (len > maxDataLen) maxDataLen = len;
        }
        int headerLen = static_cast<int>(colHeaders[j].size());
        int base = std::max(headerLen, maxDataLen);
        // добавим небольшой запас для читаемости
        colWidth[j] = std::max(6, base + 4);
    }

    const int idxWidth = 4;

    std::cout << "Таблица сравнений символов:\n\n";

    // Вспомогательная: возвращает строку, выровненную в width (без цвета)
    auto paddedPlain = [&](const std::string& s, int width) {
        std::ostringstream oss;
        oss << std::left << std::setw(width) << s;
        return oss.str();
        };

    // Печать заголовка таблицы: используем ту же логику паддинга, что и для данных
    std::cout << std::left << std::setw(idxWidth) << "№";
    for (int j = 0; j < 4; ++j) {
        std::cout << paddedPlain(colHeaders[j], colWidth[j]);
    }
    std::cout << "\n";

    // Разделитель
    int totalWidth = idxWidth;
    for (int j = 0; j < 4; ++j) totalWidth += colWidth[j];
    std::cout << std::string(totalWidth, '-') << "\n";

    // Вспомогательная функция: формирует отформатированную (паддед) ячейку,
    // затем оборачивает её в цветовые коды (цвет внутри ячейки).
    auto makeCell = [&](long long x, int width) {
        std::string plain = std::to_string(x);
        std::ostringstream oss;
        oss << std::left << std::setw(width) << plain;
        std::string padded = oss.str();

        if (x < 10)  return green + padded + reset;
        if (x > 50)  return red + padded + reset;
        return padded;
        };

    // Печать строк таблицы
    for (int i = 0; i < 8; ++i) {
        std::cout << std::left << std::setw(idxWidth) << (i + 1);
        for (int j = 0; j < 4; ++j) {
            std::cout << makeCell(results[i][j], colWidth[j]);
        }
        std::cout << "\n";
    }

    std::cout << "\nГенерация длинной строки и замер времени...\n";

    const std::string filename = "long_string.txt";
    const std::string pattern = "AbC123xYzQ";

    generateLongString(filename, pattern);

    std::ifstream in(filename, std::ios::binary);
    std::string text((std::istreambuf_iterator<char>(in)),
        std::istreambuf_iterator<char>());

    double tLinear = 0.0, tBM = 0.0, tKMP = 0.0, tRK = 0.0;
    const int runs = 10;

    for (int r = 0; r < runs; ++r) {
        tLinear += measureTime([&]() { linearSearch(text, pattern); });
        tBM += measureTime([&]() { boyerMoore(text, pattern); });
        tKMP += measureTime([&]() { kmp(text, pattern); });
        tRK += measureTime([&]() { rabinKarp(text, pattern); });
    }

    tLinear /= runs;
    tBM /= runs;
    tKMP /= runs;
    tRK /= runs;

    // Таблица времени
    std::cout << "\nТаблица времени (усреднено по " << runs << " запусков):\n\n";

    // Подготовим ширины для таблицы времени: имя алгоритма и время
    const int nameWidth = 30;
    const int timeWidth = 14;

    // Найдём минимальную ширину для отображения времени с нужной точностью
    std::ostringstream tmp;
    tmp << std::fixed << std::setprecision(6) << tLinear;
    int timeStrLen = static_cast<int>(tmp.str().size());
    int effectiveTimeWidth = std::max(timeWidth, timeStrLen + 2);

    std::cout << std::left << std::setw(nameWidth) << "Алгоритм"
        << std::setw(effectiveTimeWidth) << "Время, сек" << "\n";
    std::cout << std::string(nameWidth + effectiveTimeWidth, '-') << "\n";

    auto timeCell = [&](const std::string& name, double t) {
        std::ostringstream ossTime;
        ossTime << std::fixed << std::setprecision(6) << t;
        std::string timeStr = ossTime.str();

        std::ostringstream oss;
        oss << std::left << std::setw(nameWidth) << name
            << std::setw(effectiveTimeWidth) << timeStr;
        std::cout << oss.str() << "\n";
        };

    timeCell("Линейный", tLinear);
    timeCell("Бойера-Мура", tBM);
    timeCell("Кнута-Морриса-Пратта", tKMP);
    timeCell("Карпа-Рабина", tRK);

    std::cout << "\nГотово.\n";
}