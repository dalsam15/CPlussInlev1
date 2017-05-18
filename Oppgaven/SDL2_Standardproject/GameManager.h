/*
 * @file: GameManager.h
 * @author: Stig M. Halvorsen <halsti@nith.no>
 * @version: 1.0.0 <11.02.2013>
 *
 * @description: A singleton class to control all gameplay.
 */

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include "SDL/SDLBmp.h"
#include "SDL/SDLManager.h"
#include <vector>
#include "GameObject.h"

class GameManager
{
public:

	typedef enum Direction
	{
		up,
		down,
		left,
		right
	};


	/* Meyer's singleton style instance call */
	static GameManager& Instance()
	{
		static GameManager g;
		return g;
	}

	/* Kicks off/is the the gameloop */
	void play();
	void gameLoopTimer();
	void handleInput();
	void gameLoop();
	bool IsGameOver();
	Vector2D  RandomPos();
	//int roundToTenths(int x);
	//int roundToFifty(int x);
	void draw();
	void drawGameObject(GameObject);
	//void drawGameObject(GameObject gameObject, SDL_RendererFlip flip, float angle);
	//void drawGameObject(GameObject gameObject, bool flipX, bool flipY);
	void addSnakeBody();
	void showGameOver();
	bool withinBounds(const SDL_Rect & rect, int x, int y);
	void drawHeader();
private:

	GameManager();								// Hidden constructor
	GameManager(const GameManager&);			// Hidden copy constructor
	GameManager& operator=(const GameManager&); // Hidden assign operator

	 Direction currentDirection;
	 Direction nextDirection;
	 float RandomFloat(float, float);
	 int RandomInt(int a, int b);
	 bool isColliding(GameObject a, GameObject b);
	unsigned int m_window; // pointer to main window
	unsigned int m_gameOverWindow;
	float m_lastRender; // Time in seconds since last render
	std::vector<GameObject > snake;
	std::vector<GameObject >::iterator snakeIterator;
	GameObject appleObject;


	SDL_Surface*  snake_surface;
	SDL_Surface*  apple_surface;
	//SDL_Surface* gameOver_surface;

	SDL_Texture*  snake_texture;
	SDL_Texture*  apple_texture;
	//SDL_Texture*  gameOver_texture;

	SDL_Renderer* renderer;

	int score;
	//lol
};

#endif