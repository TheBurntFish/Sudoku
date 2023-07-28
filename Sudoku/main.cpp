#include <stdio.h>
#include <SDL.h>

#include "functions.h"

int main(int argc, char* argv[]) {
	bool quit = false;
	SDL_Event event;

	if (!initializeSDL()) {
		quit = true;
	}
	if (!initializeWindow()) {
		quit = true;
	}
	if (!initializeRenderer()) {
		quit = true;
	}
	RandomizeStart();
	setupRectBoards();
	solveSudoku();
	unsolveSudoku();

	lockCells();
	highlight();
	drawBoard();
	drawControl();
	while (!quit){
		while (SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT){
				quit = true;
				break;
			}
			
			if (event.type == SDL_MOUSEBUTTONDOWN){
				int mouseX = (int)event.button.x / CELL_SIZE;
				int mouseY = (int)event.button.y / CELL_SIZE;
				cellSelect(mouseX, mouseY);
				highlight();
				drawBoard();
				if (isComplete) {
					std::cout << "win";
				}
			}
		}

		// Render graphics, update game logic, etc.
	}
	wrapup();
	return 0;
}