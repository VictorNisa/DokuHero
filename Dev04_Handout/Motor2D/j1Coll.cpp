#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Coll.h"
#include "j1Module.h"
#include "p2Log.h"

j1Coll::j1Coll()
{
	for (uint i = 0; i < MAX_COLLIDERS; i++)
	{
		colliders[i] = nullptr;
	}
	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_CHARA] = true;
	matrix[COLLIDER_WALL][COLLIDER_DEATH] = false;

	matrix[COLLIDER_CHARA][COLLIDER_WALL] = true;
	matrix[COLLIDER_CHARA][COLLIDER_CHARA] = false;
	matrix[COLLIDER_CHARA][COLLIDER_DEATH] = true;

	matrix[COLLIDER_DEATH][COLLIDER_WALL] = false;
	matrix[COLLIDER_DEATH][COLLIDER_CHARA] = true;
	matrix[COLLIDER_DEATH][COLLIDER_DEATH] = false;
}

j1Coll::~j1Coll() {}

bool j1Coll::PreUpdate()
{
	for (uint i = 0; i < MAX_COLLIDERS; i++)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; i++)
	{
		//avoid empty colls
		if (colliders[i] == nullptr)
		{
			continue;
		}
		c1 = colliders[i];

		//don't check collisions twice
		for (uint j = 0; j < MAX_COLLIDERS; j++)
		{
			if (colliders[j] == nullptr)
			{
				continue;
			}
			c2 = colliders[j];

			if (c1->CheckCol(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
				{
					c1->callback->OnCollision(c1, c2);
				}
				if (matrix[c2->type][c1->type] && c2->callback)
				{
					c2->callback->OnCollision(c2, c1);
				}
			}
		}
	}
	return UPDATE_CONTINUE;
}

bool j1Coll::Update()
{
	DebugDraw();
	return UPDATE_CONTINUE;
}

void j1Coll::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN);
	{
		debug = !debug;
	}
	if (debug == false)
	{
		return;
	}
	Uint8 alpha = 140;
	for (uint i = 0; i < MAX_COLLIDERS; i++)
	{
		if (colliders[i] == nullptr)
		{
			continue;
		}
		switch (colliders[i]->type)
		{
		case COLLIDER_NONE:
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_WALL:
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_JUMPABLE:
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
		case COLLIDER_CHARA:
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
		case COLLIDER_DEATH:
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 0, alpha);
		}
	}
}

bool j1Coll::CleanUp()
{
	LOG("Cleaning colliders");

	for (uint i = 0; i < MAX_COLLIDERS; i++)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}

Collider* j1Coll::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;
	for (uint i = 0; i < MAX_COLLIDERS; i++)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}
	return ret;
}

bool Collider::CheckCol(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.y + rect.h > r.y);
}