#ifndef LONGEST_WORD_H
#define LONGEST_WORD_H

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

inline void run() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::cout << "Введите предложение: ";
    std::string line;
    std::getline(std::cin, line);

    std::vector<std::string> longestWords;
    size_t maxLen = 0;
    std::string word;

    for (char ch : line) {
 
        if (ch == ' ' || ch == '.' || ch == ',') {
            if (!word.empty()) {
                if (word.length() > maxLen) {
                    maxLen = word.length();
                    longestWords.clear();
                    longestWords.push_back(word);
                }
                else if (word.length() == maxLen) {
                    longestWords.push_back(word);
                }
                word.clear();
            }
        }
        else {
            word += ch;
        }
    }

    if (!word.empty()) {
        if (word.length() > maxLen) {
            longestWords.clear();
            longestWords.push_back(word);
        }
        else if (word.length() == maxLen) {
            longestWords.push_back(word);
        }
    }

    if (longestWords.empty()) {
        std::cout << "Слов не найдено." << std::endl;
    }
    else {
        std::cout << "Самое длинное слово (все варианты):" << std::endl;
        for (const auto& w : longestWords) {
            std::cout << w << std::endl;
        }
    }
}

#endif