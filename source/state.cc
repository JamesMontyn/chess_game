#include "../include/state.h"

void State::clearState() {
    for(int i = 0; i < BOARD_SIZE; i++) {
        byteBoard[i] = NO_PIECE;
    }
    turn = false;
    enPassant = false;
    enPassantPos = 0;
    whiteCastlingQueenside = false;
    whiteCastlingKingside = false;
    blackCastlingQueenside = false;
    blackCastlingKingside = false;
}

State::State() {
    clearState();
}

State::State(uint8_t byteBoard[BOARD_SIZE], bool const turn,
             bool const enPassant, int const enPassantPos, bool const whiteCastlingQueenside,
             bool const whiteCastlingKingside, bool const blackCastlingQueenside,
             bool const blackCastlingKingside) {
    this->turn = turn;
    this->enPassant = enPassant;
    this->whiteCastlingQueenside = whiteCastlingQueenside;
    this->whiteCastlingKingside = whiteCastlingKingside;
    this->blackCastlingQueenside = blackCastlingQueenside;
    this->blackCastlingKingside = blackCastlingQueenside;
    for(int i = 0; i < BOARD_SIZE; i++) {
        this->byteBoard[i] = byteBoard[i];
    }
}

State* State::copyState() {
    return new State(byteBoard, turn, enPassant, enPassantPos, 
                     whiteCastlingQueenside, whiteCastlingKingside,
                     blackCastlingQueenside, blackCastlingKingside);
}

bool State::withinBoardLimits(int const pos) {
    return pos >= 0 && pos < BOARD_SIZE;
}

void State::passTurn() {
    turn = !turn;
    enPassant = false;
}

bool State::getTurn() {
    return turn;
}

uint8_t State::getByteFromByteBoard(int const pos) {
    return byteBoard[pos];
}

void State::setByteInByteBoard(uint8_t const byte, int const pos) {
    byteBoard[pos] = byte;
}

void State::setEnPassantPos(int const pos) {
    enPassantPos = pos;
    enPassant = true;
}

bool State::setStateFromFEN(std::string FEN) {
    clearState();
    int const lengthFEN = FEN.length();

    // setting the board
    int pos = 0;
    int i = 0;
    while(i < lengthFEN && pos < BOARD_SIZE) {
        switch(FEN[i]) {
            case '/':
                pos = pos + BOARD_LENGTH - (pos % BOARD_LENGTH) - 1;
                if(pos >= BOARD_SIZE) {
                    return false;
                }
                break;
            case 'p': byteBoard[pos] = BLACK_PAWN; break;
            case 'P': byteBoard[pos] = WHITE_PAWN; break;
            case 'n': byteBoard[pos] = BLACK_KNIGHT; break;
            case 'N': byteBoard[pos] = WHITE_KNIGHT; break;
            case 'b': byteBoard[pos] = BLACK_BISHOP; break;
            case 'B': byteBoard[pos] = WHITE_BISHOP; break;
            case 'r': byteBoard[pos] = BLACK_ROOK; break;
            case 'R': byteBoard[pos] = WHITE_ROOK; break;
            case 'q': byteBoard[pos] = BLACK_QUEEN; break;
            case 'Q': byteBoard[pos] = WHITE_QUEEN; break;
            case 'k': byteBoard[pos] = BLACK_KING; break;
            case 'K': byteBoard[pos] = WHITE_KING; break;
            default:
                int const nrEmptySquares = FEN[i] - '0';
                pos += nrEmptySquares;
                if(pos >= BOARD_SIZE) {
                    return false;
                }
                break;
        }
        pos++;
        i++;
    }

    i++;
    // who's turn is it
    if(i < lengthFEN && FEN[i-1] == ' ') {
        switch(FEN[i]) {
            case 'w': turn = true; break;
            case 'b': turn = false; break;
            default: return false;
        }
    }
    else {
        return false;
    }

    i += 2;
    if(i < lengthFEN && FEN[i-1] == ' ') {
        // castling data
        if(FEN[i] == 'K' || FEN[i] == 'Q' || FEN[i] == 'k' || FEN[i] == 'q') {
            if(FEN[i++] == 'K') {
                whiteCastlingKingside = true;
            }
            if(i < lengthFEN && FEN[i++] == 'Q') {
                whiteCastlingQueenside = true;
            }
            if(i < lengthFEN && FEN[i++] == 'k') {
                blackCastlingKingside = true;
            }
            if(i < lengthFEN && FEN[i++] == 'q') {
                blackCastlingQueenside = true;
            }
        }
        else if(FEN[i++] != '-') {
            return false;
        }
        if(FEN[i++] != ' ') {
            return false;
        }

        // En Passant data
        if(FEN[i++] == '-') {
            enPassant = false;
        }
        else {
            if(i < lengthFEN-1)
            enPassantPos = FEN[i++] - 'a';
            if(enPassantPos >= BOARD_LENGTH && i >= lengthFEN) {
                return false;
            }
            enPassantPos += (FEN[i++] - '0') * BOARD_LENGTH;
            if(enPassantPos >= BOARD_LENGTH) {
                return false;
            }
        }
    }
    else {
        return false;
    }

    return true;
}

// NOT NEEDED RIGHT NOW, SO LEAVING AS IS FOR NOW
// std::string State::getFEN() {
//     std::string FEN = "";
//     int countEmptySquares = 0;
    
//     for(int i = 0; i < BOARD_SIZE; i++) {
//             switch(byteBoard[i]) {
//                 case NO_PIECE:
//                     countEmptySquares = 1;
//                     i++; 
//                     while(byteBoard[i][j] == NO_PIECE && j < BOARD_LENGTH-1) {
//                         countEmptySquares++;
//                         j++;
//                     }
//                     j--;
//                     FEN += char('0' + countEmptySquares);
//                     break;
//                 case BLACK_PAWN: FEN += 'p'; break;
//                 case WHITE_PAWN: FEN += 'P'; break;
//                 case BLACK_KNIGHT: FEN += 'n'; break;
//                 case WHITE_KNIGHT: FEN += 'N'; break;
//                 case BLACK_BISHOP: FEN += 'b'; break;
//                 case WHITE_BISHOP: FEN += 'B'; break;
//                 case BLACK_ROOK: FEN += 'r'; break;
//                 case WHITE_ROOK: FEN += 'R'; break;
//                 case BLACK_QUEEN: FEN += 'q'; break;
//                 case WHITE_QUEEN: FEN += 'Q'; break;
//                 case BLACK_KING: FEN += 'k'; break;
//                 case WHITE_KING: FEN += 'K'; break;
//             }
//             countEmptySquares = 0;
//         }
//         FEN += '/';
//     }

//     FEN += ' ';
//     if(turn) {
//         FEN += 'w';
//     }
//     else {
//         FEN += 'b';
//     }

//     FEN += ' ';
//     if(whiteCastlingKingside || whiteCastlingQueenside || blackCastlingKingside || blackCastlingQueenside) {
//         if(whiteCastlingKingside) {
//             FEN += 'K';
//         }
//         if(whiteCastlingQueenside) {
//             FEN += 'Q';
//         }
//         if(blackCastlingKingside) {
//             FEN += 'k';
//         }
//         if(blackCastlingQueenside) {
//             FEN += 'q';
//         }
//     }
//     else {
//         FEN += '-';
//     }

//     FEN += ' ';
//     if(enPassant) {
//         FEN += char('a' + xEnPassantSquare);
//         FEN += char('0' + yEnPassantSquare);
//     }
//     else {
//         FEN += '-';
//     }

//     return FEN;
// }