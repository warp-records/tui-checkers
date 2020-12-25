#include <iostream>
#include <vector>

enum Color { Red, White };

std::ostream& operator<<(std::ostream& os, Color color);

enum Type { Checker, King };

struct Position {
    int x;
    int y;
};

std::istream& operator>>(std::istream& is, Position& pos);
std::ostream& operator<<(std::ostream& os, Position pos);

struct Piece {
    Color color;
    Type type;
};


class CheckerBoard {
    //constexpr int sz = 8;
    Piece* pieces[8][8];
    Color turn;
    //how many points they have
    int redSc = 0;
    int whiteSc = 0;

    //the piece currently attempting to jump
    Piece* jumper;
    /*list of positions of pieces on the "jumpedStack" during
    the moveValid() function call*/
    std::vector<Position> jumpStack;

    Piece* getPiece(Position pos) const { return pieces[pos.x][pos.y]; }
    //brokenn, you CANT make this work!
    /*void deletePiece(Piece* pc) {
    	delete[] pc;
    	pc = nullptr;
    }*/
    bool moveValid(Position start, Position end, bool recursive);
    bool pieceExists(Piece* pc) const { 
        return pc != nullptr;
    }
    //bool canMove(Position pos) const;

    /*Will check if there is a way to make a jump
    from one point to another, can check recursively
    to handle multiple jumps. Will return a vector
    of all the positions jumped if the jump can be made,
    otherwise it will return an empty vector*/

    void deletePiece(Position pos) {
    	pieces[pos.x][pos.y] = nullptr;
    	delete[] getPiece(pos);
    }

    //for actually moving it
    void transportPiece(Position start, Position end) {
	    //actually move the fucking thing
	    pieces[end.x][end.y] = pieces[start.x][start.y];
	    //remove the old reference
    	pieces[start.x][start.y] = nullptr;
	}

    bool canMove(Position pos);

public:
    bool pieceExists(Position pos) const {
        if (pos.x < 0 || pos.y < 0 || pos.x > 8 || pos.y > 8)
            return false;

        return getPiece(pos) != nullptr; 
    }
    bool isCheckmate();
    int redScore() const { return redSc; }
    int whiteScore() const { return whiteSc; }
    Color getTurn() const { return turn; }

    //lol
    void movePiece(Position start, Position end);
    //don't let the public interface show 
    Piece getPieceCopy(Position pos) const {
    	return *getPiece(pos);
    }


    //getPiece(Position pos); 
    CheckerBoard();
    //destroy that bitch
    ~CheckerBoard();
};

