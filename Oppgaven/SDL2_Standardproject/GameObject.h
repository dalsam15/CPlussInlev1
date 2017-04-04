#pragma once
#include"Vector2D.h"
#include "SDL/SDLBmp.h"
#include "SDL/SDLManager.h"
#include "Transform.h"
class GameObject
{
public:
	GameObject();
	~GameObject();

	Vector2D position;
	SDL_Texture * texture;
	Transform transform;
};

