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
#include <chrono>

#include <SDL_ttf.h>


#include <SDL.h>
#include <SDL_image.h>

using namespace std;



#define TILE_SIZE 32
#define BOARD_WIDTH 15
#define BOARD_HEIGHT 15
#define HEADER_HEIGHT 64
#define SNAKE_START_LENGTH 4


/* Initializes SDL, creates the game window and fires off the timer. */

SDL_Texture* text = NULL;

SDL_Surface*  snake_surface = NULL;
SDL_Surface*  apple_surface = NULL;
SDL_Surface * snakeHead_surface = NULL;

SDL_Texture*  snake_texture = NULL;
SDL_Texture*  apple_texture = NULL;
SDL_Texture* snakeHead_texture = NULL;

SDL_Texture* header_texture = NULL;

SDL_Renderer* renderer = NULL;

bool running = true;
bool notGameOver = true;
bool recivedUserInfo = false;

SDL_Rect rectPlayAgain;
SDL_Rect rectClose;
SDL_Rect rect;

SDL_Rect headerRect;

SDL_Surface * header_surface;

SDL_Surface * gameOver_surface;
SDL_Texture * gameOver_texture;
SDL_Surface * gameOver_PlayAgain;
SDL_Texture * gameOver_PlayAgainTX;

SDL_Surface * gameOver_Close;
SDL_Texture * gameOver_CloseTX;

TTF_Font* font;

GameManager::GameManager()
{
	SDLManager::Instance().init();
	m_window = SDLManager::Instance().createWindow("Snake", (TILE_SIZE * BOARD_WIDTH), (TILE_SIZE * BOARD_HEIGHT) + HEADER_HEIGHT);
	renderer = SDLManager::Instance().getRenderer(m_window);
	Timer::Instance().init();
}

float GameManager:: RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}
bool GameManager::isColliding(GameObject a, GameObject b) {
	return (a.position.x == b.position.x && a.position.y == b.position.y); //operation overloading?
}


void GameManager::initalizeNewGame() {
	snake.clear();
	notGameOver = true;
	score = 0;
	currentDirection = right;
	nextDirection = right;

	//create snake body
	for (int i = 0; i < SNAKE_START_LENGTH; i++) {
		addSnakeBody();
		snake[i].position.x = 5 - i;
		snake[i].position.y = 5;
	}
	appleObject.position = RandomPos();
	drawHeader();

	snake[0].texture = snakeHead_texture;
	appleObject.texture = apple_texture;
}

void GameManager::initalizeGameSDL() {
	TTF_Init();
	font = TTF_OpenFont("Assets/fonts/junegull.ttf", 28);
	if (font == NULL) {
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		return;
	}

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
	snakeHead_surface = IMG_Load("Assets/gfx/snake head.png");
	apple_surface = IMG_Load("Assets/gfx/apple.png");

	snake_texture = SDL_CreateTextureFromSurface(renderer, snake_surface);
	snakeHead_texture = SDL_CreateTextureFromSurface(renderer, snakeHead_surface
	);
	apple_texture = SDL_CreateTextureFromSurface(renderer, apple_surface);

	gameOver_surface = IMG_Load("Assets/gfx/itsgameover.png");
	gameOver_texture = SDL_CreateTextureFromSurface(renderer, gameOver_surface);

	gameOver_PlayAgain = IMG_Load("Assets/gfx/playagain.png");
	gameOver_PlayAgainTX = SDL_CreateTextureFromSurface(renderer, gameOver_PlayAgain);

	gameOver_Close = IMG_Load("Assets/gfx/giveup.png");
	gameOver_CloseTX = SDL_CreateTextureFromSurface(renderer, gameOver_Close);

	header_surface = IMG_Load("Assets/gfx/header.png");
	header_texture = SDL_CreateTextureFromSurface(renderer, header_surface);

}

void GameManager::play()
{
	//important
	initalizeGameSDL();
	initalizeNewGame();

	// Calculate render frames per second (second / frames) (60)
	float render_fps = 1.f / 60.f;
	m_lastRender = render_fps; // set it to render immediately

	while (running) {
		handleInput();
		gameLoopTimer();
	}

	TTF_Quit();
	//TTF free?
}

void GameManager::gameLoopTimer() {
	Timer::Instance().update();
	if (Timer::Instance().elapsedTime() > 0.25f) {
		Timer::Instance().resetTime();
		 Timer::Instance().elapsedTime();

		 if (notGameOver) {
			 gameLoop();
		 }
		 else {
			 showGameOver();
		 }
	}
}

void GameManager::handleInput() {
		 SDL_Event e;
		 SDL_PollEvent(&e);

		 if (e.type == SDL_QUIT) {
			 running = false;
			 notGameOver = false;
			 return;
		 }
		 if (e.type == SDL_MOUSEBUTTONDOWN) {
			 if (withinBounds(rectClose, e.motion.x, e.motion.y)) {
				 running = false;
			 }
			 if (withinBounds(rectPlayAgain, e.motion.x, e.motion.y)) {
				 initalizeNewGame();
			 }
		 }
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
		 bool foundValidApplePos = false;
		 while (!foundValidApplePos) {
			 appleObject.position = RandomPos();
			 bool hasColliderWithAny = false;
			 for (int i = 0; i < snake.size(); i++) {
				 if (isColliding(snake[i], appleObject)) hasColliderWithAny = true;
			 }
			foundValidApplePos = !hasColliderWithAny;
		 }
		 addSnakeBody();
		 score++;
		 drawHeader();
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
		 notGameOver = false;
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
	 //cout << "x: " << snake[0].position.x << "y: " << snake[0].position.y << endl;
	 if (snake[0].position.x < 0) return true;
	 if (snake[0].position.x > BOARD_WIDTH -1 ) return true;
	 int headerPosInGrid = HEADER_HEIGHT / TILE_SIZE;
	 if (snake[0].position.y < headerPosInGrid) return true;
	 if (snake[0].position.y > BOARD_HEIGHT + 1) return true;

	 return false;
}
 Vector2D GameManager::RandomPos() {
	 int x = (int)RandomFloat(1, BOARD_WIDTH);
	 int y = (int)RandomFloat(1 + (HEADER_HEIGHT / TILE_SIZE), BOARD_HEIGHT);
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

		 // SDL_FreeSurface(textSurface);
		 //TODO: this is shit
		 SDL_Rect box;
		 box.w = 200;
		 box.h = 30;
		 box.x = 10;
		 box.y = HEADER_HEIGHT / 2;
		 SDL_RenderCopy(renderer, header_texture, NULL, &headerRect);
		 SDL_RenderCopy(renderer, text, NULL, &box);
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

 void GameManager::showGameOver()
 { 
	 printf("kjører denne i det hele tatt?????");

	 //TODO: these values are not nessecary to run all theF FUCKINGSS time

	 rect.h = 200;
	 rect.w = 400;
	 rect.x = 25;
	 rect.y = 50;

	 rectPlayAgain.h = 50;
	 rectPlayAgain.w = 173;
	 rectPlayAgain.x = 25;
	 rectPlayAgain.y = 450;

	 rectClose.h = 50;
	 rectClose.w = 173;
	 rectClose.x = 265;
	 rectClose.y = 450;

	 SDL_RenderClear(renderer);
	 SDL_RenderCopyEx(renderer, gameOver_texture, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
	 SDL_RenderCopyEx(renderer, gameOver_CloseTX, NULL, &rectClose, 0, NULL, SDL_FLIP_NONE);
	 SDL_RenderCopyEx(renderer, gameOver_PlayAgainTX, NULL, &rectPlayAgain, 0, NULL, SDL_FLIP_NONE);

	 drawGameOverScore();

	 SDLManager::Instance().renderWindow(m_window);
 }

 bool GameManager::withinBounds(const SDL_Rect& rect, int x, int y) {
	 return(x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h);
 }

 void GameManager::drawGameOverScore(){
	 SDL_Color textColor = { 10,10,10};
	 std::string score_text = "Final score: " + std::to_string(score);
	 SDL_Surface* textSurface = TTF_RenderText_Blended(font, score_text.c_str(), textColor);
	 text = SDL_CreateTextureFromSurface(renderer, textSurface);

	 SDL_Rect scoreTextBox;
	 scoreTextBox.w = 300;
	 scoreTextBox.h = 75;
	 scoreTextBox.x = 75;
	 scoreTextBox.y = 250;
	 SDL_RenderCopy(renderer, text, NULL, &scoreTextBox);
 }


 void GameManager::drawHeader() {
	headerRect.h = HEADER_HEIGHT;
	headerRect.w = 500;
	headerRect.x = 0;
	headerRect.y = 0;

	 SDL_Color textColor = { 230,230,230};
	 std::string score_text = "score: " + std::to_string(score);

	 SDL_Surface* textSurface = TTF_RenderText_Blended(font, score_text.c_str(), textColor);
	 text = SDL_CreateTextureFromSurface(renderer, textSurface);
 }
