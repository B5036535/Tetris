#include "Game.h"
#include <map>
#include <vector>
#include <set>

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
	LocatePiece();
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
}

void Game::LocatePiece()
{
	int it = 0;
	for (int x = 0; x < currentPiece->layout[0].EXTENTS[0]; x++)
	{
		for (int y = currentPiece->layout[0].EXTENTS[1] - 1; y >= 0; y--)
		{
			if (currentPiece->layout[currentPiece->currentLayout].layout[x][y])
			{
				Coord boardCoord = ConvertLocalToWorld(x, y);
				currentPiece->blockLocations[it] = boardCoord;
				it++;
			}
		}
	}
}

void Game::PlayerControls()
{
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && CanPieceMoveHorizontal(true) && releasedRight)
	{
		releasedRight = false;
		currentPiece->pos += {1, 0};
	}
	
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && CanPieceMoveHorizontal(false) && releasedLeft)
	{
		releasedLeft = false;
		currentPiece->pos += {-1, 0};
	}
	
	// NEED TO CHECK IF ROTATION IS VALID
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && releasedUp)
	{
		releasedUp = false;
		currentPiece->Rotate();
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
		if (CanPieceMoveDown())
		{
			currentPiece->pos += {0, -1};
		}
		else
			PieceHitFloor();
	}
}

void Game::PieceHitFloor()
{
	for (Coord coord : currentPiece->blockLocations)
	{
		board[coord.x][coord.y].free = false;
		board[coord.x][coord.y].colour = currentPiece->block.colour;
	}

	LineCheck();
	NewPiece();
}

void Game::NewPiece()
{
	int rnd = rand() % 7;

	currentPiece = &tetrominos[rnd];
	currentPiece->Spawn();

	currentPiece->pos += {BOARD_X / 2, BOARD_Y};
	LocatePiece();
}

bool Game::CanPieceMoveDown()
{
	std::map<int, int> m;
	for (Coord coord : currentPiece->blockLocations)
	{
		if (m.contains(coord.x))
		{
			if (m[coord.x] > coord.y)
				m[coord.x] = coord.y;
		}
		else
			m[coord.x] = coord.y;
	}

	for (const auto& [key, value] : m)
	{
		if (!board[key][value - 1].free || value - 1 < 0)
			return false;
	}

	return true;
}

bool Game::CanPieceMoveHorizontal(bool right)
{
	std::map<int, int> m;
	for (Coord coord : currentPiece->blockLocations)
	{
		if (right)
		{
			if (m.contains(coord.y))
			{
				if (m[coord.y] < coord.x)
					m[coord.y] = coord.x;
			}
			else
				m[coord.y] = coord.x;
		}
		else
		{
			if (m.contains(coord.y))
			{
				if (m[coord.y] > coord.x)
					m[coord.y] = coord.x;
			}
			else
				m[coord.y] = coord.x;
		}
	}

	for (const auto& [key, value] : m)
	{
		if (!board[value + right ? 1 : -1][key].free)
			return false;

		if (right && value + 1 >= BOARD_X)
			return false;
		else if (!right && value - 1 < 0)
			return false;
	}

	return true;
}

Coord Game::ConvertLocalToWorld(int localX, int localY)
{
	Coord localCoord;

	localCoord.x = localX - 2;
	localCoord.y = localY - 2;

	return localCoord + currentPiece->pos;
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
		board[x][y].colour = { 0,0,0,0 };
		board[x][y].free = true;
	}
}
