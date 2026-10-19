struct Maneuver {
    std::string id;           // "heavy_strike" — used as registry key
    std::string name;         // "Heavy Strike"
    std::string description;
    
    std::function<void(Character&, Character&, GameState&)> effect;
};