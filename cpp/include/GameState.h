#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <array>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include "Warrior.h"

struct GridLocation {
    int x, y;

    bool operator==(const GridLocation& other) const {
        return x == other.x && y == other.y;
    }

    std::vector<GridLocation> neighbors(GridLocation id);
};

struct cell {
    bool occupied;
//    std::string terrain_tag;
};

namespace std {
template <> struct hash<GridLocation> {
  std::size_t operator()(const GridLocation& id) const noexcept {
    return std::hash<int>()(id.x ^ (id.y << 16));
  }
};
}

struct squareGrid {
  static std::array<GridLocation, 4> DIRS;
  int width, height;
  std::unordered_set<GridLocation> walls;
  std::vector<std::vector<cell>> cells; 

  squareGrid(int width_, int height_)
    : width(width_), height(height_) {cells.resize(height, std::vector<cell>(width, {false}));}

  bool in_bounds(GridLocation id) const {
    return 0 <= id.x && id.x < width
        && 0 <= id.y && id.y < height;
  }

  bool passable(GridLocation id) const {
    return walls.find(id) == walls.end() && !cells[id.y][id.x].occupied;
  }
};



std::array<GridLocation, 4> squareGrid::DIRS = {
  GridLocation{1, 0}, GridLocation{-1, 0},
  GridLocation{0, -1}, GridLocation{0, 1}
};

std::vector<GridLocation> neighbors(GridLocation id) const {
    std::vector<GridLocation> results;
    for (GridLocation dir : DIRS) {
        GridLocation next{id.x + dir.x, id.y + dir.y};
        if (in_bounds(next) && passable(next)) {
            results.push_back(next);
        }
    }
    return results;
}

class gameState {
public:

    gameState(std::vector<warrior> teamA,
              std::vector<warrior> teamB,
              squareGrid map);               
    int  round        = 1;
    bool combatActive = true;
    std::vector<int> turnOrder;
    int currentTurnIndex = 0;
    std::vector<warrior> combatants;
    int teamASize = 0;
    int teamBSize = 0;
    squareGrid map;

    warrior& currentActor();
    bool isTerminal() const;
    int winner() const;   
    void advanceTurn();
    void eliminateCombatant(int index);

private:
    void buildTurnOrder();    // sorts combatants by initiative on construction
};