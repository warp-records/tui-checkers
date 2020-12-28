#include <iostream>
#include "checkerboard.h"

//---------------------
//it'll do for now lol
void printBoard(CheckerBoard& board) {
	std::cout << "Red Score: " << board.redScore() << 
	"\tWhite Score: " << board.whiteScore() << '\n';

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
        	int out;

        	if (board.pieceExists({ j, 7-i })) {
                Piece pc = board.getPieceCopy({ j, 7-i });

            	if (pc.color == Red) {
                    if (pc.type == Checker) {
                        std::cout << "\u265F";
                    } else {
                        std::cout << "\u265C";
                    }
            	} else {
                    if (pc.type == Checker) {
                        std::cout << "\u2659";
                    } else {
                        std::cout << "\u2654";
                    }
            	}

        	} else {
        		std::cout << ((j+i%2)%2 ? " " : "\u2588");//\u2593
        	}

        	std::cout << ((j+i%2)%2 ? " " : "\u2588");
        }
        std::cout << "\n";
    }
}


Position parseIn(std::string const& in) {
    return Position {toupper(in[0]) - 'A', in[1] - '1'};
}
 
int main() {
    CheckerBoard board;

    while (true) {
        printBoard(board);
        if (board.isCheckmate()) {
            std::cout << (board.getTurn() == Red ? White : Red) << " wins!\n";
            break;
        }
        
        std::cout << "Enter a start and end position:\n";
        Position start;
        std::cin >> start;

        //put a comma between the start and end position
        char ch;
        std::cin >> ch;
        
        Position end;
        std::cin >> end;
        
        board.movePiece(start, end);
    }
    
    return 0;
}

