#include "Probe.h"
void CProbe::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - PROBE_BB_WIDTH / 2;
	top = y - PROBE_BB_HEIGHT / 2;
	right = left + PROBE_BB_WIDTH;
	bottom = top + PROBE_BB_HEIGHT;
}
void CProbe::Render() {
	RenderBoundingBox();
}
void CProbe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (y > (y0 + PROBE_FALL_THRESHOLD) || y < y0) Reset();
	x = x0;
	y += vy * dt;
	vy += ay * dt;

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CProbe::Delete() {
	//override so it is not allowed to delete itself, only the npc it is attached to can delete it
}
void CProbe::Delete(LPGAMEOBJECT obj) {
	isDeleted = true;
}