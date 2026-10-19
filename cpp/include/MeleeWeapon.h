struct melee_weapon {
private:
 
public:
    std::string id;
    std::string name;
    int damage;
    int shock_damage;
    int shock_AC;
    bool reach;
    std::string unique_effect_name;
    std::vector<const maneuver*> maneuvers;
   
    void addManeuver(const std::string &id) {
        const maneuver* m = ManeuverRegistry::instance().get(id);
        if (m) maneuvers.push_back(m);
        // else: log a warning — unknown maneuver ID
    }

};