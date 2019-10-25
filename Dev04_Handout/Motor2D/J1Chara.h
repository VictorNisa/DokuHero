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

	ST_IDLE,
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
	const char* name;
	uint total_frames;
	float current_frame;
	SDL_Texture* texture;
	Frame* frames;
	SDL_Rect& GetCurrentFrame();
};

class j1Chara :public j1Module
{
public:
	j1Chara();
	~j1Chara();
	bool Awake(pugi::xml_node&config);

	bool Load(const char* file_name);
	void LoadAnimations(pugi::xml_node&node);
	void j1Chara::LoadCharaPosition();
	void Draw_chara();
	void Updateposition();
	bool CleanUp();
	p2Point<float> speed;
	p2Queue<chara_inputs> key_inputs;
	chara_states j1Chara::current_chara_state(p2Queue<chara_inputs>& inputs);
	Collider* characollider = nullptr;

private:
	pugi::xml_document chara_doc;
	p2SString folder;
	bool chara_loaded;
	p2List<TileSet*>sprite_tilesets;
	p2List<CharaAnimation*>Animations;
	p2Point<float> position;
	friend class j1Map;
};
#endif // __j1CHARA_H__
