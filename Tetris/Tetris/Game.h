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
		state = GameState::PLAYING;

		dropRate_normal		= 0.1f;
		dropRate_accel		= 0.01f;
		dropRate_current	= dropRate_normal;

		timer_dropRate	= 0.f;
		timer_dropPiece = 0.f;

		releasedSpacebar	= true;
		releasedRight		= true;
		releasedLeft		= true;
		releasedUp			= true;

		NewPiece();
	}

	~Game()
	{

	}

	enum GameState { PLAYING, PAUSED, LOST, WON };

	const static int BOARD_X = 10;
	const static int BOARD_Y = 40;

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
	const float rate_dropRate = 60.f;

	bool releasedSpacebar;
	bool releasedRight;
	bool releasedLeft;
	bool releasedUp;

	void Playing(float dt);
	void Paused();
	void Lost();
	void Won();

	void ResetGame();

	void LocatePiece();
	
	void PlayerControls();

	void CalculateDropRateNormal(float dt);
	void DropPiece(float dt);
	void PieceHitFloor();
	void NewPiece();

	bool CanPieceMoveDown();
	bool CanPieceMoveHorizontal(bool right);
	
	void LineCheck();
	void DeleteLine(int y, int offset);
	void ClearLine(int y);
};