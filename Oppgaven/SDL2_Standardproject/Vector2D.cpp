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


bool operator==(Vector2D &a, Vector2D &b)
{
	return a.x == b.x && a.y == b.y;
}
