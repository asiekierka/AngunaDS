#include "characters.h"
#include "level.h"
#include "spriteMgr.h"
#include "text.h"
#include "misc.h"

int getScreenX(int worldX)
{
	return worldToScreenX(worldX);
}

int getScreenY(int worldY)
{
	return worldToScreenY(worldY);
}


bool isOnScreen(int width, int height, int screenX, int screenY)
{
	return objectIsOnScreen(width, height, screenX, screenY);
}

void loadAndSetPalette(CharacterStatus * status, u16 * palette)
{
	//loadPalette contains the logic to check to see if it's already loaded
	//so we don't have to duplicate that here
	int paletteId = loadPalette(palette);

	int i;
	for(i = 0; i < status->listSize; i++)
	{
		setSpritePalette(status->spriteList[i], paletteId);
	}
}

bool ensureCharacterGfxLoaded(CharacterStatus * status)
{
	//most common, if the block it thinks is loaded for it
	//really contains the gfx info, don't do anything
	if (getOriginOfSpriteGfxBlock(status->gfxBlock) == status->currentAnim->gfxBlockSource)
	{
		return true;
	}

	//if the block it thinks is loaded isn't really loaded, 
	//first look to see if another block has the gfx data loaded
	if (findSpriteGfxBlock(status->currentAnim->gfxBlockSource) >= 0)
	{
		//if so, remember which block it is
		status->gfxBlock = findSpriteGfxBlock(status->currentAnim->gfxBlockSource);
		return true;
	}

 	status->gfxBlock = loadSpriteGfxBlock(status->currentAnim->gfxBlockSource);
	return false;

}

characterFrameDef * nextFrame(characterFrameDef * currFrame)
{

	if (currFrame->nextSprite == FRAME_NEXT)
	{
		currFrame++;
	}
	else
	{
		currFrame = currFrame->nextSprite;
	}
	
	return currFrame;
}


void setCharacterFrameOneSprite(Sprite * sprite, CharacterStatus * status,
								characterFrameDef * frameDef)
{
	if (frameDef == null)
	{
		setSpritePosition(sprite, 0,0);
		setSpriteStartTile(sprite, BLANK_TILE);
		setSpriteSize(sprite, SIZE_8);
		setSpriteShape(sprite, SQUARE);
		return;
	}

	setSpritePosition(sprite, status->screenX + frameDef->xOffset, status->screenY + frameDef->yOffset);
	if (frameDef->hflip == FLIP_V_ONLY)
	{
		setSpriteVFlip(sprite, true);
		setSpriteFlip(sprite, false);
	}
	else
	if (frameDef->hflip == FLIP_BOTH)
	{
		setSpriteVFlip(sprite, true);
		setSpriteFlip(sprite, true);
	}
	else
	{
		setSpriteFlip(sprite, frameDef->hflip);
		setSpriteVFlip(sprite, false);
	}

	setSpriteSize(sprite, frameDef->size);
	setSpriteShape(sprite, frameDef->shape);
	if (frameDef->tileNum == TILE_SPECIAL)
	{
		setSpriteStartTile(sprite, status->special + spriteGfxBlockFirstTileNum(status->gfxBlock));
	}
	else
	{
		setSpriteStartTile(sprite, frameDef->tileNum + spriteGfxBlockFirstTileNum(status->gfxBlock));
	}

}

void setCharacterFrame(CharacterStatus * status, characterFrameDef * frameDef)
{
	int ctr;
	characterFrameDef * currFrame = frameDef;
	for (ctr = 0; ctr < status->listSize; ctr++)
	{
		setCharacterFrameOneSprite(status->spriteList[ctr], status, currFrame);
		if (currFrame != null)
		{
			currFrame = nextFrame(currFrame);
			//if (currFrame->nextSprite == FRAME_NEXT)
			//{
				//currFrame++;
			//}
			//else
			//{
				//currFrame = currFrame->nextSprite;
			//}
		}
	}

	status->currentFrame = frameDef;
}

void setCharacterAnimationFrame(CharacterStatus * status, characterAnimDef * animDef)
{
	//set the current animation
	status->currentAnim = animDef;
	status->frameCounter = 1;
	//ensureCharacterGfxLoaded(status);
	setCharacterFrame(status, animDef->thisFrame);
	
}

bool setCharacterAnimation(CharacterStatus * status, characterAnimDef * animDef)
{
	//if this is already the current animation, do nothing
	
	if (animDef == status->animBase)
	{
		//but if it is stopped, turn it back on
		if (status->frameCounter == 0)
		{
			status->frameCounter = status->currentAnim->frameDelay;
		}

		return false;
	}

	setCharacterAnimationForced(status, animDef);
	return true;
}

void setCharacterAnimationForced(CharacterStatus * status, characterAnimDef * animDef)
{
	status->animBase = animDef;
	setCharacterAnimationFrame(status, animDef);
}

void updateCharacter(CharacterStatus * status)
{
	TRACE();
	if (status->listSize > 20)
	{
		ASSERT(false, "bad list size");

	}
	ASSERT(status != null, "null status");
	if (status->faded > 0)
	{
		bool dofade = true;
		int ctr;
		status->faded--;
		if (status->faded == 0)
		{
			dofade = false;
			setCharacterBulletImmunityFinished(status);
		}
		for (ctr = 0; ctr < status->listSize; ctr++)
		{
			setSpriteFaded(status->spriteList[ctr], dofade);
		}
	}
	else
	{
		setCharacterBulletImmunityFinished(status);
	}
	status->screenX = getScreenX(status->x);
	//if offscreen, keep it offscreen;
	if ((status->screenX > 350) || (status->screenX < -60))
	{
		status->screenX = 400;
	}

	status->screenY = getScreenY(status->y);
	if ((status->screenY > 200) || (status->screenY < -32))
	{
		status->screenX = 400;
	}


	//increment the frame counter
	if (status->frameCounter > 0)
	{
		status->frameCounter++;
	}
	
	//and go to the next frame, if necessary (the frame counter is reset inside)
	if (status->frameCounter > status->currentAnim->frameDelay)
	{
		setCharacterAnimationFrame(status, status->currentAnim->nextFrame);
	}
	
}


void repositionCharacter(CharacterStatus * status)
{
	//loop through and reposition all sprites according to x and y
	int ctr;
	int x = status->screenX;
	int y = status->screenY;
	characterFrameDef * currFrame = status->currentFrame;
	for (ctr = 0; ctr < status->listSize; ctr++)
	{
		if (currFrame != null)
		{
			setSpritePosition(status->spriteList[ctr], 
					x + currFrame->xOffset, y + currFrame->yOffset);
			//currFrame = currFrame->nextSprite;
			currFrame = nextFrame(currFrame);
		}
	}
}

void stopAnimation(CharacterStatus * status)
{
	status->frameCounter = 0;
}

int checkWallCollision(CharacterStatus * status)
{
	int xCheck = status->x + status->boundingBlockOffsetX;
	int yCheck = status->y + status->boundingBlockOffsetY;
	
	
	xCheck = max(0,(xCheck)) >> 3;
	yCheck = max(0,(yCheck)) >> 3;
	int yStart = yCheck;
	//int xStart = xCheck;
	
	

	int numXChecks = status->boundingBlockWidth >> 3;
	int numYChecks = status->boundingBlockHeight >> 3;

	int i;
	int j;
	int ctr = 0;
	int mobility = status->mobility;
	int collideCorners = 0;
	int xCorner = -1;
	for (i = 0; i <= numXChecks; i++)
	{
		if (i == numXChecks)
		{
			xCheck = getX2(status) >> 3;
			xCorner = 1;
		}
		int yCorner = -1;
		for (j = 0; j <= numYChecks; j++)
		{
			if (j == numYChecks)
			{
				yCheck = getY2(status) >> 3;
				yCorner = 1;
			}

			if ((tileIsWallCollision(xCheck,yCheck, mobility)))
			{
				//if a corner, mark where....
				if (xCorner == 1)
				{
					if (yCorner == -1)
					{
						collideCorners |= COLLIDE_TR;
					}
					else
					if (yCorner == 1)
					{
						collideCorners |= COLLIDE_BR;
					}
				}
				else
				if (xCorner == -1)
				{
					if (yCorner == -1)
					{
						collideCorners |= COLLIDE_TL;
					}
					else
					if (yCorner == 1)
					{
						collideCorners |= COLLIDE_BL;
					}
				}
				collideCorners |= COLLIDE_WAS_COLLISION;

			}
			yCheck++;
			ctr++;
			yCorner = 0;
		}
		yCheck = yStart;
		xCheck++;

		xCorner = 0;
	}
	return collideCorners;
	//int x2 = getX2(status) >> 3;
	//int y2 = getY2(status) >> 3;
	//return (tileIsWallCollision(x2, y2, waterWalk) ||
	        //tileIsWallCollision(xStart, y2, waterWalk) ||
		    //tileIsWallCollision(x2, yStart, waterWalk));

	//return false;

	//return false;
}

bool charactersCollide(CharacterStatus * char1, CharacterStatus * char2)
{
	int char1x1 = char1->x + char1->boundingBlockOffsetX;
	int char1y1 = char1->y + char1->boundingBlockOffsetY;

	int char1x2 = char1x1 + char1->boundingBlockWidth;
	int char1y2 = char1y1 + char1->boundingBlockHeight;

	int char2x1 = char2->x + char2->boundingBlockOffsetX;
	int char2y1 = char2->y + char2->boundingBlockOffsetY;

	if ((char2x1 > char1x2) || (char2y1 > char1y2))
	{
		return false;	
	}
	
	int char2x2 = char2x1 + char2->boundingBlockWidth;
	int char2y2 = char2y1 + char2->boundingBlockHeight;

	if ((char1x1 > char2x2) || (char1y1 > char2y2))
	{
		return false;
	}



	return true;
}

void setBoundingBlock(CharacterStatus * status, int w, int h, int x, int y)
{
	status->boundingBlockOffsetX = x;
	status->boundingBlockOffsetY = y;
	status->boundingBlockWidth = w;
	status->boundingBlockHeight = h;
}

void setCharacterBulletImmunityFinished(CharacterStatus * status)
{
	setCharacterBulletImmunity(status, null);
}

void setCharacterBulletImmunity(CharacterStatus * status, BulletDef * bullet)
{
	status->lastBulletHitBy = bullet;
}

bool isCharacterImmuneToBullet(CharacterStatus * status, BulletDef * bullet)
{
	return (bullet == status->lastBulletHitBy);
}

