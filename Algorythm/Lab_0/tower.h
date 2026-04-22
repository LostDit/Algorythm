#ifndef TOWER_H
#define TOWER_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <windows.h>

inline void run() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    int n;
    std::cout << "Введите кол-во банок: ";
    std::cin >> n;

    std::vector<int> diameters(n);
    for (int i = 0; i < n; ++i) {
        std::cout << "Банка " << i + 1 << ": ";
        std::cin >> diameters[i];
    }

    std::unordered_set<int> unique(diameters.begin(), diameters.end());
    std::cout << "Максимальная высота башни: " << unique.size() << std::endl;
}

#endif