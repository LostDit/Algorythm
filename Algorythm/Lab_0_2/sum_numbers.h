#ifndef SUM_NUMBERS_H
#define SUM_NUMBERS_H

#include <iostream>
#include <string>
#include <cctype>
#include <windows.h>

inline void run() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    std::cout << "Введите строку: ";
    std::string s;
    std::getline(std::cin, s);

    long long sum = 0;
    bool inNumber = false;
    bool negative = false;
    long long num = 0;

    for (size_t i = 0; i < s.length(); ++i) {
        char ch = s[i];
        unsigned char uc = static_cast<unsigned char>(ch);
        if (std::isdigit(uc)) {
            if (!inNumber) {
                inNumber = true;
                num = 0;
                if (i > 0 && s[i - 1] == '-')
                    negative = true;
                else
                    negative = false;
            }
            num = num * 10 + (ch - '0');
        }
        else {
            if (inNumber) {
                sum += (negative ? -num : num);
                inNumber = false;
            }
        }
    }
    if (inNumber) {
        sum += (negative ? -num : num);
    }

    std::cout << "Сумма чисел: " << sum << std::endl;
}

#endif