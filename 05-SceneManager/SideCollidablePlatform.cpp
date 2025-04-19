#include "SideCollidablePlatform.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

int CSideCollidablePlatform::IsDirectionColliable(float nx, float ny)
{
	if (nx == 0 && ny == -1) return 1;
	if (nx == -1 || nx == 1) return 1;
	else return 0;
}