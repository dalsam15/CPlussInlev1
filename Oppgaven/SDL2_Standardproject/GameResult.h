#include <iostream>
#include <string>
#pragma once

#ifndef GAMERESULT_H
#define GAMERESULT_H

using namespace std;

class GameResult
{
public:
	GameResult(string , string );
	~GameResult();
	string name;
	string points;

	
};
#endif