#pragma once
#include "Tetromino.h"
#include "Renderer.h"

class GLFWwindow;

class Game
{
public:
	Game(GLFWwindow* w, const uint32_t width, const uint32_t height) : renderer(w, this, width, height)
	{
		window = w;
		ResetGame();
	}

	~Game()
	{

	}

	enum GameState { PLAYING, PAUSED, LOST, WON };

	const static int BOARD_X = 10;
	const static int BOARD_Y = 51;

	GameState GetState() { return state; }

	void Update(float dt);
	Coord ConvertLocalToWorld(int localX, int localY);

	Tetromino* currentPiece;
	Block board[BOARD_X][BOARD_Y];
private:
	GLFWwindow* window;
	Renderer renderer;

	GameState state;


	Tetromino Long = Tetromino(TetrominoType::Long);
	Tetromino Square = Tetromino(TetrominoType::Square);
	Tetromino L = Tetromino(TetrominoType::L);
	Tetromino J = Tetromino(TetrominoType::J);
	Tetromino Z = Tetromino(TetrominoType::Z);
	Tetromino S = Tetromino(TetrominoType::S);
	Tetromino T = Tetromino(TetrominoType::T);

	Tetromino tetrominos[7] = {Long, Square, L, J, Z, S, T};

	float dropRate_current;
	float dropRate_normal;
	float dropRate_accel;

	float timer_dropPiece;

	float timer_dropRate;
	const float rate_dropRate = 30.f;

	bool releasedSpacebar;
	bool releasedRight;
	bool releasedLeft;
	bool releasedUp;

	void Playing(float dt);
	void Paused();
	void Lost();
	void Won();

	void ResetGame();

	void LocatePiece(int rotation);
	
	void PlayerControls();

	void CalculateDropRateNormal(float dt);
	void DropPiece(float dt);
	void PieceHitFloor();
	void NewPiece();

	bool MovePiece(Coord offset, int layout);
	void RotatePiece();

	void LineCheck();
	void DeleteLine(int y, int offset);
	void ClearLine(int y);
};