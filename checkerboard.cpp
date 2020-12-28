#include "checkerboard.h"


enum Color;
enum Type;

struct Position;

struct Piece;


std::ostream& operator<<(std::ostream& os, Color color) {
    return os << (color == Red ? "Red" : "White");
}

std::istream& operator>>(std::istream& is, Position& pos) {
    int x, y;
    char ch1, ch2, ch3;
    is >> ch1 >> x >> ch2 >> y >> ch3;
    
    if (ch1 != '(' || ch2 != ',' || ch3 != ')') throw std::exception();
    pos.x = x;
    pos.y = y;
    
    return is;
}

bool operator==(Position pos1, Position pos2) {
    return pos1.x == pos2.x && pos1.y == pos2.y;
}

bool operator!=(Position pos1, Position pos2) {
    return !(pos1 == pos2);
}

std::ostream& operator<<(std::ostream& os, Position pos) {
    return os << '(' << pos.x << ", " << pos.y << ')';
}



CheckerBoard::CheckerBoard() {
	turn = Red;

    //wipe the array
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            pieces[i][j] = nullptr;
        }
    }

    //initialize piece and piece positions
    for (int i = 0; i < 8; i++) {
        /*put red checkers on the "bottom" 
        and white checkers on the "top"*/
        pieces[i][i%2] = new Piece {Red, Checker};
        
        pieces[i][7-(i+1)%2] = new Piece {White, Checker};
    }

    for (int i = 0; i < 4; i++) {
        pieces[i*2][2] = new Piece {Red, Checker};

        pieces[i*2+1][5] = new Piece {White, Checker};
    }

    /*pieces[2][2] = new Piece {White, Checker};
    pieces[2][4] = new Piece {White, Checker};
    deletePiece({3, 7});*/
}


bool CheckerBoard::moveValid(Position start, Position end, bool recursive = false) {
    Piece* pc;

    if (!recursive) {
        pc = getPiece(start);
        jumper = nullptr;
    } else if (start == end) {
        return true;
    } else {
        pc = jumper;
    }
    
    int xdist = end.x - start.x;
    int ydist = end.y - start.y;
    
    if (!pc && !recursive)
        return false;
    if (pieceExists(end))
        return false;
        
    //make sure direction matches color, or ignore if king
    if (pc->type != King) {
        if (pc->color == Red && ydist < 0)
            return false;
        if (pc->color != Red && ydist > 0)
            return false;
    }

    //ensure move is in bounds
    if (0 > end.x || end.x >= 8)
        return false;
    if (0 > end.y || end.y >= 8)
        return false;
    
    //one-square diagonally is a non-jump move
    if (abs(xdist) == 1)
        return true;
        
    //jump moves go two squares diagonally
    if (abs(xdist)%2 == 0 && abs(ydist)%2 == 0) {
        //initialize the jumping piece
        if (!jumper) {
            jumper = pc;
        }

        for (int i = -2; i <= 2; i += 4) {
            for (int j = -2; j <= 2; j += 4) {

                Position jumpedPos = { (start.x*2 + j)/2, (start.y*2 + i)/2 };
                Piece* jumped = getPiece(jumpedPos);

                Position attemptedMove {start.x+j, start.y+i};

                //std::cout << "attemptedMove: " << attemptedMove << '\n';

                if (pieceExists(jumpedPos) && jumped->color != jumper->color) {
                        if (jumpStack.size() == 0 || jumpedPos != jumpStack.back()) {
                        jumpStack.push_back(jumpedPos);
                        //std::cout << "Jumped to " << attemptedMove << "!\n";
                        //REUCRSION GO BRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR!!!!!
                        if (moveValid(attemptedMove, end, true))
                            return true;
                    }
                }


            }
        }
    }
    
    //move is invalid
    return false;
}

//only used by isCheckmate()
bool CheckerBoard::canMove(Position pos) {
    //len is the lenght of the move
    //for (int len = 1; len < 2; len++) {
        for (int i = -1; i <= 1; i+=2) {
            for (int j = -1; j <= 1; j+=2) {
                if (moveValid(pos, { pos.x + j, pos.y + i }))
                    return true;
            }
        }
    //}

    return false;
}


//this is gonna be slow af... I'm gonna try and optimize it though
bool CheckerBoard::isCheckmate() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j+=2) {
            Position pos { j+i%2, i };
            Piece* pc = getPiece(pos);

            /*std::cout << "Looking at position: (" << pos.x << ", " << pos.y << ")\n";
            if (pieces[pos.x][pos.y] != nullptr) {
                std::cout << "Found a " << (pc->color) << " piece at this position!\n";
            }*/
            //std::cout << pieces[pos.x][pos.y];
            if (pieceExists(pc) && pc->color == turn && canMove(pos))
                return false;
        }
    }

    return true;
}

void CheckerBoard::movePiece(Position start, Position end) {
    Piece* startPc = getPiece(start);

    if (moveValid(start, end) && startPc->color == getTurn()) {
    	//it's a (single) jump
        while (jumpStack.size() > 0) {
    		Position jumpedPos = jumpStack.back();
    		Piece* jumped = getPiece(jumpedPos);

    		//increment team score, based on what type of piece it is
	    	if (startPc->color == Red)
	    		redSc += (jumped->type == Checker ? 1 : 2);
	    	else
	    		whiteSc += (jumped->type == Checker ? 1 : 2);

    		deletePiece(jumpedPos);
            jumpStack.pop_back();
    	}

        /*if a checker reaches the other side, promote it to a king>
        And no, we don't need to check the color, since a checker type 
        piece will never make a move ending on its own side*/ 
    	if (startPc->type == Checker && (end.y == 0 || end.y == 7)) {
            startPc->type = King;
        }

    	transportPiece(start, end);

        //change turns
        turn = turn == Red ? White : Red;

    } else {
    	//Well, I dunno what else to do :)
        std::cout << "Move is invalid!\n";
    }
}

CheckerBoard::~CheckerBoard() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (pieceExists({ j, i }))
				deletePiece({ j, i });
		}
	}

}
