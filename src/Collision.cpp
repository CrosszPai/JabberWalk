#include "Collision.h"
#include "ECS\ColliderComponent.h"

bool Collision::AABB(const SDL_Rect &recA, const SDL_Rect &recB)
{
	if (
		recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y)
	{
		return true;
	}

	return false;
}

bool Collision::AABB(const ColliderComponent &colA, const ColliderComponent &colB)
{
	if (AABB(colA.collider, colB.collider))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Collision::intersect(const int xpos, const int ypos, const SDL_Rect &rec)
{
	if (
		xpos >= rec.x &&
		xpos <= rec.x + rec.w &&
		ypos >= rec.y &&
		ypos <= rec.y + rec.h)
	{
		return true;
	}
	else
	{
		return false;
	}
}