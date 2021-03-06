#pragma once
class Vector2D
{
public:
	Vector2D();
	Vector2D(int x, int y);
	~Vector2D();

	friend bool operator==(const Vector2D &, const Vector2D &);
	Vector2D& operator=(const Vector2D &);

	int y;
	int x;
};

