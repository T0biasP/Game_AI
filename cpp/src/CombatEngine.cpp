#include "include/GameState.h"
#include "include/CombatEngine.h"
#include "include/Warrior.h"
#include <vector>

float combatEngine::computeReward(int controlledTeam) {
    float reward = 0.0f;
    for (const Warrior& w : gameState.combatants) {
        if (w.team_id == controlledTeam) {
            // Reward surviving allies by their remaining HP ratio
            if (w.current_hp > 0) {
                reward += (float)w.current_hp / w.max_hp;
            }
        } else {
            // Reward damage dealt to enemies — full value for kills
            float hpRatio = (float)w.current_hp / w.max_hp;
            reward += (1.0f - hpRatio);             // 0.0 undamaged → 1.0 dead
        }
    }

    if (gameState.isTerminal()) {
        int winner = gameState.winner();
        if (winner == controlledTeam) {
            reward += 10.0f;                        // win bonus

            // Efficiency bonus — reward winning in fewer rounds
            const int MAX_ROUNDS = 10;
            float efficiency = 1.0f - ((float)gameState.round / MAX_ROUNDS);
            reward += 5.0f * efficiency;            // up to +5 for fastest win
        } else {
            reward -= 10.0f;                        // loss penalty
        }
    }

    // --- Per-round penalty to discourage stalling ---
    reward -= 0.1f;
    return reward;
};

std::vector<int> combatEngine::getLegalActions() {
    Warrior& actor = gameState.currentActor();

    std::vector<int> actions;
    actions.push_back(8); 
    if (actor.state == DOWN) {return actions};

    //Actions
    if (actor.actions.canAct) {
        const int dirs[4][2]          = {{ 0,-1}, { 0, 1}, { 1, 0}, {-1, 0}};
        const actionID attackIDs[4]   = {
            actionID::ATTACK_NORTH,
            actionID::ATTACK_SOUTH,
            actionID::ATTACK_EAST,
            actionID::ATTACK_WEST
        };

        for (int i = 0; i < 4; i++) {
            int tx = actor.x + dirs[i][0];
            int ty = actor.y + dirs[i][1];
            for (auto& combatant : game.combatants) {
                if (combatant.team_id != actor.team_id &&
                    combatant.current_hp > 0           &&
                    combatant.x == tx                  &&
                    combatant.y == ty) {
                    actions.push_back(static_cast<int>(attackIDs[i]));
                    break;
                }
            }
        }
    }

    //Movement
    if (actor.movement > 0) {
        const int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        const int ids[4]     = {5, 6, 7, 8};  // N S E W action IDs

        for (int i = 0; i < 4; i++) {
            int nx = actor.x + dirs[i][0];
            int ny = actor.y + dirs[i][1];
            if (gameState.grid.inBounds(ny, nx) &&
                !gameState.grid.at(ny, nx).occupied) {
                actions.push_back(ids[i]);
            }
        }
    }

    return actions;
};

std::vector<float> CombatEngine::getStateVector() const {
    std::vector<float> state;
    const float gridW = gameState.grid.width  - 1;
    const float gridH = gameState.grid.height - 1;

    for (const Warrior& w : gameState.combatants) {
        bool alive = w.current_hp > 0;
        state.push_back(alive ? (float)w.current_hp / w.max_hp : 0.0f);
        state.push_back((float)w.x / gridW);
        state.push_back((float)w.y / gridH);
        state.push_back(alive ? 1.0f : 0.0f);
        state.push_back((float)w.team_id);
        state.push_back((alive && w.actions.canAct) ? 1.0f : 0.0f);
        state.push_back(alive ? (float)w.movement / w.speed : 0.0f);
    }

    // Round number — normalised against a maximum round cap
    const int MAX_ROUNDS = 10;
    state.push_back((float)gameState.round / MAX_ROUNDS);

    for (int i = 0; i < (int)gameState.combatants.size(); i++) {
        state.push_back(gameState.turnOrder[gameState.currentTurnIndex] == i ? 1.0f : 0.0f);
    }
    return state;
};

void combatEngine::moveWarrior(warrior& w, int newX, int newY) {
    gameState.grid.at(w.y, w.x).occupied = false;  // clear old cell first
    w.x = newX;                                     // update warrior position
    w.y = newY;
    gameState.grid.at(w.y, w.x).occupied = true;   // mark new cell
    w.actions.canMove = false;                      // update economy
};

gameState combatEngine::reset(std::vector<warrior> teamA, std::vector<warrior> teamB, squareGrid map){
    for (auto& warrior : teamA) {warrior.actionEconomy.reset();}
    for (auto& warrior : teamB) {warrior.actionEconomy.reset();}
    game = gameState(teamA, teamB, map);
    return game;
};

int combatEngine::roll(int num, int size){
    std::uniform_int_distribution<>;
    rolls =  distrib(1, size);
    return sum(rolls);
};

float combatEngine::step(int actionId) {
    Warrior& actor = gameState.currentActor();
    controlledTeam = actor.team
    ActionID action = static_cast<ActionID>(actionId);
    std::string actionDescription;
        switch (action) {

        case actionID::ATTACK_NORTH: { actionDescription = "attacks north!"; tryAttack( 0, -1); break; }
        case actionID::ATTACK_SOUTH: { actionDescription = "attacks south!"; tryAttack( 0,  1); break; }
        case actionID::ATTACK_EAST:  { actionDescription = "attacks east!"; tryAttack( 1,  0); break; }
        case actionID::ATTACK_WEST:  { actionDescription = "attacks west!"; tryAttack(-1,  0); break; }

        case actionID::MOVE_NORTH: { actionDescription = "moves north"; moveWarrior(actor, actor.x, actor.y - 1); actor.movement--; break; }
        case actionID::MOVE_SOUTH: { actionDescription = "moves south"; moveWarrior(actor, actor.x, actor.y + 1); actor.movement--; break; }
        case actionID::MOVE_EAST:  { actionDescription = "moves east"; moveWarrior(actor, actor.x + 1, actor.y); actor.movement--; break; }
        case actionID::MOVE_WEST:  { actionDescription = "moves west"; moveWarrior(actor, actor.x - 1, actor.y); actor.movement--; break; }

        case actionID::PASS: {
            actionDescription = "ends their turn";
            actor.actions.canAct = false;
            actor.movement = 0;
            break;
        }
        default: { break; }
    }
    std::cout << actor.name << " " << actionDescription << '\n';
    // Advance turn if the current actor has nothing left to do
    if (!actor.actions.hasActionsRemaining(actor)) {
        gameState.advanceTurn();
    }
    return computeReward(controlledTeam)
};

void combatEngine::tryAttack(warrior& actor,int dx, int dy){
    int tx = actor.x + dx;
    int ty = actor.y + dy;
    for (auto& target : gameState.combatants) {
        if (target.team_id != actor.team_id &&
            target.current_hp > 0        &&
            target.x == tx               &&
            target.y == ty) {
            actor.hit(target);
            if (target.current_hp <= 0) {
                eliminateCombatant(target);
            }
            break;
        }
    }
    actor.actions.canAct = false;
};








