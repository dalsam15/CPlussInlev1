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



#define TILE_SIZE 32


/* Initializes SDL, creates the game window and fires off the timer. */

SDL_Surface*  snake_surface = NULL;
SDL_Surface*  apple_surface = NULL;

SDL_Texture*  snake_texture = NULL;
SDL_Texture*  apple_texture = NULL;

SDL_Renderer* renderer = NULL;

GameManager::GameManager()
{
	SDLManager::Instance().init();
	m_window = SDLManager::Instance().createWindow("Amazing gasashiong");
	renderer = SDLManager::Instance().getRenderer(m_window);
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
bool GameManager::isColliding(GameObject a, GameObject b) {
	return (a.position->x == b.position->x && a.position->y == b.position->y); //operation overloading?
	/*
	float xDist = abs((a.position->x - b.position->x);
	float yDist = abs(a.position->y - b.position->y);
	float totaltDist = xDist + yDist;
	if (totaltDist < 50) {
		return true;
	}
	return false;
	*/
}

void GameManager::play()
{
	currentDirection = right;
	nextDirection = right;
	SDL_Window *window = NULL;
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
	}
	atexit(SDL_Quit);
	//SDL_CreateWindowAndRenderer(800, 480, 0, SDLManager::Instance().getMainWindow, SDLManager::Instance().getRenderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	apple_surface = SDL_LoadBMP("Assets/gfx/apple.bmp");
	snake_surface = SDL_LoadBMP("Assets/gfx/sdl_bro.bmp");
	snake_texture = SDL_CreateTextureFromSurface(renderer, snake_surface);
	apple_texture = SDL_CreateTextureFromSurface(renderer, apple_surface);

	snakeObject.position = new Vector2D(5, 5);
	snakeObject.texture = snake_texture;

	appleObject.position = RandomPos();
	appleObject.texture = apple_texture;




	bool notGameOver = true;

	// Calculate render frames per second (second / frames) (60)
	float render_fps = 1.f / 60.f;
	m_lastRender = render_fps; // set it to render immediately

	while (notGameOver) {
		handleInput();
		gameLoopTimer();
		draw();

	}
}

void GameManager::gameLoopTimer() {
	Timer::Instance().update();
	if (Timer::Instance().elapsedTime() > 1) {
		Timer::Instance().resetTime();
		cout << Timer::Instance().elapsedTime();
		//gameLoopTimer(player);
		gameLoop();
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


 void GameManager::gameLoop() {
	 for (snakeIterator = snake.begin();
		 snakeIterator != snake.end();
		 snakeIterator++)
	 {
		 //(snakeIterator)->x = player->x;
		// (snakeIterator)->y = player->y;
		 // cout << &snakeIterator << " ";
		 //Should output 1 4 8
	 }

	 //newSnake->x = player->x;
	 //newSnake->y = player->y;

	 currentDirection = nextDirection;
	 switch (currentDirection) {
	 case left:
		 snakeObject.position->x -= 1;
		 break;
	 case right:
		 snakeObject.position->x += 1;
		 break;
	 case up:
		 snakeObject.position->y -= 1;
		 break;
	 case down:
		 snakeObject.position->y += 1;
		 break;
	 }

	// cout << "py " << snakeHead.y << "px " << snakeHead.x << endl;
		 // Add bitmaps to renderer
 }
 Vector2D* GameManager::RandomPos() {
	 int x = (int)RandomFloat(1, 10);
	 int y = (int)RandomFloat(1, 10);
	// x = roundToFifty(x);
	// y = roundToFifty(y);
	 cout << "appley " << y << ", applex " << x << endl;
	 return new Vector2D(x, y);

	
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

 void GameManager::draw() {
	 if (m_lastRender >= 1 / 60);
	 {
		 SDL_RenderClear(renderer);

		 drawGameObject(snakeObject);
		 drawGameObject(appleObject);

		 if (isColliding(snakeObject, appleObject)) {
			 appleObject.position = RandomPos();

			 //snake.push_back(SDLBmp("Assets/gfx/sdl_bro.bmp"));
			 //cout << "score: " << snake.size() << endl;
		 }
		 /*
		 // Add bitmaps to renderer
		 background->draw();
		 player->draw();
		 apple->draw();
		 newSnake->draw();
		 if (isColliding(apple, player)) {
			 MoveToRandomPosition(apple);

			 snake.push_back(SDLBmp("Assets/gfx/sdl_bro.bmp"));
			 cout << "score: " << snake.size() << endl;
		 }
		 
		 for (snakeIterator = snake.begin();
			 snakeIterator != snake.end();
			 snakeIterator++)
		 {
			 cout << "snake iterator: " << snakeIterator->x << endl;
			 (snakeIterator)->draw();
		 }
		 */
		// nextCube.draw();

		 // Render window
		 SDLManager::Instance().renderWindow(m_window);
		 m_lastRender = 0.f;

		 SDLManager::Instance().renderWindow(m_window);

	 }
 }

 void GameManager::drawGameObject(GameObject gameObject)
 {
	 SDL_Rect rect;
	 rect.h = TILE_SIZE;
	 rect.w = TILE_SIZE;
	 rect.x = gameObject.position->x * TILE_SIZE;
	 rect.y = gameObject.position->y * TILE_SIZE;
	 SDL_RenderCopy(renderer, gameObject.texture, NULL, &rect);
 }