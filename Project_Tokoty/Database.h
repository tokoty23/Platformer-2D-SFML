#pragma once

struct PlayerSaveData {
    int health;
    int maxHealth;
    float x;
    float y;
    //int level_id;
};

struct EnemySaveData {
    int enemy_id;
    int health;
    bool is_defeated;
    float x;
    float y;
};

class Database
{
private:
    sqlite3* db;
public:
    Database(const std::string& dbName);
    ~Database();

    bool initializeDatabase();
    bool saveGame(int slot, int level_id, const PlayerSaveData& playerData, const std::vector<EnemySaveData>& enemiesData);
    bool loadGame(int slot, int& level_id, PlayerSaveData& playerData, std::vector<EnemySaveData>& enemiesData);
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
    
    bool execute(const std::string& sql);
};

