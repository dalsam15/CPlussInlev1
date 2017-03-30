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
#include <vector>

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
int GameManager::RandomInt(int a, int b) {
	int random = ((int)rand()) / (int)RAND_MAX;
	int diff = b - a;
	int r = random * diff;
	return a + r;
}
bool GameManager::isColliding(SDLBmp *a, SDLBmp *b) {
	float xDist = abs(a->x - b->x);
	float yDist = abs(a->y - b->y);
	float totaltDist = xDist + yDist;
	if (totaltDist < 50) {
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

	MoveToRandomPosition(&apple);
	
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

	 for (snakeIterator = snake.begin();
		 snakeIterator != snake.end();
		 snakeIterator++)
	 {
		 snakeIterator->x = player->x + 32;
		 snakeIterator->y = player->y - 12;
		 cout << "drawing snake body at x: " << snakeIterator->x << ", y: " << snakeIterator->y << endl;
		 // cout << &snakeIterator << " ";
		 //Should output 1 4 8
	 }

	 currentDirection = nextDirection;
	 switch (currentDirection) {
	 case left:
		 player->x -= 50;
		 break;
	 case right:
		 player->x += 50;
		 break;
	 case up:
		 player->y -= 50;
		 break;
	 case down:
		 player->y += 50;
		 break;
	 }

	 cout << "py " << player->y << "px " << player->x << endl;
		 // Add bitmaps to renderer
 }
 void GameManager::MoveToRandomPosition(SDLBmp * object) {
	 int x = (int)RandomFloat(1, 550);
	 int y = (int)RandomFloat(1, 350);
	 x = roundToFifty(x);
	 y = roundToFifty(y);
	 cout << "appley " << y << ", applex " << x << endl;
	 object->x = x;
	 object->y = y;

	
}

 int GameManager::roundToTenths(int x)
 {
	 x /= 10;
	 return floor(x + 0.5) * 10;
 }
 int GameManager::roundToFifty(int x)
 {
	 x /= 50;
	 return floor(x + 0.5) * 50;
 }

 void GameManager::draw(SDLBmp * player, SDLBmp * background, SDLBmp *apple) {
	 if (m_lastRender >= 1 / 60);
	 {
		 // Add bitmaps to renderer
		 background->draw();
		 player->draw();
		 apple->draw();

		 if (isColliding(apple, player)) {
			 MoveToRandomPosition(apple);

			 SDLBmp newSnake("Assets/gfx/sdl_bro.bmp");
			 snake.push_back(newSnake);
			 cout << "score: " << snake.size() << endl;
		 }

		 for (snakeIterator = snake.begin();
			 snakeIterator != snake.end();
			 snakeIterator++)
		 {
			 snakeIterator->draw();
		 }

		// nextCube.draw();

		 // Render window
		 SDLManager::Instance().renderWindow(m_window);
		 m_lastRender = 0.f;

		 SDLManager::Instance().renderWindow(m_window);

	 }
 }