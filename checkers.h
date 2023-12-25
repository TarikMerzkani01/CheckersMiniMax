
//~~~~~~~~~~~~~~~~~~~~!!!!!!!!!!!!!!!!!!!!!!!!!!!!////////////////////
//Written By Tarik Merzkani (5/11/2022)
//~~~~~~~~~~~~~~~~~~~~!!!!!!!!!!!!!!!!!!!!!!!!!!!!////////////////////

#pragma once

typedef unsigned long bitboard;     
//Represents board. Total Game Board is an array of 3 bitboards

typedef unsigned char action;      
//Represents an action. First six bits are position of the bit, last two bits are direction
//The action will be interpreted as a move or capture depending on if given to
//the respective makeAction()/makeCapture() functions.
//For last two bits, 6th bit is boolean for right (0 for left), 
//7th bit is boolean for down (0 for up). Example:
//UpLeft: 00, UpRight: 01, DownLeft: 10, DownRight: 11
//UpLeft: 0, UpRight: 1, DownLeft: 2, DownRight: 3
//action for red piece at bit position 34 (C5 in board) to move right
//action: 01 100010
//       dir  bitposvalue

//Movement definitions:
#define UpLeft 0b00000000
#define UpRight 0b01000000
#define DownLeft 0b10000000
#define DownRight 0b11000000

#define MovePieceUL(x) (x << 7)
#define MovePieceUR(x) (x << 9)
#define MovePieceDR(x) (x >> 7)
#define MovePieceDL(x) (x >> 9)

#define rds 0
#define bls 1
#define kngs 2

typedef struct actiongroup {
    action* actions;
    size_t numactions;
} actiongroup_t;

typedef struct actioneval {
    int index;
    int score;
} eval_t;

#define startred 0x000000000055AA55
#define startblue 0xAA55AA0000000000
//#define startkings 0

#define LEFT_EDGE 0x0001000100010001
#define RIGHT_EDGE 0x8000800080008000
#define TOP_EDGE 0xAA00000000000000
#define DOWN_EDGE 0x0000000000000055

#define LEFT_SLAB 0x0303030303030303
#define RIGHT_SLAB 0xC0C0C0C0C0C0C0C0
#define TOP_SLAB 0xFFFF000000000000
#define DOWN_SLAB 0x000000000000FFFF

#define BitFromPos(x) (((long) 1) << x)
#define BitFromRowCol(row, col) ( ((unsigned long)1) << (((row-1)*8)+col-1) )
#define PiecePosFromRowCol(row, col) (((row-1)*8)+col-1)
#define RowFromPiecePos(pos) ((int) pos/8 + 1)
#define ColFromPiecePos(pos) ((int) pos%8 + 1)
#define ColChar(pos) (char) (64 + (pos%8 + 1))

bitboard allboards[3];
short turn;
short stalltimer;

void bitboardToBinary(bitboard board);
void printBoard(bitboard board);
void printPieces(bitboard redps, bitboard blueps, bitboard kings);
void printMoves(bitboard redps, bitboard blueps, bitboard kings);
void printJumps(bitboard redps, bitboard blueps, bitboard kings);

bitboard create_board();

bitboard getRedMovers(bitboard redps, bitboard blueps, bitboard kings);
bitboard getBlueMovers(bitboard redps, bitboard blueps, bitboard kings);
bitboard getRedJumpers(bitboard redps, bitboard blueps, bitboard kings);
bitboard getBlueJumpers(bitboard redps, bitboard blueps, bitboard kings);

bitboard getRedMoves(bitboard redps, bitboard blueps, bitboard kings);
bitboard getBlueMoves(bitboard redps, bitboard blueps, bitboard kings);
bitboard getRedJumps(bitboard redps, bitboard blueps, bitboard kings);
bitboard getBlueJumps(bitboard redps, bitboard blueps, bitboard kings);

actiongroup_t getMoves(bitboard movers, bitboard moves, bitboard kings);
actiongroup_t getJumps(bitboard jumpers, bitboard jumps, bitboard kings);

action selectMove(actiongroup_t acts);
//Prints each move, and what number to input to make move,
//then returns action from user-chosen index

void makeMove(action move);
void makeCapture(action capture);
int promotePiece();
void freeActions(actiongroup_t acts);


////////////////////////////////////////////////////
// AI FUNCTIONS
////////////////////////////////////////////////////

action move_AI(actiongroup_t acts, char colorplayer, char difficulty);
void virtualMove(action move, bitboard aiboards[], char maxplayer);
void virtualCapture(action capture, bitboard aiboards[], char maxplayer);


actiongroup_t getMovesAI(bitboard movers, bitboard moves, bitboard kings, char max);
actiongroup_t getJumpsAI(bitboard jumpers, bitboard jumps, bitboard kings, bitboard aiboards[], char max);

int evaluatePos(bitboard pos, bitboard kings, char maxplayer);
//done for now :-)
int totalEval(bitboard playerpositions[]);

eval_t minmax_abp(unsigned char height, unsigned char depth, int index, int alpha, int beta, char maxplayer, bitboard mypos[], char prevcapt);
//chooses which index is better by recursively playing the game depth-wise

void moveHistory();



//bit from piece and col::::  bit = 8*(row-1) + (col-1) //Correct!
//A is 65 on ascii



/*
|==|==|==|==|==|==|==|==|
|  |57|  |59|  |61|  |63|
|==|==|==|==|==|==|==|==|
|48|  |50|  |52|  |54|  |
|==|==|==|==|==|==|==|==|
|  |41|  |43|  |45|  |47|
|==|==|==|==|==|==|==|==|
|32|  |34|  |36|  |38|  |
|==|==|==|==|==|==|==|==|
|  |25|  |27|  |29|  |31|
|==|==|==|==|==|==|==|==|
|16|  |18|  |20|  |22|  |
|==|==|==|==|==|==|==|==|
|  |09|  |11|  |13|  |15|
|==|==|==|==|==|==|==|==|
|00|  |02|  |04|  |06|  |
|==|==|==|==|==|==|==|==|
 A  B  C  D  E  F  G  H
*/ //Board start: 0xAA55AA000055AA55

