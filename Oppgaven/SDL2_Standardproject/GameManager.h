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
	void gameLoopTimer(SDLBmp * player);
	void handleInput();
	void gameLoop(SDLBmp * player);
	void MoveToRandomPosition(SDLBmp * object);
	int roundToTenths(int x);
	int roundToFifty(int x);
	void draw(SDLBmp * player, SDLBmp * background, SDLBmp *apple);
private:

	GameManager();								// Hidden constructor
	GameManager(const GameManager&);			// Hidden copy constructor
	GameManager& operator=(const GameManager&); // Hidden assign operator

	 Direction currentDirection;
	 Direction nextDirection;
	 float RandomFloat(float, float);
	 int RandomInt(int a, int b);
	 bool isColliding(SDLBmp*, SDLBmp*);
	unsigned int m_window; // pointer to main window
	float m_lastRender; // Time in seconds since last render
	std::vector<SDLBmp> snake;
	std::vector<SDLBmp>::iterator snakeIterator;
};

#endif