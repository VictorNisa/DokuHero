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
	matrix[COLLIDER_WALL][COLLIDER_JUMPABLE] = false;

	matrix[COLLIDER_CHARA][COLLIDER_WALL] = true;
	matrix[COLLIDER_CHARA][COLLIDER_CHARA] = false;
	matrix[COLLIDER_CHARA][COLLIDER_DEATH] = true;
	matrix[COLLIDER_CHARA][COLLIDER_JUMPABLE] = true;

	matrix[COLLIDER_DEATH][COLLIDER_WALL] = false;
	matrix[COLLIDER_DEATH][COLLIDER_CHARA] = true;
	matrix[COLLIDER_DEATH][COLLIDER_DEATH] = false;
	matrix[COLLIDER_DEATH][COLLIDER_JUMPABLE] = false;

	matrix[COLLIDER_JUMPABLE][COLLIDER_WALL] = false;
	matrix[COLLIDER_JUMPABLE][COLLIDER_CHARA] = true;
	matrix[COLLIDER_JUMPABLE][COLLIDER_DEATH] = false;
	matrix[COLLIDER_JUMPABLE][COLLIDER_JUMPABLE] = false;
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
	update_colliders();
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
				if (matrix[c1->type][c2->type] && c1->callback && c1->active)
				{
					c1->callback->OnColl(c1, c2);
				}
				if (matrix[c2->type][c1->type] && c2->callback && c2->active)
				{
					c2->callback->OnColl(c2, c1);
				}
			}
		}
	}
	return UPDATE_CONTINUE;
}

//VICTOR: Add colliders
int j1Coll::closest_x_coll()
{
	int now;
	int close;
	if (App->chara->front) 
	{
		close = App->map->data.width*App->map->data.tile_width;
		for (int i = 0; colliders[i]!=nullptr; i++)
		{
			if ((colliders[i]->type == COLLIDER_WALL) && (colliders[i]->active) && (colliders[i]->rect.x > App->chara->origin_distance_chara.x))
			{
				now = colliders[i]->rect.x - App->chara->origin_distance_chara.x;
				if (now < close)
				{
					close = now;
				}
			}
		}
	}
	else
	{
		close = App->map->data.width*App->map->data.tile_width;
		for (int i = 0; colliders[i] != nullptr; i++)
		{
			if ((colliders[i]->type == COLLIDER_WALL) && (colliders[i]->active) && ((colliders[i]->rect.x + colliders[i]->rect.w) < App->chara->origin_distance_chara.x))
			{
				now = App->chara->origin_distance_chara.x - (colliders[i]->rect.x + colliders[i]->rect.w);
				if (now < close)
				{
					close = now;
				}
			}
		}
	}
	return close;
}

int j1Coll::closest_y_coll() {
	int x = 0;
	return x;
}

bool j1Coll::on_the_way_x(int index) {
	bool not_on_the_way;
	if (App->chara->characollider->active) {
		not_on_the_way = ((App->chara->characollider->rect.y + App->chara->characollider->rect.h) < colliders[index]->rect.y)
			|| ((App->chara->characollider->rect.y) > (colliders[index]->rect.y + colliders[index]->rect.h));
	}
	else if (App->chara->lizardcollider->active) {
		not_on_the_way = ((App->chara->lizardcollider->rect.y + App->chara->lizardcollider->rect.h) < colliders[index]->rect.y)
			|| ((App->chara->lizardcollider->rect.y) > (colliders[index]->rect.y + colliders[index]->rect.h));

	}
	return !not_on_the_way;
}

void j1Coll::update_colliders()
{
	for (int i = 0; i < MAX_COLLIDERS; i++)
	{
		while (colliders[i] != nullptr)
		{
			if (colliders[i]->type == COLLIDER_JUMPABLE)
			{
				if ((App->chara->characollider->rect.y + App->chara->characollider->rect.h) < colliders[i]->rect.y)
				{
					colliders[i]->active = true;
				}
				else
				{
					colliders[i]->active = false;
				}
			}
			i++;
		}
	}
}

bool j1Coll::Update(float dt)
{
	DebugDraw();
	return UPDATE_CONTINUE;
}

void j1Coll::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
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
		if (colliders[i]->active)
		{
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
				break;
			case COLLIDER_CHARA:
				App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
				break;
			case COLLIDER_DEATH:
				App->render->DrawQuad(colliders[i]->rect, 0, 0, 0, alpha);
				break;
			}
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