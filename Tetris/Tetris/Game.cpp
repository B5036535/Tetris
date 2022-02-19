#include "Game.h"
#include <map>
#include <vector>
#include <set>
#include <iostream>

void Game::Update(float dt)
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		releasedSpacebar = true;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE)
		releasedRight = true;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE)
		releasedLeft = true;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE)
		releasedUp = true;

	switch (state)
	{
	case GameState::PLAYING:
		Playing(dt);
		break;
	case GameState::PAUSED:
		Paused();
		break;
	case GameState::LOST:
		Lost();
		break;
	case GameState::WON:
		Won();
		break;
	}
	renderer.DrawFrame();
}

void Game::Playing(float dt)
{
	PlayerControls();
	CalculateDropRateNormal(dt);
	DropPiece(dt);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && releasedSpacebar)
	{
		state = GameState::PAUSED;
		releasedSpacebar = false;
	}
}

void Game::Paused()
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && releasedSpacebar)
	{
		state = GameState::PLAYING;
		releasedSpacebar = false;
	}
	else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		ResetGame();
}

void Game::Lost()
{
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		ResetGame();
}

void Game::Won()
{
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		ResetGame();
}

void Game::ResetGame()
{
	Block freshBlock;
	for (int y = 0; y < BOARD_Y; y++)
	{
		for (int x = 0; x < BOARD_X; x++)
		{
			board[x][y] = freshBlock;
		}
	}
	state = GameState::PLAYING;

	dropRate_normal = 1.0f;
	dropRate_accel = 0.02f;
	dropRate_current = dropRate_normal;

	timer_dropRate = 0.f;
	timer_dropPiece = 0.f;

	releasedSpacebar = true;
	releasedRight = true;
	releasedLeft = true;
	releasedUp = true;

	NewPiece();
}

void Game::LocatePiece(int rotation)
{
	int it = 0;
	for (int x = 0; x < currentPiece->layout[0].EXTENT; x++)
	{
		for (int y = 0; y < currentPiece->layout[0].EXTENT; y++)
		{
			if (currentPiece->layout[rotation].layout[x][y])
			{
				Coord boardCoord = ConvertLocalToWorld(x, y);
				currentPiece->targetBlockLocations[it] = boardCoord;
				it++;
			}
		}
	}
}

void Game::PlayerControls()
{
	
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && releasedRight)
	{
		releasedRight = false;
		MovePiece({ 1,0 }, currentPiece->currentLayout);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && releasedLeft)
	{
		releasedLeft = false;
		MovePiece({ -1,0 }, currentPiece->currentLayout);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && releasedUp)
	{
		releasedUp = false;
		RotatePiece();
	}

	dropRate_current = glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS ? dropRate_accel : dropRate_normal;
}

void Game::CalculateDropRateNormal(float dt)
{
	timer_dropRate += dt;

	if (timer_dropRate > rate_dropRate)
	{
		timer_dropRate = 0.f;
		dropRate_normal /= 1.5f;
		dropRate_current = dropRate_normal;
	}
}


void Game::DropPiece(float dt)
{
	timer_dropPiece += dt;

	if (timer_dropPiece > dropRate_current)
	{
		timer_dropPiece = 0;

		if (!MovePiece({ 0, -1 }, currentPiece->currentLayout))
			PieceHitFloor();
	}
}

void Game::PieceHitFloor()
{
	for (Coord coord : currentPiece->blockLocations)
	{
		board[coord.x][coord.y] = currentPiece->block;
	}

	LineCheck();
	NewPiece();
}

void Game::NewPiece()
{
	int rnd = rand() % 7;

	currentPiece = &tetrominos[rnd];
	currentPiece->Spawn();

	if (!MovePiece({ BOARD_X / 2, BOARD_Y - 1 }, currentPiece->currentLayout))
		state = GameState::LOST;
}

bool Game::MovePiece(Coord offset, int rotation)
{
	currentPiece->targetPos = currentPiece->pos + offset;
	LocatePiece(rotation);

	bool canMove = true;
	for (Coord coord : currentPiece->targetBlockLocations)
	{
		if (!board[coord.x][coord.y].free || coord.x < 0 || coord.x >= BOARD_X ||  coord.y < 0 || coord.y >= BOARD_Y)
		{
			canMove = false;
			return canMove;
		}
	}

	if (canMove)
	{
		currentPiece->pos = currentPiece->targetPos;
		currentPiece->currentLayout = rotation;
		for (int i = 0; i < currentPiece->NUM_OF_BLOCKS; i++)
		{
			currentPiece->blockLocations[i] = currentPiece->targetBlockLocations[i];
		}
	}

	return canMove;
}

void Game::RotatePiece()
{
	Coord offset = { 0,0 };

	const int MAX_NUM_OF_ROTATION_ATTEMPTS = 4;
	int attempts = 0;
	while (!MovePiece(offset, (currentPiece->currentLayout + 1) % currentPiece->NUM_OF_LAYOUTS) && attempts < MAX_NUM_OF_ROTATION_ATTEMPTS)
	{
		for (Coord coord : currentPiece->targetBlockLocations)
		{
			if (!board[coord.x][coord.y].free || coord.x < 0 || coord.x >= BOARD_X || coord.y < 0 || coord.y >= BOARD_Y)
			{
				offset += (currentPiece->targetPos - coord);
				break;
			}
		}

		attempts++;
	}
}

Coord Game::ConvertLocalToWorld(int localX, int localY)
{
	Coord localCoord;

	localCoord.x = localX - 2;
	localCoord.y = localY - 2;

	return localCoord + currentPiece->targetPos;
}

void Game::LineCheck()
{
	std::set<int> activeYs;
	for (Coord coord : currentPiece->blockLocations)
	{
		activeYs.insert(coord.y);
	}

	std::vector<int> deletedYs;
	for (int activeY : activeYs)
	{
		bool fullLine = true;
		for (int x = 0; x < BOARD_X; x++)
		{
			if (board[x][activeY].free)
			{
				fullLine = false;
				break;
			}
		}

		if (fullLine)
			deletedYs.push_back(activeY);
	}

	if (deletedYs.size() > 0)
	{
		int startY = deletedYs[0];
		int offset = 1;
		int nextDeletedY = 1 % deletedYs.size();
		int y; 
		for (y = startY; y + offset < BOARD_Y; y++)
		{
			while(y + offset == deletedYs[nextDeletedY])
			{
				offset++;
				nextDeletedY = (nextDeletedY + 1) % deletedYs.size();
			}

			DeleteLine(y, offset);
		}
		for (;y < BOARD_Y; y++)
		{
			ClearLine(y);
		}
	}
}

void Game::DeleteLine(int y, int offset)
{
	for (int x = 0; x < BOARD_X; x++)
		board[x][y] = board[x][y + offset];
}

void Game::ClearLine(int y)
{
	for (int x = 0; x < BOARD_X; x++)
	{
		board[x][y].colour = { 0,0,0,1.f };
		board[x][y].free = true;
	}
}
