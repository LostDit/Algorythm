#include "field.h"
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <algorithm>

Field::Field() : grid(N, std::vector<int>(N, 0)) {}

bool Field::inside(int x, int y) const {
    return x >= 0 && x < N && y >= 0 && y < N;
}

bool Field::empty(int x, int y) const {
    return inside(x, y) && grid[x][y] == 0;
}

bool Field::occupied(int x, int y) const {
    return inside(x, y) && grid[x][y] == 1;
}

void Field::set(int x, int y, int val) {
    if (inside(x, y)) grid[x][y] = val;
}

int Field::get(int x, int y) const {
    if (inside(x, y)) return grid[x][y];
    return 0;
}

bool Field::canPlace(const std::vector<Point>& cells) const {
    for (const auto& p : cells) {
        if (!empty(p.x, p.y)) return false;
    }
    const int dx8[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    const int dy8[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    for (const auto& p : cells) {
        for (int d = 0; d < 8; ++d) {
            int nx = p.x + dx8[d];
            int ny = p.y + dy8[d];
            if (occupied(nx, ny)) {
                bool inShip = false;
                for (const auto& q : cells) {
                    if (q.x == nx && q.y == ny) {
                        inShip = true;
                        break;
                    }
                }
                if (!inShip) return false;
            }
        }
    }
    return true;
}

void Field::placeShip(const std::vector<Point>& cells) {
    for (const auto& p : cells) {
        set(p.x, p.y, 1);
    }
}

void Field::clear() {
    for (int i = 0; i < N; ++i)
        std::fill(grid[i].begin(), grid[i].end(), 0);
}

bool Field::saveToFile(const std::string& filename) const {
    std::ofstream fout(filename);
    if (!fout) return false;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            fout << grid[i][j] << (j == N - 1 ? "" : " ");
        }
        fout << std::endl;
    }
    return true;
}

bool Field::loadFromFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) return false;
    clear();
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int val;
            fin >> val;
            if (val != 0 && val != 1) return false;
            grid[i][j] = val;
        }
    }
    return true;
}

void Field::print() const {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

const std::vector<std::vector<int>>& Field::getGrid() const {
    return grid;
}

std::vector<Point> generateStraightShip(int len, int x, int y, bool horizontal) {
    std::vector<Point> cells;
    if (horizontal) {
        for (int i = 0; i < len; ++i)
            cells.emplace_back(x, y + i);
    }
    else {
        for (int i = 0; i < len; ++i)
            cells.emplace_back(x + i, y);
    }
    return cells;
}

std::vector<Point> generateLShip(int len, int x, int y, int direction) {
    int longArm = len - 1;
    std::vector<Point> cells;
    cells.emplace_back(x, y);
    if (direction == 0) {
        for (int i = 1; i <= longArm; ++i)
            cells.emplace_back(x + i, y);
        cells.emplace_back(x, y + 1);
    }
    else if (direction == 1) {
        for (int i = 1; i <= longArm; ++i)
            cells.emplace_back(x + i, y);
        cells.emplace_back(x, y - 1);
    }
    else if (direction == 2) {
        for (int i = 1; i <= longArm; ++i)
            cells.emplace_back(x - i, y);
        cells.emplace_back(x, y + 1);
    }
    else {
        for (int i = 1; i <= longArm; ++i)
            cells.emplace_back(x - i, y);
        cells.emplace_back(x, y - 1);
    }
    return cells;
}

bool randomChoice(double prob) {
    static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng) < prob;
}

Field generateRandomPlacement() {
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> coordDist(0, N - 1);
    std::uniform_int_distribution<int> dirDist(0, 1);   // для прямых
    std::uniform_int_distribution<int> ldirDist(0, 3);  // для Г-образных нужны G-test или нет непонятно, поэтому сформирую несколько txt

    Field field;
    bool success = false;
    int attempts = 0;

    while (!success) {
        field.clear();
        success = true;

        std::vector<int> lengths = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

        for (int len : lengths) {
            bool placed = false;
            int tries = 0;
            while (!placed && tries < MAX_ATTEMPTS) {
                std::vector<Point> ship;
                if (len == 3 || len == 4) {
                    bool isL = randomChoice();
                    if (isL) {
                        int x = coordDist(rng);
                        int y = coordDist(rng);
                        int dir = ldirDist(rng);
                        ship = generateLShip(len, x, y, dir);
                        bool inside = true;
                        for (auto& p : ship) {
                            if (!field.inside(p.x, p.y)) {
                                inside = false;
                                break;
                            }
                        }
                        if (!inside) {
                            tries++;
                            continue;
                        }
                    }
                    else {
                        bool horizontal = dirDist(rng);
                        int x = coordDist(rng);
                        int y = coordDist(rng);
                        if (horizontal) {
                            if (y + len > N) {
                                tries++;
                                continue;
                            }
                        }
                        else {
                            if (x + len > N) {
                                tries++;
                                continue;
                            }
                        }
                        ship = generateStraightShip(len, x, y, horizontal);
                    }
                }
                else {
                    bool horizontal = (len == 2) ? dirDist(rng) : true;
                    int x = coordDist(rng);
                    int y = coordDist(rng);
                    if (horizontal) {
                        if (y + len > N) {
                            tries++;
                            continue;
                        }
                    }
                    else {
                        if (x + len > N) {
                            tries++;
                            continue;
                        }
                    }
                    ship = generateStraightShip(len, x, y, horizontal);
                }

                if (field.canPlace(ship)) {
                    field.placeShip(ship);
                    placed = true;
                }
                else {
                    tries++;
                }
            }
            if (!placed) {
                success = false;
                break;
            }
        }
        attempts++;
        if (attempts > 1000 && !success) {
            std::cerr << "Too many attempts, restarting..." << std::endl;
        }
    }
    return field;
}

std::vector<ShipComponent> extractComponents(const Field& field) {
    std::vector<std::vector<bool>> visited(N, std::vector<bool>(N, false));
    std::vector<ShipComponent> components;
    const int dx4[] = { -1, 1, 0, 0 };
    const int dy4[] = { 0, 0, -1, 1 };

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (field.get(i, j) == 1 && !visited[i][j]) {
                ShipComponent comp;
                comp.minX = comp.maxX = i;
                comp.minY = comp.maxY = j;
                comp.size = 0;
                std::vector<Point> queue;
                queue.emplace_back(i, j);
                visited[i][j] = true;
                while (!queue.empty()) {
                    Point p = queue.back();
                    queue.pop_back();
                    comp.cells.insert(p);
                    comp.size++;
                    comp.minX = (std::min)(comp.minX, p.x);
                    comp.maxX = (std::max)(comp.maxX, p.x);
                    comp.minY = (std::min)(comp.minY, p.y);
                    comp.maxY = (std::max)(comp.maxY, p.y);
                    for (int d = 0; d < 4; ++d) {
                        int nx = p.x + dx4[d];
                        int ny = p.y + dy4[d];
                        if (field.inside(nx, ny) && field.get(nx, ny) == 1 && !visited[nx][ny]) {
                            visited[nx][ny] = true;
                            queue.emplace_back(nx, ny);
                        }
                    }
                }
                components.push_back(comp);
            }
        }
    }
    return components;
}

bool noTouching(const std::vector<ShipComponent>& comps, const Field& field) {
    const int dx8[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    const int dy8[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
    for (const auto& comp : comps) {
        for (const auto& p : comp.cells) {
            for (int d = 0; d < 8; ++d) {
                int nx = p.x + dx8[d];
                int ny = p.y + dy8[d];
                if (field.inside(nx, ny) && field.get(nx, ny) == 1) {
                    if (comp.cells.find(Point(nx, ny)) == comp.cells.end()) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool isValidShipShape(const ShipComponent& comp) {
    int size = comp.size;
    int w = comp.maxX - comp.minX + 1;
    int h = comp.maxY - comp.minY + 1;

    if (size == 1) {
        return (w == 1 && h == 1);
    }
    else if (size == 2) {
        return ((w == 1 && h == 2) || (w == 2 && h == 1));
    }
    else if (size == 3) {
        if ((w == 1 && h == 3) || (w == 3 && h == 1)) return true;
        if (w == 2 && h == 2) return true;
        return false;
    }
    else if (size == 4) {
        if ((w == 1 && h == 4) || (w == 4 && h == 1)) return true;
        if ((w == 2 && h == 3) || (w == 3 && h == 2)) return true;
        return false;
    }
    return false;
}

bool validatePlacement(const Field& field, int counts[5]) {
    for (int i = 1; i <= 4; ++i) counts[i] = 0;
    auto components = extractComponents(field);
    if (components.empty()) return false;
    if (!noTouching(components, field)) return false;
    for (const auto& comp : components) {
        int size = comp.size;
        if (size < 1 || size > 4) return false;
        if (!isValidShipShape(comp)) return false;
        counts[size]++;
    }
    return (counts[1] == 4 && counts[2] == 3 && counts[3] == 2 && counts[4] == 1);
}