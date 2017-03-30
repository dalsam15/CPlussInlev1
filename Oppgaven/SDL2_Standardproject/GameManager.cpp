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

float GameManager:: RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}
bool GameManager::isColliding(SDLBmp *a, SDLBmp *b) {
	float xDist = abs(abs(a->x - b->x) - 25);
	float yDist = abs(abs(a->y - b->y) - 25);
	float totaltDist = xDist + yDist;
//	cout << totaltDist << endl;
	if (totaltDist < 40) {
		return true;
	}
	return false;
}

void GameManager::play()
{
	bool notGameOver = true;

	// Load bitmaps
	SDLBmp background("Assets/gfx/sdl2.bmp");
	SDLBmp player("Assets/gfx/sdl_bro.bmp");
	SDLBmp apple("Assets/gfx/sdl_bro.bmp");

	apple.x = RandomFloat(1.f, 50.f); // fra 0 til 350
	apple.y = RandomFloat(1.f, 50.f); // fra o til 550
	
	background.draw();
	apple.draw();

	// Calculate render frames per second (second / frames) (60)
	float render_fps = 1.f / 60.f;
	m_lastRender = render_fps; // set it to render immediately

	while (notGameOver) {
		handleInput();
		gameLoopTimer(&player);
		draw(&player, &background, &apple);

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
		 player->x -= 10;
		 cout << "l!";
		 break;
	 case right:
		 player->x += 10;
		 cout << "r!";
		 break;
	 case up:
		 player->y -= 10;
		 cout << "u!";
		 break;
	 case down:
		 player->y += 10;
		 cout << "d!";
		 break;
	 }

	 cout << "next direction";
		 // Add bitmaps to renderer
 }

 void GameManager::draw(SDLBmp * player, SDLBmp * background, SDLBmp *apple) {
	 if (m_lastRender >= 1 / 60);
	 {
		 // Add bitmaps to renderer
		 background->draw();
		 player->draw();
		 apple->draw();

		 if (isColliding(apple, player)) {
			 int x = RandomFloat(1, 550);
			 int y= RandomFloat(1, 350);
			 x = (x * 10) / 10;
			 y = (y * 10) / 10;

			 apple->x = x;
			 apple->y = y;
			 apple->draw();
		 }

		// nextCube.draw();

		 // Render window
		 SDLManager::Instance().renderWindow(m_window);
		 m_lastRender = 0.f;

		 SDLManager::Instance().renderWindow(m_window);

	 }
 }