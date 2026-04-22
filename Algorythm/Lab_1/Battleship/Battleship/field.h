#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <set>
#include <string>

const int N = 10;
const int MAX_ATTEMPTS = 100000;

struct Point {
    int x, y;
    Point(int x_, int y_) : x(x_), y(y_) {}
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

struct ShipComponent {
    int size;
    int minX, maxX, minY, maxY;
    std::set<Point> cells;
};

class Field {
private:
    std::vector<std::vector<int>> grid;
public:
    Field();
    bool inside(int x, int y) const;
    bool empty(int x, int y) const;
    bool occupied(int x, int y) const;
    void set(int x, int y, int val);
    int get(int x, int y) const;
    bool canPlace(const std::vector<Point>& cells) const;
    void placeShip(const std::vector<Point>& cells);
    void clear();
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    void print() const;
    const std::vector<std::vector<int>>& getGrid() const;
};

// Вспомогательные функции
std::vector<Point> generateStraightShip(int len, int x, int y, bool horizontal);
std::vector<Point> generateLShip(int len, int x, int y, int direction);
bool randomChoice(double prob = 0.5);
Field generateRandomPlacement();
std::vector<ShipComponent> extractComponents(const Field& field);
bool noTouching(const std::vector<ShipComponent>& comps, const Field& field);
bool isValidShipShape(const ShipComponent& comp);
bool validatePlacement(const Field& field, int counts[5]);

#endif