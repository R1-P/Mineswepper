#include "geesespotter_lib.h"

char* createBoard(std::size_t xdim, std::size_t ydim){
	std::size_t capacity = xdim * ydim;

	char* newBoard = new char [capacity];
	for (std::size_t k = 0; k < capacity; ++k) {
		newBoard[k] = 0;
	}
	return newBoard;
}

void computeNeighborsHelper(char* board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {
	int i = 0;
	int j = 0;
	int counter = 0; 

	if ((board[yloc * xdim + xloc] & valueMask()) == 0x09) {
		return; 
	}
	for (i = -1; i < 2; ++i) {
		for (j = -1; j < 2; ++j) {
			if ((yloc + j) >= 0 && (yloc + j) < ydim && (xloc + i) >= 0 && (xloc + i) < xdim) {
				if ((board[(yloc + j) * xdim + xloc + i] & valueMask()) == 0x09) {
					++counter;
				}
			}
		}
	}
	board[yloc * xdim + xloc] += counter; 
	return; 
}

void computeNeighbors(char* board, std::size_t xdim, std::size_t ydim) {
	int index = 0;
	for (index = 0; index < xdim * ydim; ++index) {
		computeNeighborsHelper(board, xdim, ydim, index % xdim, index / xdim);
	}
	return; 
}

void hideBoard(char* board, std::size_t xdim, std::size_t ydim) {
	for (std::size_t k = 0; k < xdim * ydim; ++k) {  
		board[k] |= hiddenBit();
	}
	return; 
}

void cleanBoard(char* board) {

	delete[] board;
	board = nullptr;
	
}

void printBoard(char* board, std::size_t xdim, std::size_t ydim) {
	for (std::size_t k = 0; k < ydim * xdim; k = k+xdim) {
		for (std::size_t i = 0; i < xdim; ++i) {
			if ((board[k + i] & markedBit()) == markedBit()) {
			std::cout << "M";
			}
			 else if ((board[k + i] & hiddenBit()) == hiddenBit()) {
				std::cout << "*";
			}
			 else {
				std::cout << (int)board[k + i]; 
			}
		}
		std::cout <<std::endl ;
	}
}

int reveal(char* board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {

	if ((board[yloc * xdim + xloc] & markedBit()) == markedBit()) {
		return 1;
	}

	else if ((board[yloc * xdim + xloc] & valueMask()) == board[yloc * xdim + xloc]) {
		return 2;
	}

	else if ((board[yloc * xdim + xloc] & valueMask()) == 0x09) {
		board[yloc * xdim + xloc] &= valueMask();
		return 9;
	}
	//only when adj value is zero
	
	else {
		board[yloc * xdim + xloc] &= valueMask();

		if ((board[yloc * xdim + xloc] & valueMask()) == 0x00) {
			int edgex = xloc;
			int edgey = yloc; 
			if (xloc == 0x00) {
				edgex = edgex+ 1;
			}
			if (yloc == 0x00) {
				edgey = edgey + 1;
			}
			for (int rev_y = edgey - 1; rev_y <= yloc + 1; ++rev_y) {
				for (int rev_x = edgex - 1; rev_x <= xloc + 1; ++rev_x) {
					//check if still on board
					if (rev_x < xdim && rev_y < ydim ) {
						//from the statement to reveal the space
							//valuemask forces 4 left bits off-- > goes to zero
						if ((board[rev_y * xdim + rev_x] & markedBit()) != markedBit()) {
							board[rev_y * xdim + rev_x] &= valueMask();
						}
					}
				}
			}
		}
	}
	

	return 0;

}

int mark(char* board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) { 



	if ((board[yloc * xdim + xloc] & valueMask()) == board[yloc * xdim + xloc]) {
		return 2;
	}
	else if ((board[yloc * xdim + xloc] & markedBit()) == markedBit()) {
		board[yloc * xdim + xloc] ^= markedBit(); 
		return 0 ;
	}

	board[yloc * xdim + xloc] |= markedBit();

	return 0;
}

bool isGameWon(char* board, std::size_t xdim, std::size_t ydim) {
	unsigned int i = 0;
	for ( i = 0; i < xdim * ydim; ++i) {
		if ((board[i] & hiddenBit()) == hiddenBit()) {
			if ((board[i] & valueMask()) != 0x09) {
				return false; 
			}
		}
	}
	return true;		
}
