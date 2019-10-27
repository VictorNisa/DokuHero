#include "j1App.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Coll.h"
#include "j1Chara.h"
#include "p2Log.h"
#include "j1Textures.h"

j1Chara::j1Chara() :j1Module(), chara_loaded(false)
{
	name.create("character");
}

j1Chara::~j1Chara()
{

}

bool j1Chara::Awake(pugi::xml_node&config)
{
	LOG("Loading Character Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

SDL_Rect&CharaAnimation::GetCurrentFrame() {
	current_frame += frames->duration;
	if (current_frame >= total_frames)
		current_frame = 0;
	return frames[(int)current_frame].rect;
}

void j1Chara::LoadAnimations(pugi::xml_node&node)
{
	LOG("Loading character animations");
	for (node; node; node = node.next_sibling("tile")) {
		CharaAnimation*animation = new CharaAnimation();
		animation->name = node.child("properties").child("property").attribute("value").as_string();
		animation->total_frames = node.child("properties").child("property").next_sibling("property").attribute("value").as_uint();
		LOG("Loading %s animation with %d frames", animation->name, animation->total_frames);
		animation->texture = sprite_tilesets.start->data->texture;
		pugi::xml_node frame_node = node.child("animation").child("frame");
		animation->frames = new Frame[animation->total_frames];
		for (int i = 0; i < animation->total_frames; frame_node = frame_node.next_sibling("frame"), ++i) {
			uint tileset_id = frame_node.attribute("tileid").as_uint();
			animation->frames[i].duration = frame_node.attribute("duration").as_float() / 100;
			animation->frames[i].rect = sprite_tilesets.start->data->TilesetRect(1 + tileset_id);
		}
		Animations.add(animation);
		LOG("Succesfully loaded %s animation", animation->name);
	}
}

void j1Chara::Updateposition(chara_states state)
{
	p2Point<float>distance;
	switch (state) {
	case ST_IDLE_FORWARD:
		speed.x = 0;
		front = true;
		break;
	case ST_IDLE_BACKWARD:
		speed.x = 0;
		front = false;
		break;
	case ST_W_FORWARD:
		speed.x = 20;
		origin_distance_chara.x = characollider->rect.x + characollider->rect.w;
		front = true;
		break;
	case ST_W_BACKWARD:
		speed.x = -20;
		origin_distance_chara.x = characollider->rect.x;
		front = false;
		break;
	}
	distance.x = App->collisions->closest_x_coll();
	LOG("D: %f", distance.x);
	LOG("S: %f", speed.x);
	if (front) {
		if (speed.x >= distance.x) {
			position.x += distance.x - 1;
		}
		else {
			position.x += speed.x;
		}
	}
	else {
		if (speed.x <= -distance.x) {
			position.x += -distance.x + 1;
		}
		else {
			position.x += speed.x;
		}
	}
	position.y += speed.y;
	characollider->SetPos(position.x - (coll_offset_x), position.y - (coll_offset_y));
	lizardcollider->SetPos(position.x - (coll_offset_x), position.y - (coll_offset_y - (characollider->rect.h - lizardcollider->rect.h)));
}

void j1Chara::Draw_chara(chara_states state)
{
	switch (state)
	{
	case ST_IDLE_FORWARD:
		App->render->Blit(Animations.start->data->texture, position.x, position.y, &Animations.start->data->GetCurrentFrame());
		break;
	case ST_IDLE_BACKWARD:
		App->render->Blit(Animations.start->data->texture, position.x, position.y, &Animations.start->data->GetCurrentFrame(), SDL_FLIP_HORIZONTAL, sprite_tilesets.start->data);
		break;
	case ST_W_FORWARD:
		App->render->Blit(Animations.start->data->texture, position.x, position.y, &Animations.start->next->data->GetCurrentFrame());
		break;
	case ST_W_BACKWARD:
		App->render->Blit(Animations.start->data->texture, position.x, position.y, &Animations.start->next->data->GetCurrentFrame(), SDL_FLIP_HORIZONTAL, sprite_tilesets.start->data);
		break;
	}
}

void j1Chara::OnColl(Collider*chara, Collider*wall)
{

}

chara_states j1Chara::current_chara_state(p2Queue<chara_inputs>& inputs)
{
	static chara_states state = ST_IDLE_FORWARD;
	chara_inputs last_input;
	while (inputs.Pop(last_input))
	{
		switch (state)
		{
		case ST_IDLE_FORWARD:
		{
			switch (last_input)
			{
			case IN_RIGHT_DOWN:
				state = ST_W_FORWARD;
				break;
			case IN_LEFT_DOWN:
				state = ST_W_BACKWARD;
				break;
			case IN_JUMP:
				state = ST_J_UP;
				break;
			}
			break;
		}
		case ST_IDLE_BACKWARD:
		{
			switch (last_input)
			{
			case IN_RIGHT_DOWN:
				state = ST_W_FORWARD;
				break;
			case IN_LEFT_DOWN:
				state = ST_W_BACKWARD;
				break;
			case IN_JUMP:
				state = ST_J_UP;
				break;
			}
			break;
		}
		case ST_W_FORWARD:
		{

			switch (last_input)
			{
			case IN_RIGHT_UP:
				state = ST_IDLE_FORWARD;
				break;
			case IN_R_AND_L:
				state = ST_IDLE_FORWARD;
				break;
			case IN_JUMP:
				state = ST_J_FORWARD;
				break;
			case IN_LIZARD_DOWN:
				state = ST_L_FORWARD;
				break;
			}
			break;
		}
		case ST_W_BACKWARD:
		{

			switch (last_input)
			{
			case IN_LEFT_UP:
				state = ST_IDLE_BACKWARD;
				break;
			case IN_R_AND_L:
				state = ST_IDLE_BACKWARD;
				break;
			case IN_JUMP:
				state = ST_J_BACKWARD;
				break;
			case IN_LIZARD_DOWN:
				state = ST_L_BACKWARD;
				break;
			}
			break;
		}
		case ST_J_UP:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH:
				if (front = true)
				{
					state = ST_IDLE_FORWARD;
				}
				else
				{
					state = ST_IDLE_BACKWARD;
				}
				break;
			}
			break;
		}
		case ST_J_FORWARD:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH:
				state = ST_IDLE_FORWARD;
				break;
			}
			break;
		}
		case ST_J_BACKWARD:
		{
			switch (last_input)
			{
			case IN_JUMP_FINISH:
				state = ST_IDLE_BACKWARD;
				break;
			}
			break;
		}
		case ST_L_FORWARD:
		{
			switch (last_input)
			{
			case IN_LIZARD_FINISH:
				state = ST_IDLE_FORWARD;
				break;
			}
			break;
		}
		case ST_L_BACKWARD:
		{
			switch (last_input)
			{
			case IN_LIZARD_FINISH:
				state = ST_IDLE_BACKWARD;
				break;
			}
			break;
		}
		break;
		}
	}
	return state;
}

bool j1Chara::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = chara_doc.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load chara xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}
	if (ret == true)
	{
		bool ret = true;
		pugi::xml_node chara_node = chara_doc.child("map").child("tileset");
		if (chara_node == NULL)
		{
			LOG("Error parsing character xml file: Cannot find 'tileset' tag.");
			ret = false;
		}
		else
		{
			pugi::xml_node tileset;
			for (tileset = chara_node; tileset && ret; tileset = tileset.next_sibling("tileset"))
			{
				TileSet* set = new TileSet();

				if (ret == true)
				{
					ret = App->map->LoadTilesetDetails(tileset, set);
				}

				if (ret == true)
				{
					ret = App->map->LoadTilesetImage(tileset, set, this);
				}

				sprite_tilesets.add(set);
			}
			chara_node = chara_node.child("tile");
			LoadAnimations(chara_node);
			Load_chara_info();
		}
		return ret;
	}

}

/*void j1Chara::LoadCharaPosition()
{
	p2SString start;
	start.create("Start");
	bool escape = false;
	p2List_item<objectgroup*>*it = App->map->data.objectgroup.start;
	while (it != NULL)
	{
		objectgroup*ptr = it->data;
		for (int i = 0; i < ptr->num_objects; i++)
		{
			if (ptr->objects[i].name == start)
			{
				position.x = ptr->objects[i].rect.x;
				position.y = ptr->objects[i].rect.y;
				escape = true;
				break;
			}
		}
		if (escape)
		{
			break;
		}
		it = it->next;
	}
}*/

void j1Chara::Load_chara_info()
{
	p2SString group_name; group_name.create("COLLIDER_CHARA");
	p2SString start; start.create("Start");
	p2SString chara; chara.create("characollider");
	p2SString lizard; lizard.create("lizardcollider");
	p2List_item<objectgroup*>*it = App->map->data.objectgroup.start;
	while (it != NULL) 
	{
		if (it->data->name == group_name) 
		{
			for (int i = 0; i < it->data->num_objects; ++i) 
			{
				if (it->data->objects[i].name == start) 
				{
					position.x = it->data->objects[i].rect.x;
					position.y = it->data->objects[i].rect.y;
				}
				else if (it->data->objects[i].name == chara) 
				{
					characollider = App->collisions->AddCollider(it->data->objects[i].rect, COLLIDER_CHARA, App->chara);
				}
				else if (it->data->objects[i].name == lizard) 
				{
					lizardcollider = App->collisions->AddCollider(it->data->objects[i].rect, COLLIDER_CHARA, App->chara);
					lizardcollider->active = false;
				}
			}
			break;
		}
		it = it->next;
	}
	coll_offset_x = position.x - characollider->rect.x;
	coll_offset_y = position.y - characollider->rect.y;
}

bool j1Chara::PostUpdate() {
	camerapos();
	return true;
}

bool j1Chara::CleanUp()
{
	key_inputs.Clear();
	p2List_item<CharaAnimation*>* item;
	item = Animations.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}

	Animations.clear();
	p2List_item<TileSet*>* it;
	it = sprite_tilesets.start;

	while (it != NULL)
	{
		RELEASE(it->data);
		it = it->next;
	}
	sprite_tilesets.clear();
	chara_doc.reset();
	return true;
}