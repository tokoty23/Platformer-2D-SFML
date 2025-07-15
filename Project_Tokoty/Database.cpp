#include "stdafx.h"
#include "Database.h"

Database::Database(const std::string& dbName)
{
	int rc = sqlite3_open(dbName.c_str(), &db);
	if (rc)
	{
		std::cerr << "Cant open database" << std::endl;
		sqlite3_close(db);
	}
	std::cout << "Opened database successfully" << std::endl;
}

Database::~Database()
{
	sqlite3_close(db);
	std::cout << "Closed database" << std::endl;
}

bool Database::initializeDatabase()
{
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS game_saves (
            slot INTEGER PRIMARY KEY,
            level_id INTEGER NOT NULL
        );
        
        CREATE TABLE IF NOT EXISTS player_saves (
            slot INTEGER PRIMARY KEY,
            health INTEGER NOT NULL,
            max_health INTEGER NOT NULL,
            pos_x REAL NOT NULL,
            pos_y REAL NOT NULL,
            FOREIGN KEY (slot) REFERENCES game_saves(slot) ON DELETE CASCADE
        );
        
        CREATE TABLE IF NOT EXISTS level_enemies_saves (
            slot INTEGER NOT NULL,
            enemy_id INTEGER NOT NULL,
            level_id INTEGER NOT NULL,
            health INTEGER NOT NULL,
            is_defeated INTEGER NOT NULL,
            pos_x REAL NOT NULL,
            pos_y REAL NOT NULL,
            FOREIGN KEY (slot) REFERENCES game_saves(slot) ON DELETE CASCADE,
            PRIMARY KEY (slot, enemy_id)
        );
    )";
    return execute(sql);
}

bool Database::saveGame(int slot, int level_id, const PlayerSaveData& playerData, const std::vector<EnemySaveData>& enemiesData) 
{
    if (!beginTransaction()) return false;

    // Save game data
    std::string gameSql = "INSERT OR REPLACE INTO game_saves (slot, level_id) VALUES (" +
        std::to_string(slot) + ", " + std::to_string(level_id) + ")";

    if (!execute(gameSql)) 
    {
        rollbackTransaction();
        return false;
    }

    // Save player data
    std::string playerSql = "INSERT OR REPLACE INTO player_saves VALUES (" +
        std::to_string(slot) + ", " +
        std::to_string(playerData.health) + ", " +
        std::to_string(playerData.maxHealth) + ", " +
        std::to_string(playerData.x) + ", " +
        std::to_string(playerData.y) + ")";

    if (!execute(playerSql)) 
    {
        rollbackTransaction();
        return false;
    }

    // Clear old enemy data
    std::string clearEnemiesSql = "DELETE FROM level_enemies_saves WHERE slot = " + std::to_string(slot) + ";";
    if (!execute(clearEnemiesSql)) 
    {
        rollbackTransaction();
        return false;
    }

    // Save enemy data
    for (const auto& enemy : enemiesData) 
    {
        std::string enemySql = "INSERT INTO level_enemies_saves VALUES (" +
            std::to_string(slot) + ", " +
            std::to_string(enemy.enemy_id) + ", " +
            std::to_string(level_id) + ", " +
            std::to_string(enemy.health) + ", " +
            (enemy.is_defeated ? "1" : "0") + ", " +
            std::to_string(enemy.x) + ", " +
            std::to_string(enemy.y) + ")";

        if (!execute(enemySql)) 
        {
            rollbackTransaction();
            return false;
        }
    }

    return commitTransaction();
}

bool Database::loadGame(int slot, int& load_level_id, PlayerSaveData& load_playerData, std::vector<EnemySaveData>& load_enemiesData)
{

    sqlite3_stmt* stmt;

    std::string level_id = "SELECT level_id FROM game_saves WHERE slot = ?;";
    if (sqlite3_prepare_v2(db, level_id.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cout << "Eroare level_id" << std::endl;
        return false;
    }
    sqlite3_bind_int(stmt, 1, slot);

    if (sqlite3_step(stmt) == SQLITE_ROW) 
    {
        load_level_id = sqlite3_column_int(stmt, 0);
    }
    else {
        std::cout << "Eroare level_id 2" << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    
    // Load Player Data
    std::string playerData = "SELECT health, max_health, pos_x, pos_y FROM player_saves WHERE slot = ?";
    if (sqlite3_prepare_v2(db, playerData.c_str(), -1, &stmt, nullptr) != SQLITE_OK) return false;
    sqlite3_bind_int(stmt, 1, slot);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        load_playerData.health = sqlite3_column_int(stmt, 0);
        load_playerData.maxHealth = sqlite3_column_int(stmt, 1);
        load_playerData.x = (float)sqlite3_column_double(stmt, 2);
        load_playerData.y = (float)sqlite3_column_double(stmt, 3);
    }
    else
    {
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);

    // Load enemies
    std::string enemiesData = "SELECT enemy_id, health, is_defeated, pos_x, pos_y FROM level_enemies_saves WHERE slot = ?";
    if (sqlite3_prepare_v2(db, enemiesData.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cout << "Eroare enemies" << std::endl;
        return false;
    }
    sqlite3_bind_int(stmt, 1, slot);

    // sterge toti inamicii din vector, size va deveni 0
    load_enemiesData.clear();
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        EnemySaveData enemyData;
        enemyData.enemy_id = sqlite3_column_int(stmt, 0);
        enemyData.health = sqlite3_column_int(stmt, 1);
        enemyData.is_defeated = sqlite3_column_int(stmt, 2);
        enemyData.x = sqlite3_column_int(stmt, 3);
        enemyData.y = (float)sqlite3_column_double (stmt, 4);

        load_enemiesData.push_back(enemyData);
    }
    sqlite3_finalize(stmt);

    return true;
}

bool Database::beginTransaction()
{
    return execute("BEGIN TRANSACTION");
}

bool Database::commitTransaction()
{
    return execute("COMMIT");
}

bool Database::rollbackTransaction()
{
    return execute("ROLLBACK");
}

bool Database::execute(const std::string& sql)
{
	char* errMsg = nullptr;
	if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
		std::cerr << "SQL error: " << errMsg << std::endl;
		sqlite3_free(errMsg);
        return false;
	}
    return true;
}
