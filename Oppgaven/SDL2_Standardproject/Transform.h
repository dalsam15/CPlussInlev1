#pragma once
#include <SDL.h>
class Transform
{
public:
	Transform();
	~Transform();

	SDL_RendererFlip flip;
	float rotation;
};

