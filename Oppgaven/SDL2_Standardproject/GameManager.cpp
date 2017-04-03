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
	//GameManager::snake;

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
	return (a.position.x == b.position.x && a.position.y == b.position.y); //operation overloading?

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

	
	GameObject snakeObject;
	snakeObject.position.x = 5;
	snakeObject.position.y = 5;
	snakeObject.texture = snake_texture;
	
	snake.push_back(snakeObject);
	//cout << "original snake size: " << snake.size() << endl;


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
		 Timer::Instance().elapsedTime();
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



	 for (int i = (snake.size() -1 ); i > 0; i--) {
		 
		 snake[i].position.x = snake[i - 1].position.x;
		 snake[i].position.y = snake[i - 1].position.y;
			 cout << i << " X " << snake[i - 1].position.x << endl;
			 cout << i << " Y " << snake[i - 1].position.y << endl;
	 }

	 /*
	 int index = 0;
	 for (snakeIterator = snake.begin();snakeIterator != snake.end(); snakeIterator++){

		 snakeIterator._Ptr->position->x = snakeObject.position->x;
		 snakeIterator._Ptr->position->y = snakeObject.position->y;
		


		 index++;
		 
		 
	
	 }
	 */



	 currentDirection = nextDirection;
	 switch (currentDirection) {
	 case left:
		 snake[0].position.x -= 1;
		 break;
	 case right:
		 snake[0].position.x += 1;
		 break;
	 case up:
		 snake[0].position.y -= 1;
		 break;
	 case down:
		 snake[0].position.y += 1;
		 break;
	 }

 }
 Vector2D GameManager::RandomPos() {
	 int x = (int)RandomFloat(1, 10);
	 int y = (int)RandomFloat(1, 10);

	 return Vector2D(x, y);

	
}

 void GameManager::draw() {
	
	 if (m_lastRender >= 1 / 60);
	 {
		 SDL_RenderClear(renderer);

		 //drawGameObject(snakeObject);
		 drawGameObject(appleObject);

		 if (isColliding(snake[0], appleObject)) {
			 appleObject.position = RandomPos();
			 GameObject newBody;
			 newBody.texture = snake_texture;
			 snake.push_back(newBody);
		 }

		 for (snakeIterator = snake.begin();
			 snakeIterator != snake.end();
			 snakeIterator++)
		 {
			 drawGameObject(*snakeIterator);
			 
		 }

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
	 rect.x = gameObject.position.x * TILE_SIZE;
	 rect.y = gameObject.position.y * TILE_SIZE;
	 SDL_RenderCopy(renderer, gameObject.texture, NULL, &rect);
	
 }