#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>
#include <string>

void task5_variant1();

int inputPositiveInt(const std::string& prompt);
void fillRandom(std::vector<std::vector<int>>& matrix);
void printMatrix(const std::vector<std::vector<int>>& matrix, const std::string& title);
bool hasRowWithTwoNegatives(const std::vector<std::vector<int>>& matrix);
void replaceMaxElements(std::vector<std::vector<int>>& matrix, std::vector<int>& columnsWithMax);
void insertZeroColumns(std::vector<std::vector<int>>& matrix, const std::vector<int>& columns);
void removeMiddleRow(std::vector<std::vector<int>>& matrix);
void swapMiddleColumns(std::vector<std::vector<int>>& matrix);

#endif