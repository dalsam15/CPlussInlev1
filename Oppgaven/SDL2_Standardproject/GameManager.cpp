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
#include <string>

using namespace std;

#define TILE_SIZE 32
#define BOARD_WIDTH 15
#define BOARD_HEIGHT 15
#define HEADER_HEIGHT 64
#define SNAKE_START_LENGTH 4

//global variables
#pragma region sdl
	SDL_Texture* text = NULL;
	SDL_Texture*  snake_texture = NULL;
	SDL_Texture*  apple_texture = NULL;
	SDL_Texture* snakeHead_texture = NULL;
	SDL_Texture* header_texture = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Rect rectPlayAgain;
	SDL_Rect rectClose;
	SDL_Rect gameOverRect;
	SDL_Rect headerRect;
	SDL_Texture * gameOver_texture;
	SDL_Texture * gameOver_PlayAgainTX;
	SDL_Texture * gameOver_CloseTX;
	SDL_Rect headerTextRect;
	SDL_Rect scoreTextBox;
	TTF_Font* font;
#pragma endregion

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
	return a.position == b.position;
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
	moveAppleToRandomPos();
	snake[0].texture = snakeHead_texture;
	appleObject.texture = apple_texture;
}

void GameManager::modifyRect(SDL_Rect * rect,int x, int y, int width, int height) {
	rect->x = x;
	rect->y = y;
	rect->w = width;
	rect->h = height;
}
SDL_Texture* GameManager::createTextureFromPath(string path) {
	SDL_Surface * surface = IMG_Load(path.c_str());
	return SDL_CreateTextureFromSurface(renderer, surface);
}
void GameManager::initalizeGameSDL() {
	TTF_Init();
	font = TTF_OpenFont("Assets/fonts/junegull.ttf", 28);

	modifyRect(&headerTextRect, 10, HEADER_HEIGHT /2, 200, 30);
	modifyRect(&gameOverRect, 25, 50, 400, 200);
	modifyRect(&rectPlayAgain, 25, 450, 173, 50);
	modifyRect(&rectClose, 265, 450, 173, 50);
	modifyRect(&headerRect, 0, 0, 500, HEADER_HEIGHT);
	modifyRect(&scoreTextBox, 75, 250, 300, 75);

	atexit(SDL_Quit); // ?? 
	SDL_SetRenderDrawColor(renderer, 65, 65, 65, 65); //background color of game

	snake_texture = createTextureFromPath("Assets/gfx/snake body.png");
	snakeHead_texture = createTextureFromPath("Assets/gfx/snake head.png");
	apple_texture = createTextureFromPath("Assets/gfx/apple.png");
	header_texture = createTextureFromPath("Assets/gfx/header.png");
	gameOver_CloseTX = createTextureFromPath("Assets/gfx/giveup.png");
	gameOver_PlayAgainTX = createTextureFromPath("Assets/gfx/playagain.png");
	gameOver_texture = createTextureFromPath("Assets/gfx/itsgameover.png");
}

void GameManager::play()
{
	//important
	initalizeGameSDL();
	initalizeNewGame();

	while (running) 
	{
		handleInput();
		gameLoopTimer();
	}
	TTF_Quit();
	//This is where you free shit!
}

void GameManager::gameLoopTimer() {
	Timer::Instance().update();
	if (Timer::Instance().elapsedTime() > frameRateDelay) {
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

void GameManager::moveAppleToRandomPos() {
	bool foundValidApplePos = false;
	while (!foundValidApplePos) {
		appleObject.position = RandomPos();
		bool hasColliderWithAny = false;
		for (int i = 0; i < (int)snake.size(); i++) {
			if (isColliding(snake[i], appleObject)) hasColliderWithAny = true;
		}
		foundValidApplePos = !hasColliderWithAny;
	}
}

 void GameManager::gameLoop() {

	 //if snake head is colliding with apple
	 if (isColliding(snake[0], appleObject)) {
		 moveAppleToRandomPos(); 
		 addSnakeBody(); //increases length of snake
		 score++;
	 }

	 //moves each body part forward
	 for (int i = (snake.size() -1 ); i > 0; i--) {
		 snake[i].position.x = snake[i - 1].position.x;
		 snake[i].position.y = snake[i - 1].position.y;
	 }

	 //moves snake head in the current direction
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

	 //important
	 drawGame();
 }

 bool GameManager::IsGameOver(){
	 //checks if snake collides with himself
	 for (int i = 0; i < (int)snake.size(); i++) {
		 for (int j = 0; j < (int)snake.size(); j++) {
			 if (isColliding(snake[i], snake[j]) && j != i) {
				 return true;
			 }
		 }
	 }

	 //checks if snake is out of bounds
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

 void GameManager::drawGame() {
	 SDL_RenderClear(renderer);

		//draws apple
		 drawGameObject(appleObject);

		 //draws snake
		 for (snakeIterator = snake.begin();
			 snakeIterator != snake.end();
			 snakeIterator++) { drawGameObject(*snakeIterator); }
		 
		 //draws header
		 drawGameHeader();
		 
		 SDLManager::Instance().renderWindow(m_window);
 }

 void GameManager::drawGameObject(GameObject gameObject)
 {
	 SDL_Rect rect;
	 rect.h = TILE_SIZE;
	 rect.w = TILE_SIZE;
	 rect.x = gameObject.position.x * TILE_SIZE;
	 rect.y = gameObject.position.y * TILE_SIZE;
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
	 SDL_RenderClear(renderer);
	 SDL_RenderCopyEx(renderer, gameOver_texture, NULL, &gameOverRect, 0, NULL, SDL_FLIP_NONE);
	 SDL_RenderCopyEx(renderer, gameOver_CloseTX, NULL, &rectClose, 0, NULL, SDL_FLIP_NONE);
	 SDL_RenderCopyEx(renderer, gameOver_PlayAgainTX, NULL, &rectPlayAgain, 0, NULL, SDL_FLIP_NONE);
	 drawGameOverScore();
	 SDLManager::Instance().renderWindow(m_window);
 }

 bool GameManager::withinBounds(const SDL_Rect& rect, int x, int y) {
	 return(x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h);
 }

 void GameManager::drawGameOverScore(){
	 SDL_Color textColor = {10,10,10};
	 string score_text = "Final score: " + to_string(score);
	 SDL_Surface* textSurface = TTF_RenderText_Blended(font, score_text.c_str(), textColor);
	 text = SDL_CreateTextureFromSurface(renderer, textSurface);
	 SDL_RenderCopy(renderer, text, NULL, &scoreTextBox);
 }

 void GameManager::drawGameHeader() {
	SDL_Color textColor = { 230,230,230};
	string score_text = "score: " + to_string(score);
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, score_text.c_str(), textColor);
	text = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_RenderCopy(renderer, header_texture, NULL, &headerRect);
	SDL_RenderCopy(renderer, text, NULL, &headerTextRect);
 }
