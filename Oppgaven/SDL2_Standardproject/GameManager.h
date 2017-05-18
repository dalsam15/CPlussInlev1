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
#include <string>

using namespace std;

class GameManager
{
public:

	enum Direction
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

	void initalizeNewGame();
	void modifyRect(SDL_Rect * rect, int x, int y, int width, int height);
	SDL_Texture * createTextureFromPath(string path);
	void initalizeGameSDL();
	void play();
	void gameLoopTimer();
	void handleInput();
	void moveAppleToRandomPos();
	void gameLoop();
	bool IsGameOver();
	Vector2D  RandomPos();
	void drawGame();
	void drawGameObject(GameObject);
	void addSnakeBody();
	void showGameOver();
	bool withinBounds(const SDL_Rect & rect, int x, int y);
	void drawGameOverScore();
	void drawGameHeader();
private:

	GameManager();								// Hidden constructor
	GameManager(const GameManager&);			// Hidden copy constructor
	GameManager& operator=(const GameManager&); // Hidden assign operator

	 Direction currentDirection;
	 Direction nextDirection;
	 float RandomFloat(float, float);
	 bool isColliding(GameObject a, GameObject b);
	unsigned int m_window; // pointer to main window

	float frameRateDelay = 0.25f;
	bool running = true;
	bool notGameOver = true;
	int score = 0;
	GameObject appleObject;
	std::vector<GameObject > snake;
	std::vector<GameObject >::iterator snakeIterator;
};

#endif