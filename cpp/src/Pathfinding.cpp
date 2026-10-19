//More detailed version, unused
/* 
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

  std::vector<GridLocation> neighbors(GridLocation id) const {
    std::vector<GridLocation> results;

    for (GridLocation dir : DIRS) {
      GridLocation next{id.x + dir.x, id.y + dir.y};
      if (in_bounds(next) && passable(next)) {
        results.push_back(next);
      }
    }

    if ((id.x + id.y) % 2 == 0) {
      // see "Ugly paths" section for an explanation:
      std::reverse(results.begin(), results.end());
    }

    return results;
  }
};
*/

/*
template<typename Location, typename Graph>
std::unordered_map<Location, Location>
breadth_first_search(Graph graph, Location start) {
  std::queue<Location> frontier;
  frontier.push(start);

  std::unordered_map<Location, Location> distance;
  distance[start] = 0;

  while (!frontier.empty()) {
    Location current = frontier.front();
    frontier.pop();

    for (Location next : graph.neighbors(current)) {
      if (distance.find(next) == distance.end()) {
        frontier.push(next);
        distance[next] +=1;
      }
    }
  }
  return distance;
}
  */