#include <gtest/gtest.h>
#include "column_product.h"

TEST(ColumnProductTest, BasicCase) {
    std::vector<std::vector<int>> matrix = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    std::vector<int> expected = { 0, 120, 0, 384 }; // 6*10*? 2*? 4*8*12?
    // Пересчитаем вручную: столбец 0: 1,5,9 — нет чётных положительных => 0
    // столбец 1: 2,6,10 — произведение 2*6*10 = 120
    // столбец 2: 3,7,11 — нет => 0
    // столбец 3: 4,8,12 — произведение 4*8*12 = 384
    std::vector<int> result = computeColumnProduct(matrix);
    EXPECT_EQ(result, expected);
}

TEST(ColumnProductTest, MixedSigns) {
    std::vector<std::vector<int>> matrix = {
        {-2, 4, 0, 8, -3},
        {6, -4, 2, 0, 1},
        {-5, 0, 8, -2, 3}
    };
    // Ожидаемый результат:
    // столбец0: -2,6,-5 — положительные чётные: 6 => произведение = 6
    // столбец1: 4,-4,0 — положительные чётные: 4 => 4
    // столбец2: 0,2,8 — положительные чётные: 2*8 = 16
    // столбец3: 8,0,-2 — положительные чётные: 8 => 8
    // столбец4: -3,1,3 — положительные чётные: нет => 0
    std::vector<int> expected = { 6, 4, 16, 8, 0 };
    std::vector<int> result = computeColumnProduct(matrix);
    EXPECT_EQ(result, expected);
}

TEST(ColumnProductTest, EmptyMatrix) {
    std::vector<std::vector<int>> matrix;
    std::vector<int> result = computeColumnProduct(matrix);
    EXPECT_TRUE(result.empty());
}

TEST(ColumnProductTest, SingleRow) {
    std::vector<std::vector<int>> matrix = { {2, -4, 6, 0} };
    std::vector<int> expected = { 2, 0, 6, 0 };
    std::vector<int> result = computeColumnProduct(matrix);
    EXPECT_EQ(result, expected);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}