#pragma once
class Vector2D
{
public:
	Vector2D();
	Vector2D(int x, int y);
	~Vector2D();

	bool operator==(Vector2D other);

	int y;
	int x;
};

