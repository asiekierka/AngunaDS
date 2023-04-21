#include "../enemies.h"
#include "../text.h"
#include "../spriteMgr.h"
#include "../misc.h"
#include "../characters.h"
#include "../mainChar.h"
#include "../sound.h"
#include "bossGfx.h"
#include "crockyGfx.h"
#include "enemies/portraits/boss_portrait.h"

#define Frame(x) (void *)&(enemyBossCharFrames[x])
#define Anim(x) (void *)&(enemyBossCharAnim[x])

#define STATE state1
#define FRAME_CTR state2
#define LAST_STATE state3
#define NOISE_TIMER state4

#define BOSS_BULLET_START 72

#define HEAD 0
#define HAND_1_L 1
#define HAND_1_R 2
#define HAND_2_L 3
#define HAND_2_R 4

#define HEAD_B 5
#define HEAD_C 6
#define HAND_2_L_B 7
#define HAND_2_R_B 8
#define HEAD_D 9
#define HEAD_E 10

#define INIT_DISTANCE 60
#define INIT_DELAY 60
#define INIT_ONE_DROP (INIT_DISTANCE + INIT_DELAY)
#define Y_HOME 90

enum
{
	B_INIT,
	B_INIT2,
	B_HAND_FOLLOW_HEAD,
	B_HEAD_FIRE,
	B_HAND_L_ATTACKING,
	B_HEAD_FIRE_2,
	B_HAND_R_ATTACKING,
	B_HEAD_FIRE_3,
	B_HEAD_DIVE,
	B_DEFEND_PIT
};



const characterFrameDef enemyBossCharFrames[] = 
{
	[HEAD]       = { -32, -32,   0, WIDE,   SIZE_64, false, Frame(HEAD_B)},
	[HEAD_B]     = { -32,   0,  24, SQUARE, SIZE_32, false, Frame(HEAD_C)},
	[HEAD_C]     = {   0,   0,  28, WIDE,   SIZE_32, false, Frame(HEAD_D)},
	[HEAD_D]     = {   0,  16,  92, SQUARE, SIZE_16, false, null},
	[HAND_1_L]   = { -32, -16,   8, WIDE,   SIZE_64, false, null},
	[HAND_1_R]   = { -32, -16,   8, WIDE,   SIZE_64, true,  null},
	[HAND_2_L]   = { -32, -16,  16, WIDE,   SIZE_64, false, Frame(HAND_2_L_B)},
	[HAND_2_L_B] = {   8,  16, 127, SQUARE, SIZE_8,  false, null},
	[HAND_2_R]   = { -24, -16,  16, WIDE,   SIZE_64, true, Frame(HAND_2_R_B)},
	[HAND_2_R_B] = {  -8,  16, 127, SQUARE, SIZE_8,  false, null},
};

#define ANIM(x) {9999,(u16*)bossGfxTiles,Frame(x),null}

const characterAnimDef enemyBossCharAnim[] = 
{
	[HEAD]       = ANIM(HEAD),
	[HAND_1_L]   = ANIM(HAND_1_L),
	[HAND_1_R]   = ANIM(HAND_1_R),
	[HAND_2_L]   = ANIM(HAND_2_L),
	[HAND_2_R]   = ANIM(HAND_2_R),
};






////////////////////////BOSS ITSELF///////////////////////////////

void enemyBossSetState(Enemy * enemy, int state)
{
	enemy->STATE = state;
	enemy->FRAME_CTR = 0;
}


int enemyBossInit(Enemy * enemy)
{
	initEnemyGfx(enemy, (u16*)bossGfxTiles, (characterAnimDef*) &(enemyBossCharAnim[HEAD]));
	enemyBossSetState(enemy, B_INIT);
	enemy->status->mobility = MOBILITY_GHOST;
	enemy->status->y = Y_HOME - (INIT_DISTANCE * 2);
	return 1;
}


//#define TOO_CLOSE 112
#define TOO_CLOSE 130

bool characterTooClose()
{
	if (getMainCharY() < TOO_CLOSE)
	{
		return true;
	}
	return false;
}


int enemyBossYDirToHome(Enemy * enemy)
{
	return unitVal(Y_HOME - enemy->status->y);
}


int enemyBossXDirToPlayer(Enemy * enemy)
{
	return unitVal(getMainCharX() - enemy->status->x);
}

BulletDef * enemyBossShootBulletDown(Enemy * enemy)
{
	int gfxBlock = loadSpriteGfxBlock((u16*)crockyGfxTiles);
	int paletteId = loadPalette((u16*)crockyGfxPal);
	BulletDef * bullet = fireEnemyBullet(enemy, 0, -8, 0, 0, 2);
	setSpriteStartTile(bullet->sprite, BOSS_BULLET_START + spriteGfxBlockFirstTileNum(gfxBlock));
	setSpritePalette(bullet->sprite, paletteId);
	setSpriteSize(bullet->sprite, SIZE_16);
	setSpriteVFlip(bullet->sprite, true);
	return bullet;
}

void enemyBossUpdateDefendPit(Enemy * enemy)
{
	int xDir = enemyBossXDirToPlayer(enemy) * 2;
	int yDir = unitVal(getMainCharY() - 80 - enemy->status->y) * 2;
	moveEnemy(enemy, xDir, yDir);

	if (abs(xDir) < 2)
	{
		xDir = 0;
	}
	if (abs(yDir) < 2)
	{
		yDir = 0;
	}

	if (enemy->FRAME_CTR > 30)
	{
		enemyBossShootBulletDown(enemy);
		enemy->FRAME_CTR = 0;
	}

}

void enemyBossUpdateHandAttacking(Enemy * enemy)
{
	//do nothing...all state managed by hands themselves...
}

void enemyBossShoot(Enemy * enemy, int xDir, int frameSkip)
{
	BulletDef * bullet = enemyBossShootBulletDown(enemy);
	bullet->xSpeed = xDir;
	bullet->xFrameskip = frameSkip;
}

void enemyBossInitState(Enemy * enemy)
{
	if (enemy->STATE == B_INIT)
	{
		if (characterTooClose())
		{
			//switch music
			playMusic(MUSIC_MAINTHEME);
			enemy->status->x = getMainCharX();
			enemyBossSetState(enemy, B_INIT2);
		}
	}

	if (enemy->STATE != B_INIT2)
	{
		return;
	}
	if (enemy->FRAME_CTR >= (INIT_ONE_DROP * 2))
	{
		moveEnemy(enemy, 0, 2);
	}
	if (enemy->FRAME_CTR >= (INIT_ONE_DROP * 2 + 60))
	{
		enemyBossSetState(enemy, B_HEAD_FIRE);
	}
	
}

#define BF(x) ((x)*15)

void enemyBossUpdateFire(Enemy * enemy)
{
	switch(enemy->FRAME_CTR)
	{
		case BF(1):
		case BF(3):
		case BF(5):
		case BF(7):
		case BF(9):
		case BF(11):
		case BF(13):
			enemyBossShoot(enemy, 0, 0);
			break;
		case BF(2):
		case BF(10):
			enemyBossShoot(enemy, 1, 2);
			break;
		case BF(4):
		case BF(12):
			enemyBossShoot(enemy, -1, 2);
			break;
		case BF(6):
			enemyBossShoot(enemy, 1, 1);
			break;
		case BF(8):
			enemyBossShoot(enemy, -1, 1);
			break;
		case BF(14):
			enemyBossSetState(enemy, enemy->STATE + 1);
	}
	if (enemy->FRAME_CTR & 1)
	{
		moveEnemy(enemy, enemyBossXDirToPlayer(enemy), enemyBossYDirToHome(enemy));
	}
	
}

#define DIVE_TIME 35
void enemyBossUpdateHeadDive(Enemy * enemy)
{
	int xDir = 0;
		
	if (enemy->FRAME_CTR & 1)
	{
		//maybe this is TOO hard....
		//xDir = enemyBossXDirToPlayer(enemy);
	}

	int yDir = 2;

	if (enemy->FRAME_CTR > DIVE_TIME)
	{
		yDir = -2;
	}

	moveEnemy(enemy, xDir, yDir);

	if (enemy->FRAME_CTR == (DIVE_TIME * 2))
	{
		enemyBossSetState(enemy, B_HEAD_FIRE);
	}
	
}

int enemyBossUpdate(Enemy * enemy)
{

	enemy->FRAME_CTR++;
	if (enemy->STATE > B_INIT2)
	{
		if (characterTooClose())
		{
			if (enemy->STATE != B_DEFEND_PIT)
			{
				enemy->LAST_STATE = enemy->STATE;
				enemy->STATE = B_DEFEND_PIT;
			}
		}
		else
		{
			if (enemy->STATE == B_DEFEND_PIT)
			{
				enemy->STATE = enemy->LAST_STATE;
			}
		}
	}

	switch(enemy->STATE)
	{
		case B_DEFEND_PIT:
			enemyBossUpdateDefendPit(enemy);
			break;
		case B_HAND_L_ATTACKING:
		case B_HAND_R_ATTACKING:
			enemyBossUpdateHandAttacking(enemy);
			break;
		case B_HEAD_FIRE:
		case B_HEAD_FIRE_2:
		case B_HEAD_FIRE_3:
			enemyBossUpdateFire(enemy);
			break;
		case B_HEAD_DIVE:
			enemyBossUpdateHeadDive(enemy);
			break;
		case B_INIT:
		case B_INIT2:
			enemyBossInitState(enemy);
			break;
	}

	return 1;
}


const EnemyType enemy_bosshand_def;

void enemyBossKillHands(Enemy * enemy)
{
	Enemy * hand = enemy;
	hand++;
	while ((hand->enemyType) == &(enemy_bosshand_def))
	{
		hand->enemyType->updateFunction(hand, ENEMY_DEAD);
		hand++;
	}
	
}


int enemyBossFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyBossInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyBossUpdate(enemy);
			break;
		case ENEMY_DEAD:
			enemyBossKillHands(enemy);
			return killEnemy(enemy);
			break;
	}
	return 1;
}

const EnemyDatabaseDef enemy_boss_db = 
{
	boss_portrait_Tiles,
	boss_portrait_Pal,
	boss_portrait_Map,
	"Dragon",
	"Fire-Breathing beast",
	"The boss of all",
	     "the monsters",
	null,
	7,8,0
};


const EnemyType enemy_boss_def = 
{
	200,			//hp
	16,				//collideDamage
	5,				//numSprites
	64,				//bbHeight
	48,				//bbWidth,
	-24,			//bbOffx
	-32,			//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	true,			//isEnemy
	enemyBossFunc,	//updateFunction
	(short*)&bossGfxPal,
	&enemy_boss_db
};
	
	

/////////////////////BOSS HAND/////////////////////////////////


Enemy * enemyBossGetHead(Enemy * enemy)
{
	Enemy * head = enemy;
	head--;
	if (head->enemyType == &(enemy_boss_def))
		return head;
	head--;
	if (head->enemyType == &(enemy_boss_def))
		return head;
	head--;
	if (head->enemyType == &(enemy_boss_def))
		return head;
	//writeText(0,0,"can't find boss head");
	killEnemy(enemy);
	return enemy;

}


int enemyBossWhichHand(Enemy * enemy)
{
	int hand = HAND_1_L;
	if (enemy->special == 1)
	{
		hand = HAND_1_R;
	}
	return hand;

}



int enemyBossHandInit(Enemy * enemy)
{
	int hand = enemyBossWhichHand(enemy);
	initEnemyGfx(enemy, (u16*)bossGfxTiles, (characterAnimDef*) &(enemyBossCharAnim[hand]));
	enemy->status->mobility = MOBILITY_GHOST;
	return 1;

}


#define HAND_TIME 45
#define HAND_X_SPEED 2
#define HAND_Y_SPEED 1
#define HAND_X_OFFSET 100
#define HAND_Y_OFFSET 20


void enemyBossHandAttack(Enemy * enemy, Enemy * bossHead, int hand)
{
	if (hand != enemyBossWhichHand(enemy))
	{
		return;
	}

	setCharacterAnimation(enemy->status, Anim(hand + 2));

	int yDir = HAND_Y_SPEED;
	int xDir = HAND_X_SPEED;
		
	if (bossHead->FRAME_CTR & 1)
	{
		yDir++;
		xDir++;
	}

	if (hand == HAND_1_R)
	{
		xDir = -xDir;
	}

	if (bossHead->FRAME_CTR > HAND_TIME)
	{
		setCharacterAnimation(enemy->status, Anim(hand));
		xDir = -xDir;
		yDir = -yDir;
	}

	moveEnemy(enemy, xDir, yDir);

	if (bossHead->FRAME_CTR == (HAND_TIME * 2))
	{
		int nextState = B_HEAD_FIRE_2;
		if (bossHead->STATE == B_HAND_R_ATTACKING)
		{
			nextState = B_HEAD_FIRE_3;
		}
		enemyBossSetState(bossHead, nextState);
	}
}

void enemyBossHandStart(Enemy * enemy, Enemy * bossHead)
{
	int frame = bossHead->FRAME_CTR;
	int hand = enemyBossWhichHand(enemy);

	if (frame < INIT_DISTANCE)
	{
		if (hand == HAND_1_R)
		{
			moveEnemy(enemy, 0, 2);
		}
	}
	if (frame == INIT_DISTANCE)
	{
		playGameSfx(SFX_DOOR);
	}

	if ((frame > INIT_ONE_DROP) && (frame < (INIT_ONE_DROP + INIT_DISTANCE)))
	{
		if (hand == HAND_1_L)
		{
			moveEnemy(enemy, 0, 2);
		}
	}
	if (frame == (INIT_ONE_DROP + INIT_DISTANCE))
	{
		playGameSfx(SFX_DOOR);
	}
}


int enemyBossHandUpdate(Enemy * enemy)
{
	if (enemy->NOISE_TIMER > 0)
	{
		enemy->NOISE_TIMER--;
	}

	Enemy * bossHead = enemyBossGetHead(enemy);
	int hand = enemyBossWhichHand(enemy);

	if (bossHead->STATE == B_HAND_L_ATTACKING)
	{
		enemyBossHandAttack(enemy, bossHead, HAND_1_L);
		return 1;
	}
	else if (bossHead->STATE == B_HAND_R_ATTACKING)
	{
		enemyBossHandAttack(enemy, bossHead, HAND_1_R);
		return 1;
	}
	else if (bossHead->STATE == B_INIT2)
	{
		enemyBossHandStart(enemy, bossHead);
	}


	if (hand == HAND_1_L){
		enemy->status->x = bossHead->status->x - HAND_X_OFFSET;
	}
	else
	{
		enemy->status->x = bossHead->status->x + HAND_X_OFFSET;
	}
	if ((bossHead->STATE != B_HEAD_DIVE) && (bossHead->STATE != B_INIT2))
	{
		enemy->status->y = bossHead->status->y + HAND_Y_OFFSET;
	}

	return 1;
}

int enemyBossHandFunc(Enemy * enemy, int func)
{
	switch(func)
	{
		case ENEMY_INIT:
			return enemyBossHandInit(enemy);
			break;
		case ENEMY_UPDATE:
			return enemyBossHandUpdate(enemy);
			break;
		case ENEMY_BULLET_COLLIDE:
			if (enemy->NOISE_TIMER == 0)
			{
				playGameSfx(SFX_ARMOR);
				enemy->NOISE_TIMER = 15;
			}
			return NO_COLLISION;
		case ENEMY_DEAD:
			//enemyBossKillBlobs(enemy);
			return killEnemy(enemy);
			break;
	}
	return 1;
}


const EnemyType enemy_bosshand_def = 
{
	200,			//hp
	16,				//collideDamage
	3,				//numSprites
	24,				//bbHeight
	44,				//bbWidth,
	-28,			//bbOffx
	-16,			//bbOffy
	false,			//killOffscreen
	false,			//respawnScroll
	false,			//isEnemy
	enemyBossHandFunc,	//updateFunction
	(short*)&bossGfxPal,
	null
};








