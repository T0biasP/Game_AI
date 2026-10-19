#pragma once
#include "include/GameState.h" 
#include <algorithm>
#include <stdexcept>

gameState::gameState(std::vector<warrior> teamA, std::vector<warrior> teamB, squareGrid map) : map(std::move(map))
{
    teamASize = teamA.size();
    teamBSize = teamB.size();

    for (auto& c : teamA) { c.team_id = 0; combatants.push_back(std::move(c)); }
    for (auto& c : teamB) { c.team_id = 1; combatants.push_back(std::move(c)); }

    buildTurnOrder();
}

void gameState::buildTurnOrder() {
    turnOrder.resize(combatants.size());
    std::iota(turnOrder.begin(), turnOrder.end(), 0);
    std::sort(turnOrder.begin(), turnOrder.end(), [&](int a, int b) {
        return combatants[a].initiative > combatants[b].initiative;
    });
}

warrior& gameState::currentActor() {
    return combatants[turnOrder[currentTurnIndex]];
}

void GameState::advanceTurn() {
    currentTurnIndex = (currentTurnIndex + 1) % turnOrder.size();
    Warrior& next = currentActor();
    next.actions.reset(next);   // resets canAct and movement
    if (currentTurnIndex == 0) round++;
}

void gameState::eliminateCombatant(int index) {
    turnOrder.erase(
        std::remove(turnOrder.begin(), turnOrder.end(), index),
        turnOrder.end()
    );
    if (currentTurnIndex >= (int)turnOrder.size())
        currentTurnIndex = 0;
    std::cout << "Warrior" << index << "has been killed!" '\n';
}

bool gameState::isTerminal() const {
    bool teamAAlive = false, teamBAlive = false;
    for (const auto& c : combatants) {
        if (c.current_hp > 0 && c.team_id == 0) teamAAlive = true;
        if (c.current_hp > 0 && c.team_id == 1) teamBAlive = true;
    }
    return !teamAAlive || !teamBAlive;
}

int gameState::winner() const {
    if (!isTerminal()) return -1;
    for (const auto& c : combatants)
        if (c.current_hp > 0) return c.team_id;
    return -1;
}