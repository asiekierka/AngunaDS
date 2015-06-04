#include "enemyDatabase.h"
#include "enemies.h"
#include "text.h"
#include "subscreen.h"
#include "misc.h"


extern const EnemyType  enemy_bat_def ; 
extern const EnemyType  enemy_big_ghost_def ; 
extern const EnemyType  enemy_big_ghost_final_def ; 
extern const EnemyType  enemy_bosshand_def;
extern const EnemyType  enemy_boss_def ; 
extern const EnemyType  enemy_bosshand_def ; 
extern const EnemyType  enemy_boulder_def ; 
extern const EnemyType  enemy_crabblob_def ; 
extern const EnemyType  enemy_crab_def ; 
extern const EnemyType  enemy_crockyHead_def ; 
extern const EnemyType  enemy_crockyHeadFinal_def ; 
extern const EnemyType  enemy_crockyBody_def ; 
extern const EnemyType  enemy_crockyNeck_def ; 
extern const EnemyType  enemy_door_def ; 
extern const EnemyType  enemy_death_def ; 
extern const EnemyType  enemy_fire_statue_def ; 
extern const EnemyType  enemy_statue_def ; 
extern const EnemyType  enemy_ghost_def ; 
extern const EnemyType  enemy_attack_ghost_def ; 
extern const EnemyType  enemy_giantbat_def ; 
extern const EnemyType  enemy_giantbatfinal_def ; 
extern const EnemyType  enemy_graveyard_object_def ; 
extern const EnemyType  enemy_healer_def ; 
extern const EnemyType  enemy_hornet_def ; 
extern const EnemyType  enemy_item_def ; 
extern const EnemyType  enemy_lanternMask_def ; 
extern const EnemyType  enemy_map_object_def ; 
extern const EnemyType  enemy_bigmap_object_def ; 
extern const EnemyType  enemy_ogre_def ; 
extern const EnemyType  enemy_redogre_def ; 
extern const EnemyType  enemy_pit_serpent_def ; 
extern const EnemyType  enemy_redbat_def ; 
extern const EnemyType  enemy_sentry_def ; 
extern const EnemyType  enemy_shieldHobgoblin_def ; 
extern const EnemyType  enemy_shop_def ; 
extern const EnemyType  enemy_slime_def ; 
extern const EnemyType  enemy_redSlime_def ; 
extern const EnemyType  enemy_snake_def ; 
extern const EnemyType  enemy_spider_def ; 
extern const EnemyType  enemy_spit_slime_def ; 
extern const EnemyType  enemy_purpleSlime_def ; 
extern const EnemyType  enemy_switch_def ; 
extern const EnemyType  enemy_trap_def ; 
extern const EnemyType  enemy_trap2_def ; 
extern const EnemyType  enemy_trap3_def ; 
extern const EnemyType  enemy_bat_def;
extern const EnemyType  enemy_redbat_def;
extern const EnemyType  enemy_vampire_def ; 
extern const EnemyType  enemy_darkvampire_def ; 
extern const EnemyType  enemy_wallHugger_def ; 
extern const EnemyType  enemy_zipper_def ; 
extern const EnemyType  enemy_hard_zipper_def ; 


int numEnemyDefs = 52;

const EnemyType * allEnemyDefs[] = {& enemy_bat_def , 
& enemy_big_ghost_def , 
& enemy_big_ghost_final_def , 
& enemy_bosshand_def,
& enemy_boss_def , 
& enemy_bosshand_def , 
& enemy_boulder_def , 
& enemy_crabblob_def , 
& enemy_crab_def , 
& enemy_crockyHead_def , 
& enemy_crockyHeadFinal_def , 
& enemy_crockyBody_def , 
& enemy_crockyNeck_def , 
& enemy_door_def , 
& enemy_death_def , 
& enemy_fire_statue_def , 
& enemy_statue_def , 
& enemy_ghost_def , 
& enemy_attack_ghost_def , 
& enemy_giantbat_def , 
& enemy_giantbatfinal_def , 
& enemy_graveyard_object_def , 
& enemy_healer_def , 
& enemy_hornet_def , 
& enemy_item_def , 
& enemy_lanternMask_def , 
& enemy_map_object_def , 
& enemy_bigmap_object_def , 
& enemy_ogre_def , 
& enemy_redogre_def , 
& enemy_pit_serpent_def , 
& enemy_redbat_def , 
& enemy_sentry_def , 
& enemy_shieldHobgoblin_def , 
& enemy_shop_def , 
& enemy_slime_def , 
& enemy_redSlime_def , 
& enemy_snake_def , 
& enemy_spider_def , 
& enemy_spit_slime_def , 
& enemy_purpleSlime_def , 
& enemy_switch_def , 
& enemy_trap_def , 
& enemy_trap2_def , 
& enemy_trap3_def , 
& enemy_bat_def,
& enemy_redbat_def,
& enemy_vampire_def , 
& enemy_darkvampire_def , 
& enemy_wallHugger_def , 
& enemy_zipper_def , 
& enemy_hard_zipper_def , };




EnemyType * EnemyDatabase[ENEMY_DATABASE_SIZE];
int EnemyDatabaseIndex;
int LastEnemyDatabaseIndex;
Enemy * g_lastHitEnemy;
bool g_enemyDBNeedsUpdate;

void initEnemyDatabase()
{
	int i;
	for (i = 0; i < ENEMY_DATABASE_SIZE; i++)
	{
		EnemyDatabase[i] = null;
	}
	EnemyDatabaseIndex = 0;
	LastEnemyDatabaseIndex = 0;
}

void repairEnemyDatabase()
{
	initEnemyDatabase();
}


void setLastHitEnemy(Enemy * enemy)
{
	g_enemyDBNeedsUpdate = true;
	if (enemy == null)
	{
		g_lastHitEnemy = null;
		return;
	}

	if (enemy->enemyType == null)
	{
		return;
	}

	if (enemy->enemyType->databaseDef == null)
	{
		return;
	}
	
	addEnemyToDatabase(enemy->enemyType);
	if (enemy->hp > 0)
	{
		g_lastHitEnemy = enemy;
	}
	else
	{
		g_lastHitEnemy = null;
	}
	

	EnemyType * current;
	int sanityCheck = 0;
	current = null;
	while (current != enemy->enemyType)
	{
		current = nextEnemyDatabaseEntry();
		if (sanityCheck++ > 200)
		{
			ASSERT(false, "could not find enemy that should have been added");
			break;
		}
	}
}

Enemy * getLastHitEnemy()
{
	return g_lastHitEnemy;
}

bool getAndClearEnemyDBNeedsUpdate()
{
	bool oldVal = g_enemyDBNeedsUpdate;
	g_enemyDBNeedsUpdate = false;
	return oldVal;
}



bool addEnemyToDatabase(EnemyType * enemyType)
{
	EnemyType * def = enemyType;

	if (def->databaseDef == null)
	{
		return false;
	}

	int i;
	for (i = 0; i < ENEMY_DATABASE_SIZE; i++)
	{
		if (EnemyDatabase[i] == def)
		{
			return false;
		}

		if (EnemyDatabase[i] == null)
		{
			EnemyDatabase[i] = def;
			LastEnemyDatabaseIndex = i;
			showNewEnemyAdded(def->databaseDef);
			return true;
		}
	}


	ASSERT(false, "enemy database too small");
	repairEnemyDatabase();
	addEnemyToDatabase(enemyType);

	return false;
}

EnemyType * getCurrentEnemyDatabaseEntry()
{
	return EnemyDatabase[EnemyDatabaseIndex];
}

EnemyType * nextEnemyDatabaseEntry()
{
	EnemyDatabaseIndex++;
	if (getCurrentEnemyDatabaseEntry() == null)
	{
		EnemyDatabaseIndex = 0;
	}
	g_enemyDBNeedsUpdate = true;

	return getCurrentEnemyDatabaseEntry();
	
}

EnemyType * prevEnemyDatabaseEntry()
{
	EnemyDatabaseIndex--;
	if (EnemyDatabaseIndex < 0)
	{
		EnemyDatabaseIndex = LastEnemyDatabaseIndex;
	}
	g_enemyDBNeedsUpdate = true;

	return getCurrentEnemyDatabaseEntry();
}

EnemyType * enemyAtDatabase(int slot)
{
	return EnemyDatabase[slot];
}

void setEnemyAtDatabase(int slot, EnemyType * enemyType)
{
	EnemyDatabase[slot] = enemyType;	

	if ((slot > LastEnemyDatabaseIndex) && (enemyType != null))
	{
		LastEnemyDatabaseIndex = slot;
	}
}

void addAllEnemies()
{
	int i = 0;
	for (i = 0; i < numEnemyDefs; i++)
	{
		addEnemyToDatabase(allEnemyDefs[i]);
	}
}

