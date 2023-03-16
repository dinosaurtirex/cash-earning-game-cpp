#include <Windows.h>
#include <iostream>
#include <string>
#include <format>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <vector>
#include <random>
#include <algorithm>
#include <ctime>
#include <random>


const bool doDebug = true;

int_fast32_t playerCoordinateX = 5;
int_fast32_t playerCoordinateY = 5;

int_fast32_t playerScore = 0;

const int_fast32_t consoleSizeX = 64;
const int_fast32_t consoleSizeY = 32;

const int_fast32_t chronoSleep = 100;

const int_fast32_t entities_on_map = 5;


// * Hard level
// 1) Add multiplayer


int randint(int_fast32_t min, int_fast32_t max) {
	return std::rand() % (max + 1 - min) + min;
}


void spawn_random_entity(
	std::vector<int_fast32_t>& entityX,
	std::vector<int_fast32_t>& entityY
) 
{
	if (entityX.size() == 0 && entityY.size() == 0)
	{
		for (int i = 0; i < entities_on_map; i++)
		{
			int randXCoord = randint(1, consoleSizeX - 1);
			int randYCoord = randint(1, consoleSizeY - 1);
			entityX.push_back(randXCoord);
			entityY.push_back(randYCoord);
		}
	}
}


void spawn_ai(
	std::vector<int_fast32_t>& aiX,
	std::vector<int_fast32_t>& aiY
)
{
	if (aiX.size() == 0 && aiY.size() == 0)
	{
		for (int_fast32_t i = 0; i < 1; i++)
		{
			int randXCoord = randint(1, consoleSizeX - 1);
			int randYCoord = randint(1, consoleSizeY - 1);
			aiX.push_back(randXCoord);
			aiY.push_back(randYCoord);
		}
	}
}




bool is_entity(
	int_fast32_t x,
	int_fast32_t y,
	std::vector<int_fast32_t>& entityX,
	std::vector<int_fast32_t>& entityY
)
{
	for (int_fast32_t i = 0; i < entityX.size(); i++) {
		if (entityY[i] == y) {
			if (entityX[i] == x) {
				return true;
			}
		}
	}
	return false;
}


int getIndexOfVectorValue(
	std::vector<int_fast32_t>& vector,
	int_fast32_t value
) 
{
	auto it = std::find(vector.begin(), vector.end(), value);

	// If element was found
	if (it != vector.end())
	{
		// calculating the index
		// of K
		int index = it - vector.begin();
		return index;
	}

	return -1;
}


void intersectionWithEntity(
	std::vector<int_fast32_t>& entityY,
	std::vector<int_fast32_t>& entityX,
	int_fast32_t x,
	int_fast32_t y
)
{
	if (is_entity(x, y, entityX, entityY)) {
		int indexY = getIndexOfVectorValue(
			entityY,
			y
		);
		int indexX = getIndexOfVectorValue(
			entityX,
			x
		);
		if (indexY >= 0 && indexX >= 0)
		{
			entityY.erase(entityY.begin() + indexY);
			entityX.erase(entityX.begin() + indexX);
			playerScore++;
		}
	}
}


void update_game_screen(
	std::vector<int_fast32_t>& entityY,
	std::vector<int_fast32_t>& entityX,
	std::vector<int_fast32_t>& aiX,
	std::vector<int_fast32_t>& aiY
)
{
	system("cls");
	for (int y = 0; y < consoleSizeY; y++) {
		for (int x = 0; x < consoleSizeX; x++) {
			if (x == playerCoordinateX && y == playerCoordinateY) 
			{
				intersectionWithEntity(
					entityY,
					entityX,
					x,
					y
				);
				std::cout << "#";
			}
			else if (is_entity(x, y, aiX, aiY))
			{
				intersectionWithEntity(
					entityY,
					entityX,
					aiX[0],
					aiY[0]
				);
				std::cout << ":)";
				x++;
			}
			else if (is_entity(x, y, entityX, entityY)) {
				std::cout << "$";
			}
			else {
				std::cout << ".";
			}
		}
		std::cout << std::endl;
	}
	std::cout << "PLAYER SCORE: " << playerScore << std::endl;
	if (doDebug) {
		std::cout << "PLAYER X: " << playerCoordinateX << std::endl;
		std::cout << "PLAYER Y: " << playerCoordinateY << std::endl;
	}
}



bool move_is_available(
	bool y_m,
	bool y_p,
	bool x_m,
	bool x_p,
	int_fast32_t pCoordX,
	int_fast32_t pCoordY
) 
{
	if (y_m) {
		int_fast32_t y_copy = pCoordY - 1;
		return (y_copy >= 0 && y_copy < consoleSizeY);
	}
	if (x_m) {
		int_fast32_t x_copy = pCoordX - 1;
		return (x_copy >= 0 && x_copy < consoleSizeX);
	}
	if (y_p) {
		int_fast32_t y_copy = pCoordY + 1;
		return (y_copy >= 0 && y_copy < consoleSizeY);
	}
	if (x_p) {
		int_fast32_t x_copy = pCoordX + 1;
		return (x_copy >= 0 && x_copy < consoleSizeX);
	}
}


void update_ai_position(
	std::vector<int_fast32_t>& aiX,
	std::vector<int_fast32_t>& aiY,
	std::vector<int_fast32_t>& entityX,
	std::vector<int_fast32_t>& entityY
)
{
	if (aiX[0] - entityX[0] > 0)
	{
		//if (move_is_available(false, false, false, true, aiX[0], aiY[0]))
		aiX[0]--;
	}
	else if(aiX[0] - entityX[0] < 0) {
		//if (move_is_available(false, false, true, false, aiX[0], aiY[0]))
		aiX[0]++;
	}
	if (aiY[0] - entityY[0] > 0)
	{
		//if (move_is_available(true, false, false, false, aiX[0], aiY[0]))
		aiY[0]--;
	}
	else if(aiY[0] - entityY[0] < 0) {
		//if (move_is_available(false, true, false, false, aiX[0], aiY[0]))
		aiY[0]++;
	}
}


void controlls_handler(
	int_fast32_t pCoordX,
	int_fast32_t pCoordY
) 
{
	if (GetKeyState('W') & 0x8000)	
	{
		if (move_is_available(true, false, false, false, pCoordX, pCoordY))
		{
			playerCoordinateY--;
		}
	}
	if (GetKeyState('A') & 0x8000)
	{
		if (move_is_available(false, false, true, false, pCoordX, pCoordY))
		{
			playerCoordinateX--;
		}
	}
	if (GetKeyState('S') & 0x8000)
	{
		if (move_is_available(false, true, false, false, pCoordX, pCoordY))
		{
			playerCoordinateY++;
		}
	}
	if (GetKeyState('D') & 0x8000)
	{
		if (move_is_available(false, false, false, true, pCoordX, pCoordY))
		{
			playerCoordinateX++;
		}
	}
}




int main() 
{


	if (playerCoordinateX >= consoleSizeX || playerCoordinateY >= consoleSizeY) {
		std::string error = std::format(
			"pCoordX {} >= cSizeX {} or pCoordY {} >= cSizeY {}", 
			playerCoordinateX,
			consoleSizeX,
			playerCoordinateY,
			consoleSizeY
		);
		throw std::invalid_argument(error);
	}

	srand(time(nullptr));

	std::vector<int_fast32_t> entityX = {};
	std::vector<int_fast32_t> entityY = {};

	std::vector<int_fast32_t> aiX = {};
	std::vector<int_fast32_t> aiY = {};

	while (true) 
	{
		spawn_ai(
			aiX,
			aiY
		);
		spawn_random_entity(
			entityX,
			entityY
		);
		update_ai_position(
			aiX,
			aiY,
			entityX,
			entityY
		);
		controlls_handler(
			playerCoordinateX,
			playerCoordinateY
		);
		update_game_screen(
			entityY, 
			entityX, 
			aiX, 
			aiY
		);
		std::this_thread::sleep_for(std::chrono::milliseconds(chronoSleep));
	}


	return 0;
}