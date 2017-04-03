#include "Vector2D.h"

Vector2D::Vector2D()
{
	Vector2D(0, 0);
}

Vector2D::Vector2D(int newX, int newY)
{
	x = newX;
	y = newY;
}


Vector2D::~Vector2D()
{
}

bool Vector2D::operator==(Vector2D other)
{
	return (x == other.x && y == other.y);
}
