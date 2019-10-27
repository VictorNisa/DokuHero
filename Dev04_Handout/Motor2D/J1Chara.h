#ifndef __j1CHARA_H__
#define __j1CHARA_H__
#include "j1Module.h"
#include "p2List.h"
#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL_render.h"
#include "j1Map.h"
#include "p2Queue.h"

enum chara_states
{
	ST_UNKNOWN,

	ST_IDLE_FORWARD,
	ST_IDLE_BACKWARD,
	ST_W_FORWARD,
	ST_W_BACKWARD,
	ST_R_FORWARD,
	ST_R_BACKWARD,
	ST_J_UP,
	ST_J_FORWARD,
	ST_J_BACKWARD,
	ST_L_FORWARD,
	ST_L_BACKWARD
};

enum chara_inputs
{
	IN_LEFT_DOWN,
	IN_LEFT_UP,
	IN_RIGHT_DOWN,
	IN_RIGHT_UP,
	IN_R_AND_L,
	IN_JUMP,
	IN_LIZARD_DOWN,
	IN_LIZARD_UP,
	IN_JUMP_FINISH,
	IN_LIZARD_FINISH
};

struct Frame {
	SDL_Rect rect;
	float duration;
};

struct CharaAnimation {
	p2SString name;
	uint total_frames;
	float current_frame;
	SDL_Texture* texture;
	Frame* frames;
	SDL_Rect& GetCurrentFrame();
	SDL_Rect& DoOneLoop();
};

class j1Chara :public j1Module
{
public:
	j1Chara();
	~j1Chara();
	bool Awake(pugi::xml_node&config);

	bool PostUpdate();
	bool Load(const char* file_name);
	void LoadAnimations(pugi::xml_node&node);
	//void j1Chara::LoadCharaPosition();
	void Draw_chara(chara_states state);
	void Updateposition(chara_states state);
	bool CleanUp();
	p2Point<float> speed;
	p2Queue<chara_inputs> key_inputs;
	chara_states j1Chara::current_chara_state(p2Queue<chara_inputs>& inputs);
	void Load_chara_info();
	Collider* characollider = nullptr;
	Collider* lizardcollider = nullptr;
	void OnColl(Collider*chara, Collider*wall);
//	void change_chara_collider(chara_states state);
	p2Point<float> origin_distance_chara;
	bool front = true;
	bool camerapos();
	float lizard;
	p2Point<float>distance;

private:
	pugi::xml_document chara_doc;
	p2SString folder;
	bool chara_loaded;
	p2List<TileSet*>sprite_tilesets;
	p2List<CharaAnimation*>Animations;
	p2Point<float> position;
	int coll_offset_x;
	int coll_offset_y;
	int coll_lizard_offset_x;
	friend class j1Map;
};
#endif // __j1CHARA_H__
