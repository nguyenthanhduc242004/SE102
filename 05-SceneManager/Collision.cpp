#include "Collision.h"
#include "GameObject.h"

#include "debug.h"

#define BLOCK_PUSH_FACTOR 0.01f

CCollision* CCollision::__instance = NULL;

int CCollisionEvent::WasCollided() {
	return t >= 0.0f && t <= 1.0f && obj->IsDirectionCollidable(nx, ny) == 1;
}

CCollision* CCollision::GetInstance()
{
	if (__instance == NULL) __instance = new CCollision();
	return __instance;
}

/*
	SweptAABB
*/
void CCollision::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float& t, float& nx, float& ny)
{

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -9999999.0f;
		tx_exit = 99999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -99999999999.0f;
		ty_exit = 99999999999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}

}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT CCollision::SweptAABB(LPGAMEOBJECT objSrc, DWORD dt, LPGAMEOBJECT objDest)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	float mvx, mvy;
	if (dynamic_cast<CMoveable*>(objSrc)) {
		dynamic_cast<CMoveable*>(objSrc)->GetSpeed(mvx, mvy);
	}
	else
	{
		//the object is immoveable->has no velocity, same for the below
		mvx = mvy = 0.0f;
	}
	float mdx = mvx * dt;
	float mdy = mvy * dt;

	float svx, svy;
	if (dynamic_cast<CMoveable*>(objDest)) {
		dynamic_cast<CMoveable*>(objDest)->GetSpeed(svx, svy);
	}
	else
	{
		svx = svy = 0.0f;
	}
	float sdx = svx * dt;
	float sdy = svy * dt;

	//
	// NOTE: new m speed = original m speed - collide object speed
	// 
	float dx = mdx - sdx;
	float dy = mdy - sdy;

	objSrc->GetBoundingBox(ml, mt, mr, mb);
	objDest->GetBoundingBox(sl, st, sr, sb);

	SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, dx, dy, objDest, objSrc);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void CCollision::Scan(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* objDests, vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < objDests->size(); i++)
	{
		LPGAMEOBJECT objDest = objDests->at(i);

		// Skip self-collision
		if (objDest == objSrc) continue;

		//  FIX: Skip deleted or non-collidable objects
		//	-> FIX: Need to process non-collidable objects like Bullet and PiranhaPlant
		if (objDest->IsDeleted()) continue;

		// Run SweptAABB only for valid targets
		LPCOLLISIONEVENT e = SweptAABB(objSrc, dt, objDest);

		if (e->WasCollided() == 1)
			coEvents.push_back(e);
		else
			delete e;
	}
}

void CCollision::Filter(LPGAMEOBJECT objSrc,
	vector<LPCOLLISIONEVENT>& coEvents,
	LPCOLLISIONEVENT& colX,
	LPCOLLISIONEVENT& colY,
	int filterBlock = 1,		// 1 = only filter block collisions, 0 = filter all collisions 
	int filterX = 1,			// 1 = process events on X-axis, 0 = skip events on X 
	int filterY = 1)			// 1 = process events on Y-axis, 0 = skip events on Y
{
	float min_tx, min_ty;

	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];
		if (c->isDeleted) continue;
		if (c->obj->IsDeleted()) continue;

		// ignore collision event with object having IsBlocking = 0 (like coin, mushroom, etc)
		if (filterBlock == 1 && !c->obj->IsBlocking())
		{
			continue;
		}

		if (c->t < min_tx && c->nx != 0 && filterX == 1) {
			min_tx = c->t; min_ix = i;
		}

		if (c->t < min_ty && c->ny != 0 && filterY == 1) {
			min_ty = c->t; min_iy = i;
		}
	}

	if (min_ix >= 0) colX = coEvents[min_ix];
	if (min_iy >= 0) colY = coEvents[min_iy];
}

/*
*  Simple/Sample collision framework
*  NOTE: Student might need to improve this based on game logic
*/

void CCollision::Process(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	LPCOLLISIONEVENT colX = NULL;
	LPCOLLISIONEVENT colY = NULL;
	std::unordered_set<LPGAMEOBJECT> collidedObjects;

	coEvents.clear();

	if (objSrc->IsCollidable())
	{
		Scan(objSrc, dt, coObjects, coEvents);
	}

	if (coEvents.size() == 0)
	{
		objSrc->OnNoCollision(dt);
	}
	else
	{
		Filter(objSrc, coEvents, colX, colY);

		float x, y, vx, vy, dx, dy;
		objSrc->GetPosition(x, y);
		if (dynamic_cast<CMoveable*>(objSrc)) {
			dynamic_cast<CMoveable*>(objSrc)->GetSpeed(vx, vy);
		}
		else {
			vx = vy = 0;
		}
		dx = vx * dt;
		dy = vy * dt;

		if (colX != NULL && colY != NULL)
		{
			if (colY->t < colX->t)
			{
				y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
				objSrc->SetPosition(x, y);

				objSrc->OnCollisionWith(colY);
				collidedObjects.insert(colY->obj);

				LPCOLLISIONEVENT colX_other = NULL;
				colX->isDeleted = true;
				coEvents.push_back(SweptAABB(objSrc, dt, colX->obj));

				Filter(objSrc, coEvents, colX_other, colY, 1, 1, 0);

				if (colX_other != NULL)
				{
					x += colX_other->t * dx + colX_other->nx * BLOCK_PUSH_FACTOR;
					objSrc->OnCollisionWith(colX_other);
					collidedObjects.insert(colX_other->obj);
				}
				else
				{
					x += dx;
				}
			}
			else
			{
				x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
				objSrc->SetPosition(x, y);

				objSrc->OnCollisionWith(colX);
				collidedObjects.insert(colX->obj);

				LPCOLLISIONEVENT colY_other = NULL;
				colY->isDeleted = true;
				coEvents.push_back(SweptAABB(objSrc, dt, colY->obj));

				Filter(objSrc, coEvents, colX, colY_other, 1, 0, 1);

				if (colY_other != NULL)
				{
					y += colY_other->t * dy + colY_other->ny * BLOCK_PUSH_FACTOR;
					objSrc->OnCollisionWith(colY_other);
					collidedObjects.insert(colY_other->obj);
				}
				else
				{
					y += dy;
				}
			}
		}
		else if (colX != NULL)
		{
			x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
			y += dy;
			objSrc->OnCollisionWith(colX);
			collidedObjects.insert(colX->obj);
		}
		else if (colY != NULL)
		{
			x += dx;
			y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
			objSrc->OnCollisionWith(colY);
			collidedObjects.insert(colY->obj);
		}
		else
		{
			x += dx;
			y += dy;
		}

		objSrc->SetPosition(x, y);
	}

	// Non-blocking collisions (e.g., coins, triggers)
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = coEvents[i];
		if (e->isDeleted) continue;
		if (e->obj->IsBlocking()) continue;

		objSrc->OnCollisionWith(e);
		collidedObjects.insert(e->obj);
	}

	// Static overlap detection (only for new, unhandled, non-blocking collisions)
	if (objSrc->IsCollidable())
	{
		for (LPGAMEOBJECT objDest : *coObjects)
		{
			if (objDest == objSrc) continue;
			if (objDest->IsDeleted() || !objDest->IsCollidable()) continue;
			if (collidedObjects.count(objDest) > 0) continue;

			float sl, st, sr, sb;
			float ml, mt, mr, mb;

			objSrc->GetBoundingBox(ml, mt, mr, mb);
			objDest->GetBoundingBox(sl, st, sr, sb);

			bool isOverlapping = !(mr < sl || ml > sr || mb < st || mt > sb);

			if (isOverlapping)
			{
				CCollisionEvent* overlapEvent = new CCollisionEvent(1.0f, 0, 0, 0, 0, objDest, objSrc);
				objSrc->OnCollisionWith(overlapEvent);
				delete overlapEvent;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
