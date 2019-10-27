#pragma once
#ifndef  _TRANSITION_
#define _TRANSITION_

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

enum MapNames;

class j1MapChange : public j1Module
{

public:

	j1MapChange();
	virtual ~j1MapChange();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);

	//bool ChangeMap(int newMap, float time);
	bool IsChanging() const;
	bool fading = false;

private:
	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	}
	current_step = fade_step::none;


	int Map2;
	uint start_time = 0;
	uint total_time = 0;
	SDL_Rect screen;


};
#endif // ! _TRANSITION_