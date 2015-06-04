#include "enemies.h"


void initEnemyDatabase();
bool addEnemyToDatabase(EnemyType * enemyType);
void setLastHitEnemy(Enemy * enemy);
Enemy * getLastHitEnemy();
EnemyType * getCurrentEnemyDatabaseEntry();
EnemyType * nextEnemyDatabaseEntry();
EnemyType * prevEnemyDatabaseEntry();
bool getAndClearEnemyDBNeedsUpdate();
void addAllEnemies();


//only for saving/loading the database
EnemyType * enemyAtDatabase(int slot);
void setEnemyAtDatabase(int slot, EnemyType * enemyType);
#define ENEMY_DATABASE_SIZE 32
