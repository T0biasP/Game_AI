#include <iostream>
#include <string>
#include <vector>
#include "include/Warrior.h"
#include "include/GameState.h"
#include "include/CombatEngine.h"
#include "include/GameState.h"

// --- Helper: print the current state of all combatants ---
void printState(const combatEngine& engine) {
    const gameState& gs = engine.getGameState();

    std::cout << "\n--- Round " << gs.round << " ---\n";
    for (const warrior& w : gs.combatants) {
        std::cout << (w.team_id == 0 ? "[A] " : "[B] ")
                  << w.name
                  << "  HP: " << w.HP << "/" << w.max_HP
                  << "  Pos: (" << w.x << "," << w.y << ")"
                  << (w.HP <= 0 ? "  [DOWN]" : "")
                  << "\n";
    }
}

// --- Helper: print the legal actions available ---
void printLegalActions(const std::vector<int>& actions) {
    std::cout << "Legal actions: [ ";
    for (int a : actions) { std::cout << a << " "; }
    std::cout << "]\n";
}

std::string actionToString(actionID action) {
    switch (action) {
        case actionID::PASS:         "PASS";
        case actionID::ATTACK_NORTH: return "ATTACK_NORTH";
        case actionID::ATTACK_SOUTH: return "ATTACK_SOUTH";
        case actionID::ATTACK_EAST:  return "ATTACK_EAST";
        case actionID::ATTACK_WEST:  return "ATTACK_WEST";
        case actionID::MOVE_NORTH:   return "MOVE_NORTH";
        case actionID::MOVE_SOUTH:   return "MOVE_SOUTH";
        case actionID::MOVE_EAST:    return "MOVE_EAST";
        case actionID::MOVE_WEST:    return "MOVE_WEST";
    }
    return "UNKNOWN_ACTION";
}

int main() {

    // --- Construct warriors ---
    warrior warriorA;
    warriorA.name           = "Fighter A";
    warriorA.team_id        = 0;
    warriorA.max_HP         = 5;
    warriorA.HP     = 5;
    warriorA.AC             = 10;
    warriorA.speed          = 3;
    warriorA.movement = 3;
    warriorA.x              = 0;
    warriorA.y              = 2;
    warriorA.initiative     = 5;

    warrior warriorB;
    warriorB.name           = "Fighter B";
    warriorB.team_id        = 0;
    warriorB.max_HP         = 5;
    warriorB.HP             = 5;
    warriorB.AC             = 100;
    warriorB.speed          = 3;
    warriorB.movement = 3;
    warriorB.x              = 4;
    warriorB.y              = 2;
    warriorB.initiative     = 5;

    // --- Construct grid ---
    squareGrid grid(5, 5);

    // --- Construct engine and reset ---
    combatEngine engine;
    engine.reset({warriorA}, {warriorB}, grid);

    std::cout << "=== COMBAT START ===\n";
    printState(engine);

    // --- Safety cap to prevent infinite loop during testing ---
    const int MAX_STEPS = 200;
    int stepCount = 0;

    // --- Main test loop ---
    while (!engine.getGameState().isTerminal() && stepCount < MAX_STEPS) {
        std::vector<int> legal = engine.getLegalActions();
        printLegalActions(legal);

        // Simple scripted policy — prefer attacks over movement, movement over pass
        int chosen = legal.front();
        for (int a : legal) {
            if (a <= static_cast<int>(actionID::ATTACK_WEST)) {
                chosen = a;     // prefer any attack
                break;
            }
        }

        std::cout << "Actor: " << engine.getGameState().currentActor().name
                  << "  Action: " << actionToString(static_cast<actionID>(chosen)) << "\n";

        float reward = engine.step(chosen);
        std::cout << "Reward: " << reward << "\n";

        printState(engine);
        stepCount++;
    }

    // --- Result ---
    std::cout << "\n=== COMBAT END ===\n";
    if (stepCount >= MAX_STEPS) {
        std::cout << "WARNING: hit step cap — possible infinite loop\n";
    } else {
        int winner = engine.getGameState().winner();
        std::cout << "Winner: Team " << winner << "\n";
        std::cout << "Rounds: " << engine.getGameState().round << "\n";
        std::cout << "Steps:  " << stepCount << "\n";
    }

    return 0;
}