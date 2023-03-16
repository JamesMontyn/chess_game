#include "../include/game.h"

Game::Game() {
    state = new State();
}

Game::~Game() {
    delete state;
}

bool Game::isKingAttacked(State* state, uint8_t colorOfKing) {
    // TODO
    return false;
}

std::vector<int> Game::getPossibleMoves(uint8_t const pieceByte, int const pos) {
    std::vector<int> possibleMoves;

    // TODO: En Passant move

    int tmpPos = pos;
    bool colorPieceByte = getColorOfPiece(pieceByte);
    switch(pieceByte) {
        case WHITE_PAWN:
            tmpPos += BOARD_LENGTH;
            if(state->withinBoardLimits(tmpPos)
               && state->getByteFromByteBoard(tmpPos) == NO_PIECE) {
                possibleMoves.push_back(tmpPos);
            } // one step upwards move
            tmpPos += BOARD_LENGTH;
            if(pos >= BOARD_LENGTH && pos < 2*BOARD_LENGTH 
               && state->withinBoardLimits(tmpPos)
               && state->getByteFromByteBoard(tmpPos-BOARD_LENGTH) == NO_PIECE
               && state->getByteFromByteBoard(tmpPos) == NO_PIECE) {
                possibleMoves.push_back(tmpPos);
            } // two step upwards move (only possible if at y=BOARD_LENGTH-1)
            tmpPos = pos+BOARD_LENGTH-1;
            if(state->withinBoardLimits(tmpPos)) {
                uint8_t tmp = state->getByteFromByteBoard(tmpPos);
                if((tmp != NO_PIECE && colorPieceByte != getColorOfPiece(tmp))
                    || state->getEnPassantPos() == tmpPos) {
                    possibleMoves.push_back(tmpPos);
                }
            } // left diagonal take
            tmpPos += 2;
            if(state->withinBoardLimits(tmpPos)) {
                uint8_t tmp = state->getByteFromByteBoard(tmpPos);
                if((tmp != NO_PIECE && colorPieceByte != getColorOfPiece(tmp))
                    || state->getEnPassantPos() == tmpPos) {
                    possibleMoves.push_back(tmpPos);
                }
            } // right diagonal take
            break;
        case BLACK_PAWN:
            tmpPos -= BOARD_LENGTH;
            if(state->withinBoardLimits(tmpPos)
               && state->getByteFromByteBoard(tmpPos) == NO_PIECE) {
                possibleMoves.push_back(tmpPos);
            } // one step upwards move
            tmpPos -= BOARD_LENGTH;
            if(pos >= BOARD_LENGTH && pos < 2*BOARD_LENGTH 
               && state->withinBoardLimits(tmpPos)
               && state->getByteFromByteBoard(tmpPos-BOARD_LENGTH) == NO_PIECE
               && state->getByteFromByteBoard(tmpPos) == NO_PIECE) {
                possibleMoves.push_back(tmpPos);
            } // two step upwards move (only possible if at y=BOARD_LENGTH-1)
            tmpPos = pos-BOARD_LENGTH-1;
            if(state->withinBoardLimits(tmpPos)) {
                uint8_t tmp = state->getByteFromByteBoard(tmpPos);
                if((tmp != NO_PIECE && colorPieceByte != getColorOfPiece(tmp))
                    || state->getEnPassantPos() == tmpPos) {
                    possibleMoves.push_back(tmpPos);
                }
            } // left diagonal take
            tmpPos += 2;
            if(state->withinBoardLimits(tmpPos)) {
                uint8_t tmp = state->getByteFromByteBoard(tmpPos);
                if((tmp != NO_PIECE && colorPieceByte != getColorOfPiece(tmp))
                    || state->getEnPassantPos() == tmpPos) {
                    possibleMoves.push_back(tmpPos);
                }
            } // right diagonal take
            break;
        case WHITE_KNIGHT:
        case BLACK_KNIGHT:
            int directionIncrementals[8] = {NORTH+NORTH+EAST, NORTH+NORTH+WEST, SOUTH+SOUTH+EAST, SOUTH+SOUTH+WEST, 
                                            EAST+EAST+NORTH, EAST+EAST+SOUTH, WEST+WEST+NORTH, WEST+WEST+SOUTH}; 

            for(int i = 0; i < 8; i++) {
                tmpPos = pos+directionIncrementals[i];
                if(state->withinBoardLimits(tmpPos)) {
                    uint8_t tmp = state->getByteFromByteBoard(tmpPos);
                    if(tmp == NO_PIECE || colorPieceByte != getColorOfPiece(tmp)) {
                        possibleMoves.push_back(tmpPos);
                    }
                }
            }
            break;
        case WHITE_QUEEN: // queen is same as bishop + rook
        case BLACK_QUEEN:
        case WHITE_BISHOP:
        case BLACK_BISHOP: {
            int directionIncrementals[4] = {NORTH+EAST, NORTH+WEST, SOUTH+EAST, SOUTH+WEST};

            for(int i = 0; i < 4; i++) {
                tmpPos = pos+directionIncrementals[i];
                while(state->withinBoardLimits(tmpPos)) {
                    uint8_t tmp = state->getByteFromByteBoard(tmpPos);
                    if(tmp == NO_PIECE) {
                        possibleMoves.push_back(tmpPos);
                        tmpPos = tmpPos+directionIncrementals[i];
                        continue;
                    }
                    if(colorPieceByte != getColorOfPiece(tmp)) {
                        possibleMoves.push_back(tmpPos);
                    }
                    break;
                }
            }

            if(pieceByte != WHITE_QUEEN && pieceByte != BLACK_QUEEN) {
                break;
            } // no break if the piece is a queen (queen can also do rook moves)
            
            // reset tmpX, tmpY for queen to check rook moves
            tmpPos = pos;
        }
        case WHITE_ROOK:
        case BLACK_ROOK: {
            int directionIncrementals[4] = {NORTH, EAST, WEST, SOUTH};

            for(int i = 0; i < 4; i++) {
                tmpPos = pos+directionIncrementals[i];
                while(state->withinBoardLimits(tmpPos)) {
                    uint8_t tmp = state->getByteFromByteBoard(tmpPos);
                    if(tmp == NO_PIECE) {
                        possibleMoves.push_back(tmpPos);
                        tmpPos = tmpPos+directionIncrementals[i];
                        continue;
                    }
                    if(colorPieceByte != getColorOfPiece(tmp)) {
                        possibleMoves.push_back(tmpPos);
                    }
                    break;
                }
            }
            break;
        }
        case WHITE_KING:
        case BLACK_KING: {
            int directionIncrementals[8] = {NORTH+EAST, NORTH+WEST, SOUTH+EAST, SOUTH+WEST, 
                                            NORTH, EAST, WEST, SOUTH}; 

            for(int i = 0; i < 8; i++) {
                tmpPos = pos+directionIncrementals[i];
                if(state->withinBoardLimits(tmpPos)) {
                    uint8_t tmp = state->getByteFromByteBoard(tmpPos);
                    if(tmp == NO_PIECE || colorPieceByte != getColorOfPiece(tmp)) {
                        possibleMoves.push_back(tmpPos);
                    }
                }
            }
        }
    }

    return possibleMoves;                                                   
}



std::vector<int> Game::getLegalMovesOfPiece(uint8_t const pieceByte, int const pos) {
    /**
     * A legal move of a piece is:
     * 1. a move according to the rules of the piece
     * 2. a move that is within the bounds of the board
     * 3. a move that is either:
     *    3.1 to an empty square
     *    3.2 to a piece of the opponent's color
     * 4. [RULE 4 is exempted for knights] a move that does not "hop over" pieces
     * 5. a move that does not leaves your own king attacked afterwards
    */

    // TODO: for en passant we can just check if difference prevY and newY is 2

    std::vector<int> legalMoves = getPossibleMoves(pieceByte, pos);
    State* stateCopy = state->copyState();

    return legalMoves;  
}