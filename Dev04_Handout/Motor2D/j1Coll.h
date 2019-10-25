#pragma once
#ifndef __j1Coll_H__
#define __j1Coll_H__

#define MAX_COLLIDERS 100

#include "j1Module.h"

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_WALL,
	COLLIDER_JUMPABLE,
	COLLIDER_CHARA,
	COLLIDER_DEATH,

	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}
	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}
	bool CheckCol(const SDL_Rect& r) const;
};

class j1Coll : public j1Module
{
public:
	j1Coll();
	~j1Coll();

	bool PreUpdate() override;
	bool Update();
	bool CleanUp() override;

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);
	void DebugDraw();

private:
	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;
};

#endif // !__j1Coll_H__