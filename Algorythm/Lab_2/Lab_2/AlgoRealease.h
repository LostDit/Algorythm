#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <windows.h>

//1. Линейный поиск
int linearSearch(const std::string& text, const std::string& pattern, size_t& comparisons) {
    comparisons = 0;
    int n = text.size();
    int m = pattern.size();
    if (m == 0) return 0;

    for (int i = 0; i <= n - m; ++i) {
        int j = 0;
        while (j < m) {
            comparisons++;
            if (text[i + j] != pattern[j]) break;
            j++;
        }
        if (j == m) return i;
    }
    return -1;
}

//2. Бойер-Мур
int boyerMoore(const std::string& text, const std::string& pattern, size_t& comparisons) {
    comparisons = 0;
    int n = text.size();
    int m = pattern.size();
    if (m == 0) return 0;
    if (m > n) return -1;

    const int ALPHABET = 256;
    std::vector<int> badChar(ALPHABET, m);
    for (int i = 0; i < m - 1; ++i) {
        badChar[(unsigned char)pattern[i]] = m - 1 - i;
    }

    int shift = 0;
    while (shift <= n - m) {
        int j = m - 1;
        while (j >= 0) {
            comparisons++;
            if (text[shift + j] != pattern[j]) break;
            j--;
        }
        if (j < 0) return shift;

        int stopChar = (unsigned char)text[shift + j];
        int step = badChar[stopChar] - (m - 1 - j);
        if (step < 1) step = 1;
        shift += step;
    }
    return -1;
}

//3. Кнут-Моррис-Пратт
std::vector<int> buildPrefixFunction(const std::string& pattern, size_t& comparisons) {
    int m = pattern.size();
    std::vector<int> pi(m, 0);
    for (int i = 1; i < m; ++i) {
        int j = pi[i - 1];
        while (j > 0 && pattern[i] != pattern[j]) {
            comparisons++;
            j = pi[j - 1];
        }
        if (pattern[i] == pattern[j]) {
            comparisons++;
            j++;
        }
        pi[i] = j;
    }
    return pi;
}

int kmpSearch(const std::string& text, const std::string& pattern, size_t& comparisons) {
    comparisons = 0;
    int n = text.size();
    int m = pattern.size();
    if (m == 0) return 0;
    if (m > n) return -1;

    std::vector<int> pi = buildPrefixFunction(pattern, comparisons);

    int j = 0;
    for (int i = 0; i < n; ++i) {
        while (j > 0 && text[i] != pattern[j]) {
            comparisons++;
            j = pi[j - 1];
        }
        if (text[i] == pattern[j]) {
            comparisons++;
            j++;
        }
        if (j == m) return i - m + 1;
    }
    return -1;
}

//4. Карп-Рабин
int rabinKarp(const std::string& text, const std::string& pattern, size_t& comparisons) {
    comparisons = 0;
    int n = text.size();
    int m = pattern.size();
    if (m == 0) return 0;
    if (m > n) return -1;

    const unsigned long long BASE = 91138233;
    unsigned long long hashPattern = 0, hashText = 0, powBase = 1;

    for (int i = 0; i < m; ++i) {
        hashPattern = hashPattern * BASE + (unsigned char)pattern[i];
        hashText = hashText * BASE + (unsigned char)text[i];
        if (i < m - 1) powBase *= BASE;
    }

    for (int i = 0; i <= n - m; ++i) {
        if (hashPattern == hashText) {
            bool match = true;
            for (int j = 0; j < m; ++j) {
                comparisons++;
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) return i;
        }
        if (i < n - m) {
            hashText = (hashText - (unsigned char)text[i] * powBase) * BASE
                + (unsigned char)text[i + m];
        }
    }
    return -1;
}

//Вспомогательные функции для таблицы
struct Example {
    std::string text;
    std::string pattern;
    std::string comment;
};

std::vector<Example> createExamples() {
    return {
        {"Hello world! This is a test string", "world", "Подстрока в середине"},
        {"abcdefghijklmnopqrstuvwxyz", "xyz", "Подстрока в конце"},
        {"This string does not contain the substring", "absent", "Подстроки нет"},
        {"AAAAAAAAAAAAAAAAAAAAAAAAAAAA", "AAA", "Повторяющиеся символы (выигрывает KMP)"},
        {"short", "sh", "Короткая подстрока"},
        {"ABCDEFGHIJKLMNOPQRSTUVWXYZ", "KLMN", "Разные символы"},
        {"This is a very long string with a needle at the end", "needle at the end", "Длинная подстрока"},
        {"abcabcabcabcabcabcabc", "abcabc", "Много повторов (коллизии у Рабина)"}
    };
}

void printComparisonTable(const std::vector<Example>& examples,
    const std::vector<std::vector<size_t>>& results) {
    std::cout << "\nТаблица количества сравнений символов\n\n";

    const int w_num = 4;      // ширина для номера
    const int w_ex = 44;      // ширина для примера
    const int w_algo = 10;    // ширина для каждого алгоритма

    // Заголовки (выравнивание: левое для примера, правое для чисел)
    std::cout << std::left << std::setw(w_num) << "№"
        << std::setw(w_ex) << "Пример (строка | подстрока)"
        << std::right
        << std::setw(w_algo) << "Линейный"
        << std::setw(w_algo) << "Бойер-Мур"
        << std::setw(w_algo) << "КМП"
        << std::setw(w_algo) << "Карп-Рабин"
        << std::left << "  Комментарий\n";

    // Разделитель
    std::cout << std::string(w_num + w_ex + w_algo * 4 + 2, '-') << "\n";

    for (size_t i = 0; i < examples.size(); ++i) {
        const auto& ex = examples[i];
        // Формируем строку примера строго заданной длины
        std::string text_part = ex.text;
        if (text_part.size() > w_ex - 10)
            text_part = text_part.substr(0, w_ex - 13) + "...";
        std::string pat_part = ex.pattern;
        if (pat_part.size() > 8)
            pat_part = pat_part.substr(0, 8) + "...";
        std::string exampleStr = text_part + " | " + pat_part;
        if (exampleStr.size() > w_ex)
            exampleStr = exampleStr.substr(0, w_ex - 3) + "...";

        std::cout << std::left << std::setw(w_num) << (i + 1)
            << std::setw(w_ex) << exampleStr;

        // Находим лучший и худший результат
        size_t best = *std::min_element(results[i].begin(), results[i].end());
        size_t worst = *std::max_element(results[i].begin(), results[i].end());

        for (int alg = 0; alg < 4; ++alg) {
            size_t val = results[i][alg];
            if (val == best) std::cout << "\033[32m";
            else if (val == worst) std::cout << "\033[31m";
            std::cout << std::right << std::setw(w_algo) << val;
            if (val == best || val == worst) std::cout << "\033[0m";
        }
        std::cout << std::left << "  " << ex.comment << "\n";
    }
    std::cout << "\n(зелёный — наименьшее число сравнений, красный — наибольшее)\n";
}

//Постройка длинной строки + замеры
std::string generateLongString(int length = 100'000'000, int patternLen = 10) {
    const std::string filename = "long_string.txt";
    std::ifstream inFile(filename);
    if (inFile.is_open()) {
        std::string content((std::istreambuf_iterator<char>(inFile)),
            std::istreambuf_iterator<char>());
        inFile.close();
        if (content.size() == length) return content;
    }

    std::cout << "Генерация длинной строки (" << length << " символов)...\n";
    std::ofstream outFile(filename);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::uniform_int_distribution<> dis(0, chars.size() - 1);

    for (int i = 0; i < length - patternLen; ++i)
        outFile.put(chars[dis(gen)]);
    std::string pattern = "XyZ012AbCd";
    outFile.write(pattern.c_str(), patternLen);
    outFile.close();

    std::ifstream inFile2(filename);
    std::string content((std::istreambuf_iterator<char>(inFile2)),
        std::istreambuf_iterator<char>());
    return content;
}

double measureTime(const std::string& text, const std::string& pattern,
    int (*algo)(const std::string&, const std::string&, size_t&)) {
    const int repeats = 10;
    double total = 0.0;
    volatile int dummyResult = 0;  // предотвращает оптимизацию вызова
    for (int i = 0; i < repeats; ++i) {
        size_t dummyComparisons = 0;
        auto start = std::chrono::high_resolution_clock::now();
        int result = algo(text, pattern, dummyComparisons);
        auto end = std::chrono::high_resolution_clock::now();
        dummyResult = result;
        std::chrono::duration<double> diff = end - start;
        total += diff.count();
    }
    (void)dummyResult;
    return total / repeats;
}

void printTimeTable(const std::string& text, const std::string& pattern) {
    std::cout << "\nЗамер времени на длинной строке =====\n";
    std::cout << "Длина строки: " << text.size() << " символов\n";
    std::cout << "Подстрока (в конце): \"" << pattern << "\"\n\n";
    std::cout << std::left << std::setw(25) << "Алгоритм" << "Среднее время (сек)\n";
    std::cout << std::string(40, '-') << "\n";

    struct {
        const char* name;
        int (*func)(const std::string&, const std::string&, size_t&);
    } algos[] = {
        {"Линейный", linearSearch},
        {"Бойера-Мура", boyerMoore},
        {"Кнута-Морриса-Пратта", kmpSearch},
        {"Карпа-Рабина", rabinKarp}
    };

    for (const auto& a : algos) {
        double avg = measureTime(text, pattern, a.func);
        std::cout << std::left << std::setw(25) << a.name
            << std::fixed << std::setprecision(6) << avg << "\n";
    }
}

//Главная функция
void runLab() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    std::cout << "Лабораторная работа №2: Алгоритмы поиска подстроки\n";

    //Таблица сравнения всех примеров
    std::vector<Example> examples = createExamples();
    std::vector<std::vector<size_t>> results;
    for (const auto& ex : examples) {
        size_t c1, c2, c3, c4;
        linearSearch(ex.text, ex.pattern, c1);
        boyerMoore(ex.text, ex.pattern, c2);
        kmpSearch(ex.text, ex.pattern, c3);
        rabinKarp(ex.text, ex.pattern, c4);
        results.push_back({ c1, c2, c3, c4 });
    }
    printComparisonTable(examples, results);

    //Замер времени на длинной строке
    const int LONG_LEN = 100'000'000;   // 100 млн символов (по условию)
    const int PATTERN_LEN = 10;
    std::string longText = generateLongString(LONG_LEN, PATTERN_LEN);
    std::string pattern = "XyZ012AbCd";
    printTimeTable(longText, pattern);
}