class ManeuverRegistry {
public:
    static ManeuverRegistry& instance() {
        static ManeuverRegistry reg;
        return reg;
    }

    void registerManeuver(maneuver m) {
        pool[m.id] = std::move(m);
    }

    const maneuver* get(const std::string& id) const {
        auto it = pool.find(id);
        return (it != pool.end()) ? &it->second : nullptr;
    }

private:
    std::unordered_map<std::string, maneuver> pool;
};