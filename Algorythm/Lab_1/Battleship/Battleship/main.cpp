#include <iostream>
#include <windows.h>
#include "field.h"

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    Field field = generateRandomPlacement();
    std::cout << "Сгенерированное поле:" << std::endl;
    field.print();

    std::string filename = "sea_battle.txt";
    if (field.saveToFile(filename)) {
        std::cout << "Поле сохранено в файл " << filename << std::endl;
    }
    else {
        std::cerr << "Ошибка сохранения в файл" << std::endl;
        return 1;
    }

    Field loadedField;
    if (!loadedField.loadFromFile(filename)) {
        std::cerr << "Ошибка загрузки из файла" << std::endl;
        return 1;
    }

    std::cout << "\nЗагруженное поле:" << std::endl;
    loadedField.print();

    int counts[5];
    if (validatePlacement(loadedField, counts)) {
        std::cout << "\nРасстановка правильная." << std::endl;
        std::cout << "Количество кораблей: " << counts[1] + counts[2] + counts[3] + counts[4] << std::endl;
        std::cout << "  Однопалубных: " << counts[1] << std::endl;
        std::cout << "  Двухпалубных: " << counts[2] << std::endl;
        std::cout << "  Трёхпалубных: " << counts[3] << std::endl;
        std::cout << "  Четырёхпалубных: " << counts[4] << std::endl;
    }
    else {
        std::cout << "\nРасстановка неправильная." << std::endl;
    }

    return 0;
}