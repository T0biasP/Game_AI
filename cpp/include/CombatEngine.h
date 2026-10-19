#include "GameState.h"
#include "Warrior.h"
#include <vector>

enum class actionID {
    PASS         = 0,
    ATTACK_NORTH = 1,
    ATTACK_SOUTH = 2,
    ATTACK_EAST  = 3,
    ATTACK_WEST  = 4,
    MOVE_NORTH   = 5,
    MOVE_SOUTH   = 6,
    MOVE_EAST    = 7,
    MOVE_WEST    = 8
    
};

class combatEngine {
public:
    gameState game;

    gameState getGameState() const {return game;}
    int roll(int num, int size);
    gameState reset(std::vector<warrior> teamA, std::vector<warrior> teamB, squareGrid map);
    std::vector<int> getLegalActions();
    float step(int chosen);
    std::vector<float> getStateVector(gameState state);
    int computeReward();
    void moveWarrior(warrior& w, int newX, int newY);
    void tryAttack(int dx, int dy);
};