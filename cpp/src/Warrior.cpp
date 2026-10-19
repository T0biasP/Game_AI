#include "Warrior.h"
#include <iostream>

bool actionEconomy::hasActionsRemaining(const warrior& w) const {return canAct || w.movement > 0;}
void actionEconomy::reset(warrior& w) {canAct = true; w.movement = w.speed;}

warrior::warrior()
    : name(""), id(""),
      team_id(0), speed(0), movement(0),
      x(0), y(0), initiative(0.0),
      actions{} {};
      
/*
float warrior::findDistance(warrior& target) {
    x_diff = abs(x - target.x);
    y_diff = abs(y - target.y);
    return(sqrt(x_diff**2+y_diff**2));
}

void warrior::hit(direction) {
    if (actions.canAct==true){
        float distance = findDistance(target);
        if (distance == 1) {
            int roll = roll(20);
            int modifier = std::max(strength, dexterity) + level + bonus;
            int attackRoll = roll + modifier;
            if (attackRoll >= target.ac) {target.hp-=1;}
            if (attackRoll >= (target.ac+10)) {target.hp-=1;}
            target.hp = std::max(target.hp, 0);
            actions.canAct = false;
        }
    }
}

void warrior::move(int x_distance, int y_distance) {
    distance = abs(x_distance) + abs(y_distance)
    if (abs(distance) <= speed) {
        x += x_distance;
        y += y_distance;
        actions.canMove == false;    
    }
}
    */