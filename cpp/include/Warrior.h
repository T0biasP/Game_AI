#pragma once
//#include "MeleeWeapon.h" 
#include <string>

struct warrior;

struct actionEconomy {
  bool canAct = true;
  bool hasActionsRemaining(const warrior &w) const;
  void reset(warrior &w);
};

struct warrior{
private:

  // int physical;
  // int evasion;
  // int mental;
  // int dex;
  // int str;
  // int level;

public:
    std::string name, id;
    int max_HP = 5, HP = 5, AC = 10, team_id, speed, movement, x, y;//, hit_bonus = 0, speed = 6, movement = speed, 
    double initiative;
    actionEconomy actions;
    enum state {UP, DOWN};
    state currentstate = UP;

    float findDistance();
    void move();
    void hit(const warrior& target);

        //int splendor;
    //std::vector<const weapon*> weapons;

    //Constructors
    warrior();
    warrior(std::string name, std::string id, int team_id, int speed,
        int movement, int x, int y, double initiative, actionEconomy actions)
    : name(name), id(id), team_id(team_id), speed(speed), movement(movement),
    x(x), y(y), initiative(initiative), actions(actions) {};

    
  warrior(const warrior &) = default; //Create by copying
  warrior(warrior &&) = default; //Move from one storage space to another
  warrior &operator=(const warrior &) = default; //Copy into existing
  warrior &operator=(warrior &&) = default; //Mode data into existing



    //void addMeleeWeapon(const std::string &id) {
    //    const weapon* m = WeaponRegistry::instance().get(id);
    //    if (m) maneuvers.push_back(m);
    //    // else: log a warning — unknown maneuver ID
    //}

    //void addRangedWeapon(const std::string &id) {
    //
    //}
};