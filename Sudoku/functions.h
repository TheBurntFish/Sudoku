#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <vector>
#include <iostream>
#include <utility>
#include <random>


#include <chrono>
#include <thread>

//y then x
//rows then col
char board[9][9]{
	{' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' '},
	{' ',' ',' ',' ',' ',' ',' ',' ',' '}
};

//char board[9][9]{
//	{' ',' ','6',' ','1',' ',' ','8','4'},
//	{' ',' ','8',' ','9','4',' ',' ',' '},
//	{'9','4',' ',' ',' ',' ','6','1','3'},
//	{' ',' ','1','4',' ','9','8',' ','2'},
//	{' ',' ',' ','2','8',' ','7',' ','5'},
//	{'4','8',' ',' ','3',' ',' ',' ','6'},
//	{'5','1','4','9',' ','6',' ',' ','8'},
//	{'3',' ',' ',' ',' ','8','4',' ','9'},
//	{' ',' ',' ',' ','4','7',' ','6',' '}
//};
char controlBoard[3][3]{
	{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'}
};

//GAMESTATE = "Number Select";
std::random_device rd;
std::mt19937 generator(rd());

SDL_Window* WINDOW;
//9 100
int NUM_CELLS = 9;
int CELL_SIZE = 50;

int BTN_SIZE = 50;

int TABLE_WIDTH = NUM_CELLS * CELL_SIZE;
int TABLE_HEIGHT = NUM_CELLS * CELL_SIZE;

int CONTROL_WIDTH = BTN_SIZE * 3;
int CONTROL_HEIGHT = BTN_SIZE * 4;

int WINDOW_WIDTH = TABLE_WIDTH + CONTROL_WIDTH;
int WINDOW_HEIGHT = TABLE_HEIGHT;

bool initializeSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("SDL initialization failed");
		return false;
	}
	return true;
}

bool initializeWindow() {
	WINDOW = SDL_CreateWindow("Window Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (WINDOW == nullptr) {
		// Window creation failed
		// Handle error...
		printf("SDL Window creation failed");
		SDL_Quit();
		return false;
	}
	return true;
}

SDL_Renderer* RENDERER;
bool initializeRenderer() {
	RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED);
	if (RENDERER == nullptr)
	{
		// Renderer creation failed
		// Handle error...
		printf("SDL Renderer creation failed");
		SDL_DestroyWindow(WINDOW);
		SDL_Quit();
		return false;
	}
	return true;
}

SDL_Rect rect;

SDL_Surface* ONE = IMG_Load("images/1.png");
SDL_Surface* TWO = IMG_Load("images/2.png");
SDL_Surface* THREE = IMG_Load("images/3.png");
SDL_Surface* FOUR = IMG_Load("images/4.png");
SDL_Surface* FIVE = IMG_Load("images/5.png");
SDL_Surface* SIX = IMG_Load("images/6.png");
SDL_Surface* SEVEN = IMG_Load("images/7.png");
SDL_Surface* EIGHT = IMG_Load("images/8.png");
SDL_Surface* NINE = IMG_Load("images/9.png");

SDL_Surface* ERASER = IMG_Load("images/eraser.png");

SDL_Texture* texture;

SDL_Rect rectBoard[9][9];
SDL_Rect rectControlBoard[4][3];

void setupRectBoards() {
	// 9x9 table
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			rect = { (col)*CELL_SIZE, (row)*CELL_SIZE, CELL_SIZE - 2, CELL_SIZE - 2 };
			rectBoard[row][col] = rect;
		}
	}

	// 3x3 control board
	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			rect = { (col + 9) * BTN_SIZE, (row + 3) * BTN_SIZE, CELL_SIZE - 2, CELL_SIZE - 2 };
			rectControlBoard[row][col] = rect;
		}
	}

	// eraser
	rect = { (10) * BTN_SIZE, (6) * BTN_SIZE, CELL_SIZE - 2, CELL_SIZE - 2 };
	rectControlBoard[3][2] = rect;


}

bool isComplete;

int selectedX = 10;
int selectedY = 10;

void highlight() {
	isComplete = true;
	for (int row = 0; row < NUM_CELLS; row++) {
		for (int col = 0; col < NUM_CELLS; col++) {
			//highlight row and column of selected
			if (row == selectedY || col == selectedX) {
				SDL_SetRenderDrawColor(RENDERER, 100, 255, 100, 255);
				SDL_RenderFillRect(RENDERER, &rectBoard[row][col]);
				//if same value highlight red
				if (board[selectedY][selectedX] == board[row][col] && !(selectedY == row && selectedX == col)) {
					isComplete = false;
					if (board[selectedY][selectedX] != ' ') {
						SDL_SetRenderDrawColor(RENDERER, 255, 50, 50, 255);
						SDL_RenderFillRect(RENDERER, &rectBoard[row][col]);
					}
				}
			}
			else {
				SDL_SetRenderDrawColor(RENDERER, 255, 255, 255, 255);
				SDL_RenderFillRect(RENDERER, &rectBoard[row][col]);
				//color same value
				if (board[selectedY][selectedX] == board[row][col] && board[selectedY][selectedX] != ' ') {
					SDL_SetRenderDrawColor(RENDERER, 255, 255, 100, 255);
					SDL_RenderFillRect(RENDERER, &rectBoard[row][col]);
				}
				//highlight same 3x3
				int subgridStartRow = selectedY - selectedY % 3;
				int subgridStartCol = selectedX - selectedX % 3;
				for (int i = 0; i < 3; ++i) {
					for (int j = 0; j < 3; ++j) {
						if (board[subgridStartRow + i][subgridStartCol + j] == board[selectedY][selectedX] && !(selectedY == subgridStartRow + i && selectedX == subgridStartCol + j)) {
							if (board[selectedY][selectedX] != ' ') {
								SDL_SetRenderDrawColor(RENDERER, 255, 50, 50, 255);
								SDL_RenderFillRect(RENDERER, &rectBoard[subgridStartRow + i][subgridStartCol + j]);
							}
						}
					}
				}
			}
		}
	}
}



void drawBoard() {

	for (int row = 0; row < NUM_CELLS; row++) {
		for (int col = 0; col < NUM_CELLS; col++) {
			if (board[row][col] == ' ') {
				isComplete = false;
			}
			// Check if the number is already present in the 3x3 subgrid




			SDL_SetRenderDrawColor(RENDERER, 255, 255, 255, 255);
			if (board[row][col] == '1') {
				texture = SDL_CreateTextureFromSurface(RENDERER, ONE);
				SDL_RenderCopy(RENDERER, texture, NULL, &rectBoard[row][col]);
			}
			if (board[row][col] == '2') {
				texture = SDL_CreateTextureFromSurface(RENDERER, TWO);
				SDL_RenderCopy(RENDERER, texture, NULL, &rectBoard[row][col]);
			}
			if (board[row][col] == '3') {
				texture = SDL_CreateTextureFromSurface(RENDERER, THREE);
				SDL_RenderCopy(RENDERER, texture, NULL, &rectBoard[row][col]);
			}
			if (board[row][col] == '4') {
				texture = SDL_CreateTextureFromSurface(RENDERER, FOUR);
				SDL_RenderCopy(RENDERER, texture, NULL, &rectBoard[row][col]);
			}
			if (board[row][col] == '5') {
				texture = SDL_CreateTextureFromSurface(RENDERER, FIVE);
				SDL_RenderCopy(RENDERER, texture, NULL, &rectBoard[row][col]);
			}
			if (board[row][col] == '6') {
				texture = SDL_CreateTextureFromSurface(RENDERER, SIX);
				SDL_RenderCopy(RENDERER, texture, NULL, &rectBoard[row][col]);
			}
			if (board[row][col] == '7') {
				texture = SDL_CreateTextureFromSurface(RENDERER, SEVEN);
				SDL_RenderCopy(RENDERER, texture, NULL, &rectBoard[row][col]);
			}
			if (board[row][col] == '8') {
				texture = SDL_CreateTextureFromSurface(RENDERER, EIGHT);
				SDL_RenderCopy(RENDERER, texture, NULL, &rectBoard[row][col]);
			}
			if (board[row][col] == '9') {
				texture = SDL_CreateTextureFromSurface(RENDERER, NINE);
				SDL_RenderCopy(RENDERER, texture, NULL, &rectBoard[row][col]);
			}
			SDL_DestroyTexture(texture);
		}
	}

	SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 255);
	for (int i = 0; i < NUM_CELLS; i += 3) {
		//thick vertical lines
		rect = { i * CELL_SIZE, 0, 2 , TABLE_HEIGHT };
		SDL_RenderFillRect(RENDERER, &rect);
		//thick horizontal lines
		rect = { 0 , i * CELL_SIZE, TABLE_WIDTH , 2 };
		SDL_RenderFillRect(RENDERER, &rect);
	}

	SDL_RenderPresent(RENDERER);
}

void drawControl() {
	SDL_SetRenderDrawColor(RENDERER, 100, 100, 255, 255);
	for (int col = 0; col < 3; col++) {
		for (int row = 0; row < 3; row++) {
			//draws the grid for the right control panel
			SDL_RenderFillRect(RENDERER, &rectControlBoard[row][col]);
			if (controlBoard[row][col] == '1') {
				texture = SDL_CreateTextureFromSurface(RENDERER, ONE);
				SDL_RenderCopy(RENDERER, texture, NULL, &rectControlBoard[row][col]);
			}
			if (controlBoard[row][col] == '2') {
				texture = SDL_CreateTextureFromSurface(RENDERER, TWO);
				SDL_RenderCopy(RENDERER, texture, NULL, &rectControlBoard[row][col]);
			}
			if (controlBoard[row][col] == '3') {
				texture = SDL_CreateTextureFromSurface(RENDERER, THREE);
				SDL_RenderCopy(RENDERER, texture, NULL, &rectControlBoard[row][col]);
			}
			if (controlBoard[row][col] == '4') {
				texture = SDL_CreateTextureFromSurface(RENDERER, FOUR);
				SDL_RenderCopy(RENDERER, texture, NULL, &rectControlBoard[row][col]);
			}
			if (controlBoard[row][col] == '5') {
				texture = SDL_CreateTextureFromSurface(RENDERER, FIVE);
				SDL_RenderCopy(RENDERER, texture, NULL, &rectControlBoard[row][col]);
			}
			if (controlBoard[row][col] == '6') {
				texture = SDL_CreateTextureFromSurface(RENDERER, SIX);
				SDL_RenderCopy(RENDERER, texture, NULL, &rectControlBoard[row][col]);
			}
			if (controlBoard[row][col] == '7') {
				texture = SDL_CreateTextureFromSurface(RENDERER, SEVEN);
				SDL_RenderCopy(RENDERER, texture, NULL, &rectControlBoard[row][col]);
			}
			if (controlBoard[row][col] == '8') {
				texture = SDL_CreateTextureFromSurface(RENDERER, EIGHT);
				SDL_RenderCopy(RENDERER, texture, NULL, &rectControlBoard[row][col]);
			}
			if (controlBoard[row][col] == '9') {
				texture = SDL_CreateTextureFromSurface(RENDERER, NINE);
				SDL_RenderCopy(RENDERER, texture, NULL, &rectControlBoard[row][col]);
			}
		}
	}
	// eraser
	SDL_RenderFillRect(RENDERER, &rectControlBoard[3][2]);
	texture = SDL_CreateTextureFromSurface(RENDERER, ERASER);
	SDL_RenderCopy(RENDERER, texture, NULL, &rectControlBoard[3][2]);
	SDL_RenderPresent(RENDERER);
}

std::vector<std::pair<int, int>> lockedCells;
void lockCells() {
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			if (board[row][col] != ' ') {
				lockedCells.push_back(std::make_pair(row, col));
			}
			//std::cout << board[row][col] << " ";
		}
		//std::cout << std::endl;
	}
}

bool checkLockedCells(int x, int y) {
	for (const auto& cells : lockedCells) {
		if (cells.first == y && cells.second == x) {
			return false;
		}
	}
	return true;
}

void cellSelect(int x, int y) {
	if (x < 9 && y < 9) {
		selectedX = x;
		selectedY = y;
	}
	else if (3 <= y && y < 6) {
		char num = controlBoard[y - 3][x - 9];
		/*std::cout << num << std::endl;
		std::cout << selectedX << ", " << selectedY << std::endl;*/
		if (selectedX != 10 && selectedY != 10) {
			if (checkLockedCells(selectedX, selectedY)) {
				board[selectedY][selectedX] = num;
			}
		}
	}
	else if (y == 6 && x == 10) {
		if (selectedX != 10 && selectedY != 10) {
			if (checkLockedCells(selectedX, selectedY)) {
				board[selectedY][selectedX] = ' ';
			}
		}
	}
}



int random() {
	std::uniform_int_distribution<int> distribution(0, 8);
	return distribution(generator);
}

// Function to check if a value can be placed in a particular position
bool isSafe(int row, int col, char value) {
	// Check if the number is already present in the row or column
	for (int i = 0; i < 9; ++i) {
		if (board[row][i] == value || board[i][col] == value) {
			return false;
		}
	}

	// Check if the number is already present in the 3x3 subgrid
	int subgridStartRow = row - row % 3;
	int subgridStartCol = col - col % 3;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (board[subgridStartRow + i][subgridStartCol + j] == value) {
				return false;
			}
		}
	}

	return true;
}


bool match = false;
int vert;
int horz;
void RandomizeStart() {
	for (int i = 0; i < 20; i++) {
		match = false;
		int randY = random(); // random y value
		int randX = random(); // random x
		char randV = static_cast<char>(random() + '0');



		if (isSafe(randY, randX, randV)) {
			board[randY][randX] = randV;
			//std::cout << "hello " << randY << randX << randV << std::endl;
		}
	}
}

//recursion time
// Recursive function to solve the Sudoku puzzle
bool solveSudoku() {
	// Find an unassigned position (cell with value 0)
	int row, col;
	bool isUnassigned = false;
	for (row = 0; row < 9; ++row) {
		for (col = 0; col < 9; ++col) {
			if (board[row][col] == ' ' || board[row][col] == '0') {
				isUnassigned = true;
				break;
			}
		}
		if (isUnassigned) {
			break;
		}
	}

	// If all positions are assigned, the puzzle is solved
	if (!isUnassigned) {
		return true;
	}

	// Try assigning values from 1 to 9
	for (int num = 1; num <= 9; ++num) {
		char value = static_cast<char>(num + '0');
		if (isSafe(row, col, value)) {
			// Assign the value and recursively solve for the remaining grid
			board[row][col] = value;
			SDL_RenderClear(RENDERER);
			highlight();
			drawBoard();
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			if (solveSudoku()) {
				return true;
			}
			// Backtrack and try a different value
			board[row][col] = ' ';
		}
	}

	return false;
}

void unsolveSudoku() {
	int removePairs = 20;
	int randY;
	int randX;
	int mirrorY;
	int mirrorX;
	while (removePairs > 0) {
		randY = random();
		randX = random();
		// so like 0-8 -> 8-0 kinda
		mirrorY = (randY * -1) + 8;
		mirrorX = (randX * -1) + 8;
		if (board[randY][randX] != ' ') {
			board[randY][randX] = ' ';
			board[mirrorY][mirrorX] = ' ';
			removePairs--;
		}
	}
}



void wrapup() {
	SDL_DestroyTexture(texture);

	SDL_FreeSurface(ONE);
	SDL_FreeSurface(TWO);
	SDL_FreeSurface(THREE);
	SDL_FreeSurface(FOUR);
	SDL_FreeSurface(FIVE);
	SDL_FreeSurface(SIX);
	SDL_FreeSurface(SEVEN);
	SDL_FreeSurface(EIGHT);
	SDL_FreeSurface(NINE);

	SDL_DestroyRenderer(RENDERER);
	SDL_DestroyWindow(WINDOW);
	SDL_Quit();
}