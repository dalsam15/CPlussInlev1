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

#include <SDL.h>
#include <SDL_image.h>

using namespace std;



#define TILE_SIZE 32
#define BOARD_WIDTH 15
#define BOARD_HEIGHT 15


/* Initializes SDL, creates the game window and fires off the timer. */

SDL_Surface*  snake_surface = NULL;
SDL_Surface*  apple_surface = NULL;

SDL_Texture*  snake_texture = NULL;
SDL_Texture*  apple_texture = NULL;

SDL_Renderer* renderer = NULL;





GameManager::GameManager()
{
	SDLManager::Instance().init();
	m_window = SDLManager::Instance().createWindow("Amazing gasashiong", (TILE_SIZE * BOARD_WIDTH), (TILE_SIZE * BOARD_HEIGHT));
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
	//apple_surface = SDL_LoadBMP("Assets/gfx/apple.bmp");
	snake_surface = IMG_Load("Assets/gfx/snake body.png");
	SDL_Surface * snakeHead_surface = IMG_Load("Assets/gfx/snake head.png");
	apple_surface = IMG_Load("Assets/gfx/apple.png");

	snake_texture = SDL_CreateTextureFromSurface(renderer, snake_surface);
	SDL_Texture*  snakeHead_texture = SDL_CreateTextureFromSurface(renderer, snakeHead_surface
	);
	apple_texture = SDL_CreateTextureFromSurface(renderer, apple_surface);

	for (int i = 0; i < 4; i++) {
		addSnakeBody();
		snake[i].position.x = 5 - i;
		snake[i].position.y = 5;
	}
	snake[0].texture = snakeHead_texture;
	

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
					 snake[0].transform.flip = SDL_FLIP_NONE;
					 snake[0].transform.rotation = -90;
				 }
				 break;
			 case SDLK_DOWN:
				 if (currentDirection != up) {
					 nextDirection = down;
					 snake[0].transform.flip = SDL_FLIP_NONE;
					 snake[0].transform.rotation = 90;
				 }
				 break;
			 case SDLK_RIGHT:
				 if (currentDirection != left) {
					 nextDirection = right;
					 snake[0].transform.flip = SDL_FLIP_NONE;
					 snake[0].transform.rotation = 0;
				 }
				 break;
			 case SDLK_LEFT:
				 if (currentDirection != right) {
					 nextDirection = left;
					 snake[0].transform.flip = SDL_FLIP_HORIZONTAL;
					 snake[0].transform.rotation = 0;
				 }
				 break;
			 }
		 }

	 }


 void GameManager::gameLoop() {

	 if (isColliding(snake[0], appleObject)) {
		 appleObject.position = RandomPos();
		 addSnakeBody();
	 }

	 for (int i = (snake.size() -1 ); i > 0; i--) {
		 
		 snake[i].position.x = snake[i - 1].position.x;
		 snake[i].position.y = snake[i - 1].position.y;
	 }

	 


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
	 if (IsGameOver()) {
		 cout << "oops..." << endl;
	 }

	 draw();
 }

 bool GameManager::IsGameOver(){
	 for (int i = 0; i < snake.size(); i++) {
		 for (int j = 0; j < snake.size(); j++) {
			 if (isColliding(snake[i], snake[j]) && j != i) {
				 return true;
			 }
		 }
	 }
	 cout << "x: " << snake[0].position.x << "y: " << snake[0].position.y << endl;
	 if (snake[0].position.x < 0) return true;
	 if (snake[0].position.x > BOARD_WIDTH -1 ) return true;
	 if (snake[0].position.y < 0) return true;
	 if (snake[0].position.y > BOARD_HEIGHT -1) return true;

	 return false;
}
 Vector2D GameManager::RandomPos() {
	 int x = (int)RandomFloat(1, 10);
	 int y = (int)RandomFloat(1, 10);

	 return Vector2D(x, y);

	
}

 void GameManager::draw() {
	
	
		 SDL_RenderClear(renderer);

		
		 drawGameObject(appleObject);

		 

		 for (snakeIterator = snake.begin();
			 snakeIterator != snake.end();
			 snakeIterator++)
		 {
			 drawGameObject(*snakeIterator);
			 
		 }
		  //Render window
		 SDLManager::Instance().renderWindow(m_window);
		 m_lastRender = 0.f;

		 SDLManager::Instance().renderWindow(m_window);

	 
 }

 void GameManager::drawGameObject(GameObject gameObject)
 {
	 SDL_Rect rect;
	 rect.h = TILE_SIZE;
	 rect.w = TILE_SIZE;
	 rect.x = gameObject.position.x * TILE_SIZE;
	 rect.y = gameObject.position.y * TILE_SIZE;
	// SDL_RenderCopy(renderer, gameObject.texture, NULL, &rect);
	 SDL_RenderCopyEx(renderer, gameObject.texture, NULL, &rect, gameObject.transform.rotation, NULL, gameObject.transform.flip);
	
 }

 void GameManager::addSnakeBody()
 {
	 GameObject newBody;
	 newBody.texture = snake_texture;
	 snake.push_back(newBody);
 }
