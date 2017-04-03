#pragma once
#include"Vector2D.h"
#include "SDL/SDLBmp.h"
#include "SDL/SDLManager.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	Vector2D position;
	SDL_Texture * texture;
};

