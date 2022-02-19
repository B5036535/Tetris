#include "Tetromino.h"

void Tetromino::Rotate()
{
	currentLayout = (currentLayout + 1) % NUM_OF_LAYOUTS;
}

void Tetromino::Spawn()
{
	currentLayout = 0;
	pos = spawnOffset;
	targetPos = spawnOffset;
}

void Tetromino::LongPiece()
{
	block.colour = { 0.2f, 0.8f, 1.0f ,1.0f };
	//layout[0](-2, 0) = true;
	//layout[0](-1, 0) = true;
	//layout[0](0, 0) = true;
	//layout[0](1, 0) = true;

	layout[0](-2, 0) = true;
	layout[0](-1, 0) = true;
	layout[0](0, 0) = true;
	layout[0](1, 0) = true;

	spawnOffset = { 0, -2 };
}

void Tetromino::TPiece()
{
	block.colour = { 1.f, 0.f, 1.0f ,1.0f };
	//layout[0](-2, 0) = true;
	//layout[0](-1, 0) = true;
	//layout[0](0, 0) = true;
	//layout[0](-1, -1) = true;

	layout[0](1, 0) = true;
	layout[0](-1, 0) = true;
	layout[0](0, 0) = true;
	layout[0](0, -1) = true;
	spawnOffset = { 0, -1 };

}

void Tetromino::LPiece()
{
	block.colour = { 1.f, 1.f, 0.8f ,1.0f };
	//layout[0](-1, 1) = true;
	//layout[0](-1, 0) = true;
	//layout[0](-1, -1) = true;
	//layout[0](0, -1) = true;
	
	layout[0](0, 2) = true;
	layout[0](0, 1) = true;
	layout[0](0, 0) = true;
	layout[0](1, 0) = true;

	spawnOffset = { 0, -2 };
}

void Tetromino::JPiece()
{
	block.colour = { 0.f, 0.f, 1.f ,1.0f };
	//layout[0](0, 1) = true;
	//layout[0](0, 0) = true;
	//layout[0](0, -1) = true;
	//layout[0](-1, -1) = true;

	layout[0](0, 2) = true;
	layout[0](0, 1) = true;
	layout[0](0, 0) = true;
	layout[0](-1, 0) = true;

	spawnOffset = { 0, -2 };
}

void Tetromino::SquarePiece()
{
	block.colour = {1.f, 0.3f, 0.f ,1.0f };
	//layout[0](-1, 0) = true;
	//layout[0](0, 0) = true;
	//layout[0](0, -1) = true;
	//layout[0](-1, -1) = true;

	layout[0](0, 1) = true;
	layout[0](0, 0) = true;
	layout[0](1, 1) = true;
	layout[0](1, 0) = true;

	for (int i = 1; i < NUM_OF_LAYOUTS; i++)
		layout[i] = layout[0];

	spawnOffset = { 0, -1 };
}

void Tetromino::ZPiece()
{
	block.colour = { 1.f, 0.f, 0.f ,1.0f };
	//layout[0](-2, 0) = true;
	//layout[0](-1, 0) = true;
	//layout[0](-1, -1) = true;
	//layout[0](0, -1) = true;

	layout[0](-1, 1) = true;
	layout[0](0, 1) = true;
	layout[0](0, 0) = true;
	layout[0](1, 0) = true;
	spawnOffset = { 0, -1 };
}

void Tetromino::SPiece()
{
	block.colour = { 0.f, 1.f, 0.f ,1.0f };
	//layout[0](-2, -1) = true;
	//layout[0](-1, -1) = true;
	//layout[0](-1, 0) = true;
	//layout[0](0, 0) = true;

	layout[0](1, 1) = true;
	layout[0](0, 1) = true;
	layout[0](0, 0) = true;
	layout[0](-1, 0) = true;
	spawnOffset = { 0, -1 };
}

void Tetromino::CalcRotation(TLayout& origin, TLayout& dest)
{
	//dest(0, 0) = origin(-1, 0);
	//dest(0, -1) = origin(0, 0);
	//dest(-1, -1) = origin(0, -1);
	//dest(-1, 0) = origin(-1, -1);
	//
	//dest(1, 0) = origin(-1, 1);
	//dest(1, -1) = origin(0, 1);
	//dest(0, -2) = origin(1, 0);
	//dest(-1, -2) = origin(1, -1);
	//dest(-2, -1) = origin(0, -2);
	//dest(-2, 0) = origin(-1, -2);
	//dest(-1, 1) = origin(-2, -1);
	//dest(0, 1) = origin(-2, 0);

	dest(0, 0) = origin(0, 0);
	dest(-2, 0) = origin(0, -2);
	dest(-1, 0) = origin(0, -1);
	dest(0, 2) = origin(-2, 0);
	dest(0, 1) = origin(-1, 0);
	dest(2, 0) = origin(0, 2);
	dest(1, 0) = origin(0, 1);
	dest(0, -2) = origin(2, 0);
	dest(0, -1) = origin(1, 0);

	dest(1, 1) = origin(-1, 1);
	dest(1, -1) = origin(1, 1);
	dest(-1, -1) = origin(1, -1);
	dest(-1, 1) = origin(-1, -1);
}
