#pragma once
#include <stdexcept>
#include <glm/vec4.hpp>

struct Coord
{
	Coord()
	{
		this->x = 0;
		this->y = 0;
	}

	Coord(const Coord& other)
	{
		x = other.x;
		y = other.y;
	}
	Coord(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	Coord(int coords[2]) : Coord(coords[0], coords[1])
	{
	}

	int x;
	int y;

	friend Coord operator+(const Coord& lhs, const Coord& rhs)
	{
		Coord coord;
		coord.x = lhs.x + rhs.x;
		coord.y = lhs.y + rhs.y;

		return coord;
	}
	friend Coord operator-(const Coord& lhs, const Coord& rhs)
	{
		Coord coord;
		coord.x = lhs.x - rhs.x;
		coord.y = lhs.y - rhs.y;

		return coord;
	}
	friend Coord operator+=(Coord& lhs, const Coord& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		return lhs;
	}
};

struct Block
{
	Block()
	{
		free = true;
		colour = { 0.f,0.f,0.f,1.f };
	}

	Block(const Block& other)
	{
		free = other.free;
		colour = other.colour;
	}
	bool free;
	glm::vec4 colour;
};

//struct TLayout
//{
//	TLayout()
//	{
//		for (int x = 0; x < EXTENTS[0]; x++)
//		{
//			for (int y = 0; y < EXTENTS[1]; y++)
//			{
//				layout[x][y] = false;
//			}
//		}
//	}
//
//	bool layout[4][4];
//
//	bool &operator()(int x, int y)
//	{
//		if ((x >= EXTENTS[0] / 2 || x < -(EXTENTS[0] / 2)) || (y >= EXTENTS[1] / 2 || y < -(EXTENTS[1] / 2)))
//			std::runtime_error("attempting to access out of bounds");
//		else
//			return layout[x + (EXTENTS[0] / 2)][y + (EXTENTS[1] / 2)];
//	}
//
//	const int EXTENTS[2] = { 4,4 };
//private:
//};

struct TLayout
{
	TLayout()
	{
		for (int x = 0; x < EXTENT; x++)
		{
			for (int y = 0; y < EXTENT; y++)
			{
				layout[x][y] = false;
			}
		}
	}

	TLayout(const TLayout& other)
	{
		for (int x = 0; x < EXTENT; x++)
		{
			for (int y = 0; y < EXTENT; y++)
			{
				layout[x][y] = other.layout[x][y];
			}
		}
	}

	bool& operator()(int x, int y)
	{
		if ((x > EXTENT / 2 || x < -(EXTENT / 2)) || (y > EXTENT / 2 || y < -(EXTENT / 2)))
			std::runtime_error("attempting to access out of bounds");
		else
			return layout[x + (EXTENT / 2)][y + (EXTENT / 2)];
	}

	const static int EXTENT = 5;
	bool layout[EXTENT][EXTENT];
private:
};

enum TetrominoType{ Long, Square, L, J, Z, S, T };

class Tetromino
{
public:
	Tetromino(TetrominoType type)
	{
		pos.x = 0;
		pos.y = 0;

		switch (type)
		{
		case TetrominoType::Long:
			LongPiece();
			break;
		case TetrominoType::Square:
			SquarePiece();
			break;
		case TetrominoType::L:
			LPiece();
			break;
		case TetrominoType::J:
			JPiece();
			break;
		case TetrominoType::Z:
			ZPiece();
			break;
		case TetrominoType::S:
			SPiece();
			break;
		case TetrominoType::T:
			TPiece();
			break;
		}

		if(type != TetrominoType::Square)
		for(int i = 0; i < NUM_OF_LAYOUTS - 1; i++)
			CalcRotation(layout[i], layout[i + 1]);

		block.free = false;

		currentLayout = 0;
	}
	~Tetromino()
	{
	
	}
	Block block;

	const static int NUM_OF_LAYOUTS = 4;
	const static int NUM_OF_BLOCKS = 4;
	TLayout layout[NUM_OF_LAYOUTS];
	int currentLayout;

	Coord pos;
	Coord blockLocations[4];

	Coord targetPos;
	Coord targetBlockLocations[4];

	void Rotate();
	void Spawn();
private:
	Coord spawnOffset;

	void LongPiece();
	void TPiece();
	void LPiece();
	void JPiece();
	void SquarePiece();
	void ZPiece();
	void SPiece();

	void CalcRotation(TLayout& origin, TLayout& dest);
};