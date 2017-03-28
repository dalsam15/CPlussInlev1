/*
 * @file: GameManager.cpp
 * @author: Stig M. Halvorsen <halsti@nith.no>
 * @version: 1.0.0 <11.02.2013>
 *
 * @description: A singleton class to control all gameplay.
 */

#include "SDL/SDLBmp.h"
#include "GameManager.h"
#include "InputManager.h"
#include "Timer.h"
#include <iostream>

using namespace std;
/* Initializes SDL, creates the game window and fires off the timer. */
GameManager::GameManager()
{
	SDLManager::Instance().init();
	m_window = SDLManager::Instance().createWindow("My Awesome SDL 2.0 Game");
	Timer::Instance().init();
}

void GameManager::play()
{
	bool notGameOver = true;

	// Load bitmaps
	SDLBmp background("Assets/gfx/sdl2.bmp");
	SDLBmp player("Assets/gfx/sdl_bro.bmp");

	background.draw();

	// Calculate render frames per second (second / frames) (60)
	float render_fps = 1.f / 60.f;
	m_lastRender = render_fps; // set it to render immediately

	while (notGameOver) {
		handleInput();
		gameLoopTimer(&player);
		draw(&player, &background);

	}
}

void GameManager::gameLoopTimer(SDLBmp * player) {
	Timer::Instance().update();
	if (Timer::Instance().elapsedTime() > 1) {
		Timer::Instance().resetTime();
		cout << Timer::Instance().elapsedTime();
		//gameLoopTimer(player);
		gameLoop(player);
	}
}

void GameManager::handleInput() {
		 /* Input Management */

		 // Left key
		 SDL_Event e;
		 SDL_PollEvent(&e);
		 if (e.type == SDL_KEYDOWN) {
			 switch (e.key.keysym.sym) {
			 case SDLK_UP:
				 if (currentDirection != down) {
					 nextDirection = up;
				 }
				 break;
			 case SDLK_DOWN:
				 if (currentDirection != up) {
					 nextDirection = down;
				 }
				 break;
			 case SDLK_RIGHT:
				 if (currentDirection != left) {
					 nextDirection = right;
				 }
				 break;
			 case SDLK_LEFT:
				 if (currentDirection != right) {
					 nextDirection = left;
				 }
				 break;
			 }
		 }

	 }


 void GameManager::gameLoop(SDLBmp * player) {
	 currentDirection = nextDirection;
	 switch (currentDirection) {
	 case left:
		 player->x -= 15;
		 cout << "l!";
		 break;
	 case right:
		 player->x += 15;
		 cout << "r!";
		 break;
	 case up:
		 player->y -= 15;
		 cout << "u!";
		 break;
	 case down:
		 player->y += 15;
		 cout << "d!";
		 break;
	 }

	 cout << "next direction";
		 // Add bitmaps to renderer
 }

 void GameManager::draw(SDLBmp * player, SDLBmp * background) {
	 if (m_lastRender >= 1 / 60);
	 {
		 // Add bitmaps to renderer
		 background->draw();
		 player->draw();
		// nextCube.draw();

		 // Render window
		 SDLManager::Instance().renderWindow(m_window);
		 m_lastRender = 0.f;

		 SDLManager::Instance().renderWindow(m_window);

	 }
 }