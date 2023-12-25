
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "checkers.h"

//~~~~~~~~~~~~~~~~~~~~!!!!!!!!!!!!!!!!!!!!!!!!!!!!////////////////////
//Written By Tarik Merzkani (5/11/2022)
//~~~~~~~~~~~~~~~~~~~~!!!!!!!!!!!!!!!!!!!!!!!!!!!!////////////////////

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
*/

void bitboardToBinary(bitboard board) {
    for (int j = 56; j >= 0; j-=8) {
        for(int i = 0; i < 8; i++){
            printf("%c", (board & (((long)1) << (i+j))) ? '1' : '0');
        }
        printf("\n");
    }
}

void printBoard(bitboard board) {
    printf("   |===|===|===|===|===|===|===|===|\n");
    char r = 8;
    for (int j = 56; j >= 0; j-=8) {
        printf(" %d ", r--);
        for(int i = 0; i < 8; i++){
            printf("| %c ", (board & (((long)1) << (i+j))) ? 'P' : ' ');
            
        }
        printf("|\n   |===|===|===|===|===|===|===|===|\n");
    }
    printf("     A   B   C   D   E   F   G   H\n");
}

void printPieces(bitboard redps, bitboard blueps, bitboard kings) {
    printf("   |===|===|===|===|===|===|===|===|\n");
    char r = 8;
    for (int j = 56; j >= 0; j-=8) {
        printf(" %d ", r--);
        for(int i = 0; i < 8; i++){
            if ((redps & (((long)1) << (i+j)))){
                printf("| R");
            } else if ((blueps & (((long)1) << (i+j)))) {
                printf("| B");
            } else {
                printf("|  ");
            }
            if ((kings & (((long)1) << (i+j)))) {
                printf("*");
            } else {printf(" ");}
        }
        printf("|\n   |===|===|===|===|===|===|===|===|\n");
    }
    printf("     A   B   C   D   E   F   G   H\n");
}

void printMoves(bitboard redps, bitboard blueps, bitboard kings) {
    printf("   |===|===|===|===|===|===|===|===|\n");
    char r = 8;
    //Odd number turns are Red (for example, turn 1)
    bitboard moves = ((turn&1) ? getRedMoves(redps,blueps,kings) : getBlueMoves(redps,blueps,kings));
    for (int j = 56; j >= 0; j-=8) {
        printf(" %d ", r--);
        for(int i = 0; i < 8; i++){
            if ((moves & (((long)1) << (i+j)))) {
                printf("|[ ]");
            } else {
                if ((redps & (((long)1) << (i+j)))){
                    printf("| R");
                } else if ((blueps & (((long)1) << (i+j)))) {
                    printf("| B");
                } else {
                    printf("|  ");
                }
                if ((kings & (((long)1) << (i+j)))) {
                    printf("*");
                } else {printf(" ");}
            }
        }
        printf("|\n   |===|===|===|===|===|===|===|===|\n");
    }
    printf("     A   B   C   D   E   F   G   H\n");
}
void printJumps(bitboard redps, bitboard blueps, bitboard kings) {
    printf("   |===|===|===|===|===|===|===|===|\n");
    char r = 8;
    //Odd number turns are Red (for example, turn 1)
    bitboard moves = ((turn&1) ? getRedJumps(redps,blueps,kings) : getBlueJumps(redps,blueps,kings));
    for (int j = 56; j >= 0; j-=8) {
        printf(" %d ", r--);
        for(int i = 0; i < 8; i++){
            if ((moves & (((long)1) << (i+j)))) {
                printf("|< >");
            } else {
                if ((redps & (((long)1) << (i+j)))){
                    printf("| R");
                } else if ((blueps & (((long)1) << (i+j)))) {
                    printf("| B");
                } else {
                    printf("|  ");
                }
                if ((kings & (((long)1) << (i+j)))) {
                    printf("*");
                } else {printf(" ");}
            }
        }
        printf("|\n   |===|===|===|===|===|===|===|===|\n");
    }
    printf("     A   B   C   D   E   F   G   H\n");
}

bitboard create_board() {
    allboards[0] = startred, allboards[1] = startblue, allboards[2] = 0;
    return (startred | startblue);
}

bitboard getRedMovers(bitboard redps, bitboard blueps, bitboard kings) {
    bitboard unoccupied = ~(redps | blueps); // Not Occupied
    bitboard redK = redps & kings;         // Kings
    //Left 7 is ul, Left 9 is ur
    bitboard moves = ((redps & (~LEFT_EDGE) & (~TOP_EDGE))) & (unoccupied >> 7);    //upleft
    moves |= ((redps & (~RIGHT_EDGE) & (~TOP_EDGE))) & (unoccupied >> 9);            //upright
    if ( redK ) {
        moves |= ((redK & (~LEFT_EDGE) & (~DOWN_EDGE))) & (unoccupied << 7);        //downleft
        moves |= ((redK & (~RIGHT_EDGE) & (~DOWN_EDGE))) & (unoccupied << 9);        //downright
    }
    return moves;
}
bitboard getBlueMovers(bitboard redps, bitboard blueps, bitboard kings) {
    bitboard unoccupied = ~(redps | blueps); // Not Occupied
    bitboard blueK = blueps & kings;         // Kings
    //Left 7 is ul, Left 9 is ur
    bitboard moves = ((blueps & (~LEFT_EDGE) & (~DOWN_EDGE))) & (unoccupied << 9);      //downleft
    moves |= ((blueps & (~RIGHT_EDGE) & (~DOWN_EDGE))) & (unoccupied << 7);             //downright
    if ( blueK ) {
        moves |= ((blueK & (~LEFT_EDGE) & (~TOP_EDGE))) & (unoccupied >> 7);           //upleft
        moves |= ((blueK & (~RIGHT_EDGE) & (~TOP_EDGE))) & (unoccupied >> 9);          //upright
    }
    return moves;
}
bitboard getRedJumpers(bitboard redps, bitboard blueps, bitboard kings) {
    bitboard unoccupied = ~(redps | blueps); // Not Occupied
    bitboard redK = redps & kings;         // Kings
    //Left 7 is ul, Left 9 is ur
    bitboard moves = ((redps & (~LEFT_SLAB) & (~TOP_SLAB))) & (unoccupied >> 14);    //upleft
    moves |= ((redps & (~RIGHT_SLAB) & (~TOP_SLAB))) & (unoccupied >> 18);            //upright
    //check if something in between
    moves &= (((blueps & (~RIGHT_EDGE) & (~DOWN_EDGE)) >> 7) & redps) | (((blueps & (~LEFT_EDGE) & (~DOWN_EDGE)) >> 9) & redps);
    bitboard kmoves = 0;
    if ( redK ) {
        kmoves |= ((redK & (~LEFT_SLAB) & (~DOWN_SLAB))) & (unoccupied << 14);        //downleft
        kmoves |= ((redK & (~RIGHT_SLAB) & (~DOWN_SLAB))) & (unoccupied << 18);        //downright
        kmoves &= (((blueps & (~LEFT_EDGE) & (~TOP_EDGE)) << 7) & redK) | (((blueps & (~RIGHT_EDGE) & (~TOP_EDGE)) << 9) & redK);
    }
    moves |= kmoves;
    return moves;
}

bitboard getBlueJumpers(bitboard redps, bitboard blueps, bitboard kings){
    bitboard unoccupied = ~(redps | blueps); // Not Occupied
    bitboard blueK = blueps & kings;         // Kings
    //Left 7 is ul, Left 9 is ur
    bitboard moves = (((blueps & (~LEFT_SLAB) & (~DOWN_SLAB))) & (unoccupied << 18));      //downleft
    moves |= (((blueps & (~RIGHT_SLAB) & (~DOWN_SLAB))) & (unoccupied << 14));             //downright
    moves &= ((((redps & (~LEFT_EDGE) & (~TOP_EDGE)) << 7) & blueps) | (((redps & (~RIGHT_EDGE) & (~TOP_EDGE)) << 9) & blueps));
    bitboard kmoves = 0;
    if ( blueK ) {
        kmoves |= (((blueK & (~LEFT_SLAB) & (~TOP_SLAB))) & (unoccupied >> 14));           //upleft
        kmoves |= (((blueK & (~RIGHT_SLAB) & (~TOP_SLAB))) & (unoccupied >> 18));          //upright
        kmoves &= ((((redps & (~RIGHT_EDGE) & (~DOWN_EDGE)) >> 7) & blueK) | (((redps & (~LEFT_EDGE) & (~DOWN_EDGE)) >> 9) & blueK));
    }
    moves |= kmoves;
    return moves;
}

bitboard getRedMoves(bitboard redps, bitboard blueps, bitboard kings) {
    bitboard unoccupied = ~(redps | blueps); // Not Occupied
    bitboard redK = redps & kings;         // Kings
    //Left 7 is ul, Left 9 is ur
    bitboard moves = ((redps & (~LEFT_EDGE) & (~TOP_EDGE)) << 7) & (unoccupied);    //upleft
    moves |= ((redps & (~RIGHT_EDGE) & (~TOP_EDGE)) << 9) & (unoccupied);            //upright
    if ( redK ) {
        moves |= ((redK & (~LEFT_EDGE) & (~DOWN_EDGE)) >> 9) & (unoccupied);        //downleft
        moves |= ((redK & (~RIGHT_EDGE) & (~DOWN_EDGE)) >> 7) & (unoccupied);        //downright
    }
    return moves;
}


bitboard getBlueMoves(bitboard redps, bitboard blueps, bitboard kings) {
    bitboard unoccupied = ~(redps | blueps); // Not Occupied
    bitboard blueK = blueps & kings;         // Kings
    //Left 7 is ul, Left 9 is ur
    bitboard moves = ((blueps & (~LEFT_EDGE) & (~DOWN_EDGE)) >> 9) & (unoccupied);      //downleft
    moves |= ((blueps & (~RIGHT_EDGE) & (~DOWN_EDGE)) >> 7) & (unoccupied);             //downright
    if ( blueK ) {
        moves |= ((blueK & (~LEFT_EDGE) & (~TOP_EDGE)) << 7) & (unoccupied);           //upleft
        moves |= ((blueK & (~RIGHT_EDGE) & (~TOP_EDGE)) << 9) & (unoccupied);          //upright
    }
    return moves;
}

bitboard getRedJumps(bitboard redps, bitboard blueps, bitboard kings) {
    bitboard unoccupied = ~(redps | blueps); // Not Occupied
    bitboard redK = redps & kings;         // Kings
    //Left 7 is ul, Left 9 is ur
    bitboard moves = ((redps & (~LEFT_SLAB) & (~TOP_SLAB)) << 14) & (unoccupied);    //upleft
    moves |= ((redps & (~RIGHT_SLAB) & (~TOP_SLAB)) << 18) & (unoccupied);            //upright
    //check if something in between
    bitboard cancellations = ((((blueps & (~LEFT_EDGE) & (~TOP_EDGE)) << 7) & ((redps & (~LEFT_SLAB) & (~TOP_SLAB)) << 14)));
    cancellations |= ((((blueps & (~RIGHT_EDGE) & (~TOP_EDGE)) << 9) & ((redps & (~RIGHT_SLAB) & (~TOP_SLAB)) << 18)));
    moves &= cancellations;
    bitboard kmoves = 0;
    if ( redK ) {
        kmoves |= ((redK & (~LEFT_SLAB) & (~DOWN_SLAB)) >> 18) & (unoccupied);        //downleft
        //printBoard(kmoves);
        kmoves |= ((redK & (~RIGHT_SLAB) & (~DOWN_SLAB)) >> 14) & (unoccupied);        //downright
        cancellations = (((blueps & (~RIGHT_EDGE) & (~DOWN_EDGE)) >> 7) & ((redK & (~RIGHT_SLAB) & (~DOWN_SLAB)) >> 14));
        cancellations |= (((blueps & (~LEFT_EDGE) & (~DOWN_EDGE)) >> 9) & ((redK & (~LEFT_SLAB) & (~DOWN_SLAB)) >> 18));
        kmoves &= cancellations;
    }
    moves |= kmoves;
    return moves;
}

bitboard getBlueJumps(bitboard redps, bitboard blueps, bitboard kings) {
    bitboard unoccupied = ~(redps | blueps); // Not Occupied
    bitboard blueK = blueps & kings;         // Kings
    //Left 7 is ul, Left 9 is ur
    bitboard moves = ((blueps & (~LEFT_SLAB) & (~DOWN_SLAB)) >> 18) & (unoccupied);      //downleft
    moves |= ((blueps & (~RIGHT_SLAB) & (~DOWN_SLAB)) >> 14) & (unoccupied);             //downright
    bitboard cancellations = (((redps & (~RIGHT_EDGE) & (~DOWN_EDGE)) >> 7) & ((blueps & (~RIGHT_SLAB) & (~DOWN_SLAB)) >> 14));
    cancellations |= (((redps & (~LEFT_EDGE) & (~DOWN_EDGE)) >> 9) & ((blueps & (~LEFT_SLAB) & (~DOWN_SLAB)) >> 18));
    moves &= cancellations;
    bitboard kmoves = 0;
    if ( blueK ) {
        kmoves |= ((blueK & (~LEFT_SLAB) & (~TOP_SLAB)) << 14) & (unoccupied);           //upleft
        kmoves |= ((blueK & (~RIGHT_SLAB) & (~TOP_SLAB)) << 18) & (unoccupied);          //upright
        cancellations = (((redps & (~LEFT_EDGE) & (~TOP_EDGE)) << 7) & ((blueK & (~LEFT_SLAB) & (~TOP_SLAB)) << 14));
        cancellations |= (((redps & (~RIGHT_EDGE) & (~TOP_EDGE)) << 9) & ((blueK & (~RIGHT_SLAB) & (~TOP_SLAB)) << 18));
        kmoves &= cancellations;
    }
    moves |= kmoves;
    return moves;
}

actiongroup_t getMoves(bitboard movers, bitboard moves, bitboard kings){
    actiongroup_t mygroup;
    if (moves == 0){
        mygroup.actions = 0;
        mygroup.numactions = 0;
        return mygroup;
    }
    int movecount = 0;
    action* results = malloc(24);
    action act = 0;
    for (int i = 8; i > 0; i--){    //Iterate through each row and see each piece in that row
        if ( movers & (((long)0xFF) << ((i-1)*8))){    //check if row has legal movable pieces
            for (int j = 8; j > 0; j--){
                unsigned char piecepos = PiecePosFromRowCol(i,j);
                unsigned long bitpos = ((unsigned long)1) << piecepos;
                if ( movers & bitpos ) {    //if there's a movable piece there
                    if (turn&1) {   //if red's turn
                        if (bitpos << 7 & moves) {  //checks if can move UpLeft
                            act = UpLeft | (0x3F & piecepos);
                            results[movecount++] = act;
                        } 
                        if (bitpos << 9 & moves ) {    //UpRight      
                            act = UpRight | (0x3F & piecepos);
                            results[movecount++] = act;
                        }
                        if (bitpos & kings){ //if king piece (for DownLeft,DownRight)
                            if ( bitpos >> 7 & moves ) {        //DownRight
                                act = DownRight | (0x3F & piecepos);
                                results[movecount++] = act;
                            } 
                            if ( bitpos >> 9 & moves ) {        //DownLeft      
                                act = DownLeft | (0x3F & piecepos);
                                results[movecount++] = act;
                            }
                        } 
                    } else {        //if blue's turn
                        if ( bitpos >> 7 & moves ) {        //DownRight
                            act = DownRight | (0x3F & piecepos);
                            results[movecount++] = act;
                        } 
                        if ( bitpos >> 9 & moves ) {        //DownLeft     
                            act = DownLeft | (0x3F & piecepos);
                            results[movecount++] = act; 
                        }
                        if (bitpos & kings) {
                            if (bitpos << 7 & moves) {  //checks if can move UpLeft
                                act = UpLeft | (0x3F & piecepos);
                                results[movecount++] = act;
                            } 
                            if (bitpos << 9 & moves ) {    //UpRight        
                                act = UpRight | (0x3F & piecepos);
                                results[movecount++] = act;
                            }
                        }
                    }
                }
            }
        }
    }
    
    mygroup.numactions = (size_t) movecount;
    mygroup.actions = results;
    return mygroup;
    //Remember to free actions!!!
    //Returns 1 if no moves
}



actiongroup_t getJumps(bitboard jumpers, bitboard jumps, bitboard kings) {
    actiongroup_t mygroup;
    if (jumps == 0){
        mygroup.actions = 0;
        mygroup.numactions = 0;
        return mygroup;
    }
    int movecount = 0;
    action* results = malloc(16);
    action act = 0;
    for (int i = 8; i > 0; i--){    //Iterate through each row and see each piece in that row
        if ( jumpers & (((long)0xFF) << ((i-1)*8))){    //check if row has legal movable pieces
            for (int j = 8; j > 0; j--){
                unsigned char piecepos = PiecePosFromRowCol(i,j);
                unsigned long bitpos = ((unsigned long)1) << piecepos;
                if ( jumpers & bitpos ) {    //if there's a movable piece there
                    if (turn&1) {   //if red's turn
                        if ((bitpos << 14 & jumps) & (allboards[1] << 7)) {  //checks if can move UpLeft
                            act = UpLeft | (0x3F & piecepos);
                            results[movecount++] = act;
                        } 
                        if ((bitpos << 18 & jumps) & (allboards[1] << 9)) {    //UpRight      
                            act = UpRight | (0x3F & piecepos);
                            results[movecount++] = act;
                        }
                        if (bitpos & kings){ //if king piece (for DownLeft,DownRight)
                            if ((bitpos >> 14 & jumps) & (allboards[1] >> 7)) {        //DownRight
                                act = DownRight | (0x3F & piecepos);
                                results[movecount++] = act;
                            } 
                            if ((bitpos >> 18 & jumps) & (allboards[1] >> 9)) {        //DownLeft      
                                act = DownLeft | (0x3F & piecepos);
                                results[movecount++] = act;
                            }
                        } 
                    } else {        //if blue's turn
                        if ((bitpos >> 14 & jumps) & (allboards[0] >> 7)) {        //DownRight
                            act = DownRight | (0x3F & piecepos);
                            results[movecount++] = act;
                        } 
                        if ((bitpos >> 18 & jumps) & (allboards[0] >> 9)) {        //DownLeft     
                            act = DownLeft | (0x3F & piecepos);
                            results[movecount++] = act; 
                        }
                        if (bitpos & kings) {
                            if ((bitpos << 14 & jumps) & (allboards[0] << 7)) {  //checks if can move UpLeft
                                act = UpLeft | (0x3F & piecepos);
                                results[movecount++] = act;
                            } 
                            if ((bitpos << 18 & jumps) & (allboards[0] << 9)) {    //UpRight        
                                act = UpRight | (0x3F & piecepos);
                                results[movecount++] = act;
                            }
                        }
                    }
                }
            }
        }
    }
    
    mygroup.numactions = (size_t) movecount;
    mygroup.actions = results;
    return mygroup;
    //Remember to free actions!!!
    //Returns size of 0 if no moves
}

action selectMove(actiongroup_t acts) {
    char* directions[4] = {":🡴", ":🡵", ":🡷", ":🡶"};
    printf("Select which move you would want to do:\n");
    short length = acts.numactions;
    char pos = 0, dir = 0;
    for(int i = length-1; i >= 0; i--){
        pos = (acts.actions[i]& 0x3F);
        dir = ((acts.actions[i]& 0xC0) >> 6);
        if (i == 0) { 
            printf("%c%d%s\n", ColChar(pos), RowFromPiecePos(pos),
            directions[(int) dir]);
        } else {
            printf("%c%d%s , ", ColChar(pos), RowFromPiecePos(pos),
            directions[(int) dir]);  
        }
    }
    int i = 0;
    for(i = length-1; i >= 0; i--){
        if (i == 0){
            printf("%4d\n\n", length-1-i);     
        } else{
            printf("%4d , ", length-1-i); 
        }
    }
    char index[128];
    printf("Please enter a number: ");
    //make error checking for this one
    i = scanf("%s", index);
    i = atoi(index);
    while (i < 0  || i >= length){
        printf("\nEnter a number of the ones given: ");
        i = scanf("%s", index);
        i = atoi(index);
    }
    int fr = length-1-i;
    pos = (acts.actions[fr]& 0x3F);
    dir = ((acts.actions[fr]& 0xC0) >> 6);
    printf("\nMove chosen: %d. %c%d%s \n", i, ColChar(pos), RowFromPiecePos(pos),
            directions[(int) dir]);
    return acts.actions[fr];
}
//Prints each move, and what number to input to make move,
//then returns action from user-chosen index


void makeMove(action move) {
    bitboard piecepos = BitFromPos((0x3F & move));
    char posdir = (move >> 6);
    int player = (turn+1)&1;
    switch(posdir){
        case 0: //UpLeft
            allboards[player] = (allboards[player] & (~piecepos)) | (MovePieceUL(piecepos));
            if ((allboards[2] & piecepos)){
                allboards[2] = (allboards[2] & (~piecepos)) | (MovePieceUL(piecepos));
            }
            break;
        case 1: //UpRight
            allboards[player] = (allboards[player] & (~piecepos)) | (MovePieceUR(piecepos));
            if ((allboards[2] & piecepos)){
                allboards[2] = (allboards[2] & (~piecepos)) | (MovePieceUR(piecepos));
            }
            break;
        case 2: //DownLeft
            allboards[player] = (allboards[player] & (~piecepos)) | (MovePieceDL(piecepos));
            if ((allboards[2] & piecepos)){
                allboards[2] = (allboards[2] & (~piecepos)) | (MovePieceDL(piecepos));
            }
            break;
        case 3: //DownRight
            allboards[player] = (allboards[player] & (~piecepos)) | (MovePieceDR(piecepos));
            if ((allboards[2] & piecepos)){
                allboards[2] = (allboards[2] & (~piecepos)) | (MovePieceDR(piecepos));
            }
            break;
    }
}

void makeCapture(action capture) {
    bitboard piecepos = BitFromPos((0x3F & capture));
    char posdir = (capture >> 6);
    int player = (turn+1)&1;
    switch(posdir){
        case 0: //UpLeft
            allboards[player] = (allboards[player] & (~piecepos)) | (MovePieceUL(MovePieceUL(piecepos)));
            allboards[(!(player))] &= (~(MovePieceUL(piecepos)));
            allboards[kngs] &= (~(MovePieceUL(piecepos)));
            if ((allboards[2] & piecepos)){
                allboards[2] = (allboards[2] & (~piecepos)) | (MovePieceUL(MovePieceUL(piecepos)));
            }
            break;
        case 1: //UpRight
            allboards[player] = (allboards[player] & (~piecepos)) | (MovePieceUR(MovePieceUR(piecepos)));
            allboards[(!(player))] &= (~(MovePieceUR(piecepos)));
            allboards[kngs] &= (~(MovePieceUR(piecepos)));
            if ((allboards[2] & piecepos)){
                allboards[2] = (allboards[2] & (~piecepos)) | (MovePieceUR(MovePieceUR(piecepos)));
            }
            break;
        case 2: //DownLeft
            allboards[player] = (allboards[player] & (~piecepos)) | (MovePieceDL(MovePieceDL(piecepos)));
            allboards[(!(player))] &= (~(MovePieceDL(piecepos)));
            allboards[kngs] &= (~(MovePieceDL(piecepos)));
            if ((allboards[2] & piecepos)){
                allboards[2] = (allboards[2] & (~piecepos)) | (MovePieceDL(MovePieceDL(piecepos)));
            }
            break;
        case 3: //DownRight
            allboards[player] = (allboards[player] & (~piecepos)) | (MovePieceDR(MovePieceDR(piecepos)));
            allboards[(!(player))] &= (~(MovePieceDR(piecepos)));
            allboards[kngs] &= (~(MovePieceDR(piecepos)));
            if ((allboards[2] & piecepos)){
                allboards[2] = (allboards[2] & (~piecepos)) | (MovePieceDR(MovePieceDR(piecepos)));
            }
            break;
    }
}

int promotePiece(){
    int player = (turn+1)&1;
    bitboard promotables = 0;
    if (player==0){ //Reds
        promotables = 0xFF00000000000000 & allboards[player];
    } else {    //Blues
        promotables = 0x00000000000000FF & allboards[player];
    }
    bitboard oldkngs = allboards[kngs];
    allboards[kngs] |= promotables;
    if(oldkngs != allboards[kngs]) {
        return 0;               //Promoted Piece!
    } else {
        return 1;               //No promotes
    }
}

void freeActions(actiongroup_t acts){
    free(acts.actions);
}

// ////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////////   AI STUFF   ///////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////

void printOOB(){
    printf("   |===|===|===|===|===|===|===|===|\n");
    char r = 8;
    for (int i = 8; i > 0; i--){    //Iterate through each row and see each piece in that row
        printf(" %d ", r--);
        for (int j = 8; j > 0; j--){
            if ((allboards[rds] & (((long)1) << (i+j)))){
                if ((j & 1) == (i & 1)) {printf("|e");} else {printf("| ");}
                printf("R");
            } 
            else if ((allboards[bls] & (((long)1) << (i+j)))) {
                if ((j & 1) == (i & 1)) {printf("|e");} else {printf("| ");}
                printf("B");
            } else {
                if ((j & 1) == (i & 1)) {printf("|//");} else {printf("|  ");}
            }
            if ((allboards[kngs] & (((long)1) << (i+j)))) {
                printf("*");
            } else {printf(" ");}
        }   
        printf("|\n   |===|===|===|===|===|===|===|===|\n");
    }
    printf("     A   B   C   D   E   F   G   H\n");
    printMoves(allboards[rds], allboards[bls], allboards[kngs]);
}

action move_AI(actiongroup_t acts, char colorplayer, char difficulty) {        //TO DO YET
    //char* directions[4] = {":0", ":1", ":2", ":3"};
    //0 UL, 1 UR, 2 DL, 3 DR
    short length = acts.numactions;
    //unsigned char pos = 0, dir = 0;
    // for(int i = length-1; i >= 0; i--){
    //     pos = (acts.actions[i]& 0x3F);
    //     dir = ((acts.actions[i]& 0xC0) >> 6);
    //     if (i == 0) { 
    //         printf("%c%d%s\n", ColChar(pos), RowFromPiecePos(pos),
    //         directions[(int) dir]);
    //     } else {
    //         printf("%c%d%s, ", ColChar(pos), RowFromPiecePos(pos),
    //         directions[(int) dir]);  
    //     }
    // }
    int fr = -1;
    if (difficulty > 0){
        while (fr < 0  || fr >= length) {
            eval_t movechoice = minmax_abp(difficulty, 0, 0, INT_MIN, INT_MAX, colorplayer, allboards, 0);
            fr = movechoice.index;
            //printf("MINMAX INDEX: %d AND SCORE %d \n", movechoice.index, movechoice.score);
        }
    } else {
        while (fr < 0  || fr >= length) {
            fr = rand() % length;
        }
    }
    //pos = (acts.actions[fr]& 0x3F);
    //dir = ((acts.actions[fr]& 0xC0) >> 6);
    // printf("INDEX: %d. %c%d%s\n", fr, ColChar(pos), RowFromPiecePos(pos),
    //         directions[(int) dir]);
    // if((RowFromPiecePos(pos) & 1) != (ColFromPiecePos(pos) & 1)) {
    //     printf("ERROR BAD MOVE PRINTED/////////////////////////////////////////////////////\n");
    //     printOOB();
    // }

    return acts.actions[fr];
}

void virtualMove(action move, bitboard aiboards[], char maxplayer) {    //DONEEEEEEE
    //THESE DO PROMOTION THEMSELVES
    //with move, changes boards in aiboards, corresponding if maxplayer or not.
    bitboard piecepos = BitFromPos((0x3F & move));
    char posdir = (move >> 6);
    int player = !(maxplayer);     //reds are maxplayer 1, but board 0. Blues are maxplayer 0, board 1
    switch(posdir){   
        case 0: //UpLeft
            aiboards[player] = (aiboards[player] & (~piecepos)) | (MovePieceUL(piecepos));
            if ((aiboards[2] & piecepos)){
                aiboards[2] = (aiboards[2] & (~piecepos)) | (MovePieceUL(piecepos));
            }
            break;
        case 1: //UpRight
            aiboards[player] = (aiboards[player] & (~piecepos)) | (MovePieceUR(piecepos));
            if ((aiboards[2] & piecepos)){
                aiboards[2] = (aiboards[2] & (~piecepos)) | (MovePieceUR(piecepos));
            }
            break;
        case 2: //DownLeft
            aiboards[player] = (aiboards[player] & (~piecepos)) | (MovePieceDL(piecepos));
            if ((aiboards[2] & piecepos)){
                aiboards[2] = (aiboards[2] & (~piecepos)) | (MovePieceDL(piecepos));
            }
            break;
        case 3: //DownRight
            aiboards[player] = (aiboards[player] & (~piecepos)) | (MovePieceDR(piecepos));
            if ((aiboards[2] & piecepos)){
                aiboards[2] = (aiboards[2] & (~piecepos)) | (MovePieceDR(piecepos));
            }
            break;
    }
    //after move, promote piece
    bitboard promotables = 0;
    if (maxplayer){ //Reds
        promotables = 0xFF00000000000000 & aiboards[player];
    } else {    //Blues
        promotables = 0x00000000000000FF & aiboards[player];
    }
    aiboards[kngs] |= promotables;
}

void virtualCapture(action capture, bitboard aiboards[], char maxplayer) {      //DONNEEEEEEEE
    //THESE DO PROMOTION THEMSELVES
    bitboard piecepos = BitFromPos((0x3F & capture));
    char posdir = (capture >> 6);
    int player =  !(maxplayer);
    switch(posdir){
        case 0: //UpLeft
            aiboards[player] = (aiboards[player] & (~piecepos)) | (MovePieceUL(MovePieceUL(piecepos)));
            aiboards[(!(player))] &= (~(MovePieceUL(piecepos)));
            aiboards[kngs] &= (~(MovePieceUL(piecepos)));
            if ((aiboards[2] & piecepos)){
                aiboards[2] = (aiboards[2] & (~piecepos)) | (MovePieceUL(MovePieceUL(piecepos)));
            }
            break;
        case 1: //UpRight
            aiboards[player] = (aiboards[player] & (~piecepos)) | (MovePieceUR(MovePieceUR(piecepos)));
            aiboards[(!(player))] &= (~(MovePieceUR(piecepos)));
            aiboards[kngs] &= (~(MovePieceUR(piecepos)));
            if ((aiboards[2] & piecepos)){
                aiboards[2] = (aiboards[2] & (~piecepos)) | (MovePieceUR(MovePieceUR(piecepos)));
            }
            break;
        case 2: //DownLeft
            aiboards[player] = (aiboards[player] & (~piecepos)) | (MovePieceDL(MovePieceDL(piecepos)));
            aiboards[(!(player))] &= (~(MovePieceDL(piecepos)));
            aiboards[kngs] &= (~(MovePieceDL(piecepos)));
            if ((aiboards[2] & piecepos)){
                aiboards[2] = (aiboards[2] & (~piecepos)) | (MovePieceDL(MovePieceDL(piecepos)));
            }
            break;
        case 3: //DownRight
            aiboards[player] = (aiboards[player] & (~piecepos)) | (MovePieceDR(MovePieceDR(piecepos)));
            aiboards[(!(player))] &= (~(MovePieceDR(piecepos)));
            aiboards[kngs] &= (~(MovePieceDR(piecepos)));
            if ((aiboards[2] & piecepos)){
                aiboards[2] = (aiboards[2] & (~piecepos)) | (MovePieceDR(MovePieceDR(piecepos)));
            }
            break;
    }
    //after move, promote piece
    bitboard promotables = 0;
    if (maxplayer){ //Reds
        promotables = 0xFF00000000000000 & aiboards[player];
    } else {    //Blues
        promotables = 0x00000000000000FF & aiboards[player];
    }
    aiboards[kngs] |= promotables;
}

actiongroup_t getMovesAI(bitboard movers, bitboard moves, bitboard kings, char max){
    actiongroup_t mygroup;
    if (moves == 0){
        mygroup.actions = 0;
        mygroup.numactions = 0;
        return mygroup;
    }
    int movecount = 0;
    action* results = malloc(32);
    action act = 0;
    for (int i = 8; i > 0; i--){    //Iterate through each row and see each piece in that row
        if ( movers & (((long)0xFF) << ((i-1)*8))){    //check if row has legal movable pieces
            for (int j = 8; j > 0; j--){
                unsigned char piecepos = PiecePosFromRowCol(i,j);
                unsigned long bitpos = ((unsigned long)1) << piecepos;
                if ( movers & bitpos ) {    //if there's a movable piece there
                    /////
                    if (max) {   //if red's turn OR MAX
                    /////
                        if (bitpos << 7 & moves) {  //checks if can move UpLeft
                            act = UpLeft | (0x3F & piecepos);
                            results[movecount++] = act;
                        } 
                        if (bitpos << 9 & moves ) {    //UpRight      
                            act = UpRight | (0x3F & piecepos);
                            results[movecount++] = act;
                        }
                        if (bitpos & kings){ //if king piece (for DownLeft,DownRight)
                            if ( bitpos >> 7 & moves ) {        //DownRight
                                act = DownRight | (0x3F & piecepos);
                                results[movecount++] = act;
                            } 
                            if ( bitpos >> 9 & moves ) {        //DownLeft      
                                act = DownLeft | (0x3F & piecepos);
                                results[movecount++] = act;
                            }
                        } 
                    } else {        //if blue's turn
                        if ( bitpos >> 7 & moves ) {        //DownRight
                            act = DownRight | (0x3F & piecepos);
                            results[movecount++] = act;
                        } 
                        if ( bitpos >> 9 & moves ) {        //DownLeft     
                            act = DownLeft | (0x3F & piecepos);
                            results[movecount++] = act; 
                        }
                        if (bitpos & kings) {
                            if (bitpos << 7 & moves) {  //checks if can move UpLeft
                                act = UpLeft | (0x3F & piecepos);
                                results[movecount++] = act;
                            } 
                            if (bitpos << 9 & moves ) {    //UpRight        
                                act = UpRight | (0x3F & piecepos);
                                results[movecount++] = act;
                            }
                        }
                    }
                }
            }
        }
    }
    
    mygroup.numactions = (size_t) movecount;
    mygroup.actions = results;
    return mygroup;
    //Remember to free actions!!!
    //Returns 1 if no moves
}

actiongroup_t getJumpsAI(bitboard jumpers, bitboard jumps, bitboard kings, bitboard aiboards[], char max) {
    actiongroup_t mygroup;
    if (jumps == 0){
        mygroup.actions = 0;
        mygroup.numactions = 0;
        return mygroup;
    }
    int movecount = 0;
    action* results = malloc(24);
    action act = 0;
    unsigned char piecepos = 0;
    unsigned long bitpos = 0;
    for (int i = 8; i > 0; i--){    //Iterate through each row and see each piece in that row
        if ( jumpers & (((long)0xFF) << ((i-1)*8))){    //check if row has legal movable pieces
            for (int j = 8; j > 0; j--){
                piecepos = PiecePosFromRowCol(i,j);
                bitpos = ((unsigned long)1) << piecepos;
                if ( jumpers & bitpos ) {    //if there's a movable piece there
                    //////
                    if (max) {   //if red's turn OR MAX
                    //////
                        if ((bitpos << 14 & jumps) & (aiboards[1] << 7)) {  //checks if can move UpLeft
                            act = UpLeft | (0x3F & piecepos);
                            results[movecount++] = act;
                        } 
                        if ((bitpos << 18 & jumps) & (aiboards[1] << 9)) {    //UpRight      
                            act = UpRight | (0x3F & piecepos);
                            results[movecount++] = act;
                        }
                        if (bitpos & kings){ //if king piece (for DownLeft,DownRight)
                            if ((bitpos >> 14 & jumps) & (aiboards[1] >> 7)) {        //DownRight
                                act = DownRight | (0x3F & piecepos);
                                results[movecount++] = act;
                            } 
                            if ((bitpos >> 18 & jumps) & (aiboards[1] >> 9)) {        //DownLeft      
                                act = DownLeft | (0x3F & piecepos);
                                results[movecount++] = act;
                            }
                        } 
                    } else {        //if blue's turn
                        if ((bitpos >> 14 & jumps) & (aiboards[0] >> 7)) {        //DownRight
                            act = DownRight | (0x3F & piecepos);
                            results[movecount++] = act;
                        } 
                        if ((bitpos >> 18 & jumps) & (aiboards[0] >> 9)) {        //DownLeft     
                            act = DownLeft | (0x3F & piecepos);
                            results[movecount++] = act; 
                        }
                        if (bitpos & kings) {
                            if ((bitpos << 14 & jumps) & (aiboards[0] << 7)) {  //checks if can move UpLeft
                                act = UpLeft | (0x3F & piecepos);
                                results[movecount++] = act;
                            } 
                            if ((bitpos << 18 & jumps) & (aiboards[0] << 9)) {    //UpRight        
                                act = UpRight | (0x3F & piecepos);
                                results[movecount++] = act;
                            }
                        }
                    }
                }
            }
        }
    }
    
    mygroup.numactions = (size_t) movecount;
    mygroup.actions = results;
    return mygroup;
    //Remember to free actions!!!
    //Returns size of 0 if no moves
}

int evaluatePos(bitboard playerpos, bitboard kings, char maxplayer) {       //NOT YET LMAO??????
    //Iterating goes backwards, for each row for each col, 63 -> 0
    int score = 0;
    for (int i = 8; i > 0; i--){    //Iterate through each row and see each piece in that row
        if ( playerpos & (((long)0xFF) << ((i-1)*8))){    //check if row has legal movable pieces
            for (int j = 8; j > 0; j--){
                unsigned char piecepos = PiecePosFromRowCol(i,j);
                unsigned long bitpos = ((unsigned long)1) << piecepos;  //& to confirm piece
                char addpts = 10;
                if (playerpos&bitpos) {     //if there is a piece in this square
                    //positional modifiers
                    if ( ((i >= 3) && (i<= 6)) && ((j >= 3) && (j<= 6))) {  //if center piece
                        addpts += 2;
                    } else if (j == 1 || j == 8) { //if edge piece
                        addpts += 2;
                    }
                    //identity modifier
                    if (bitpos & kings) { //if king piece
                        addpts += 10;
                    } else {
                        //first proximity to promoting modifiers
                        //different for red and blue
                        if (maxplayer){ //if red
                            if (i > 3) {            //if past 3rd row
                                addpts += 1;
                                if (i > 5){         //if past 5th row
                                    addpts += 1;
                                }
                            }
                        } else {
                            if (i < 6) {            //if past 3rd row
                                addpts += 1;
                                if (i < 4){         //if past 5th row
                                    addpts += 1;
                                }
                            }
                        }
                    }
                    score += addpts;
                }
            }
        }
    }
    if (!(maxplayer)) {   //if minplayer or blue, return negative
        score = score *-1;
    }
    return score;
}

int totalEval(bitboard playerpositions[]) {
    int result = evaluatePos(playerpositions[rds], playerpositions[kngs], 1) + evaluatePos(playerpositions[bls], playerpositions[kngs], 0);
    return result;
}

eval_t minmax_abp(unsigned char height, unsigned char depth, int index, int alpha, int beta, char maxplayer, bitboard mypos[], char prevcapt) {
    //write documentation please, including purpose, input parameters, and return value
    //index is index of the previous move done
    eval_t bestchoice;
    //bitboard *virtualresults = malloc(3 * sizeof(bitboard));
    bitboard virtualresults[3];
    bitboard movables, moves;
    actiongroup_t actionpl;
    // printf("AT DEPTH: %d AT INDEX: %d\n", (char) depth, index);
    // printPieces(mypos[rds], mypos[bls], mypos[kngs]);
    if (depth == height) {
        bestchoice.index = index;
        bestchoice.score = totalEval(mypos);
        //free(virtualresults);
        return bestchoice;
    }
    if (maxplayer) {        //IF RED OKAYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
        int bestValue = INT_MIN;            //WORST VALUE FOR MAX/RED
        bestchoice.index = 0;               //DEFAULT MOVE IF ANY
        bestchoice.score = bestValue;       //WORST
        //get ALLMOVES
        int numactions = 0;
        moves = getRedJumps(mypos[rds], mypos[bls], mypos[kngs]);
        if (moves) {    //if there are captures!
            movables = getRedJumpers(mypos[rds], mypos[bls], mypos[kngs]);
            actionpl = getJumpsAI(movables, moves, mypos[kngs], mypos, maxplayer);     //generate every capture
            numactions = actionpl.numactions;
            for(int i = 0; i < numactions; i++){
                //do every capture
                virtualresults[0] = mypos[rds]; 
                virtualresults[1] = mypos[bls];
                virtualresults[2] = mypos[kngs];
                //make capture to change virtual results
                virtualCapture(actionpl.actions[i], virtualresults, maxplayer);
                //at this point, we have a resultant board (VIRTUALRESULTS WILL HAVE CHANGED)
                //call minmax again to go further with the other capture
                eval_t movescore = minmax_abp(height, depth+1, i, alpha, beta, maxplayer, virtualresults, 1);
                if (movescore.score > bestValue) {
                    bestValue = movescore.score;
                    bestchoice = movescore;
                    bestchoice.index = i;
                }
                if (bestValue > alpha)
                    alpha = bestValue;
                if (beta <= alpha)
                    break;
            }
            freeActions(actionpl);
            //this has scowered all best moves, selecting the best move for a certain node
            //make capture then see if there is another capture
        } else if (prevcapt == 0) {    //if no captures
            moves = getRedMoves(mypos[rds], mypos[bls], mypos[kngs]);
            if (moves == 0) { //No moves and no captures available
                //check for this so tree ends
                //return 0;
                bestchoice.index = index;           //index that brought about such
                bestchoice.score = INT_MIN;         //Red Losing is -inf score (best for blue)
                //free(virtualresults);
                return bestchoice;
            }
            movables = getRedMovers(mypos[rds], mypos[bls], mypos[kngs]);
            actionpl = getMovesAI(movables, moves, mypos[kngs], maxplayer);
            //now having all generated moves
            numactions = actionpl.numactions;
            for(int i = 0; i < numactions; i++){
                //do every move
                //actionpl.actions[i]
                //passing mypos will change them. Make another bitboard array with current position
                //virtualresults has different memory address
                virtualresults[0] = mypos[rds]; 
                virtualresults[1] = mypos[bls];
                virtualresults[2] = mypos[kngs];
                //make capture to change virtual results
                virtualMove(actionpl.actions[i], virtualresults, maxplayer);
                //at this point, we have a resultant board (VIRTUALRESULTS WILL HAVE CHANGED)
                //call minmax again to go further with the other capture
                eval_t movescore = minmax_abp(height, (depth+1), i, alpha, beta, (!(maxplayer)), virtualresults, 0);
                if (movescore.score > bestValue) {
                    bestValue = movescore.score;
                    bestchoice = movescore;
                    bestchoice.index = i;
                }
                if (bestValue > alpha)
                    alpha = bestValue;
                if (beta <= alpha)
                    break;
            }
            freeActions(actionpl);
            //this has scowered all best moves, selecting the best move for a certain node
        }
        //free(virtualresults);
        //this is only reached when good 
        //freeActions(actionpl);
        if (numactions == 0) {
            bestchoice.index = index;
            bestchoice.score = totalEval(mypos);
            return bestchoice;
        }
        return bestchoice;
    } else {                    //Blue's turn (minimum) okayyyyyyyyyy
        int bestValue = INT_MAX;            //WORST VALUE FOR MIN/BLUE
        bestchoice.index = 0;
        bestchoice.score = bestValue;
        int numactions = 0;
        //get ALLMOVES
        moves = getBlueJumps(mypos[rds], mypos[bls], mypos[kngs]);
        if (moves) {    //if there are captures!
            movables = getBlueJumpers(mypos[rds], mypos[bls], mypos[kngs]);
            actionpl = getJumpsAI(movables, moves, mypos[kngs], mypos, maxplayer);     //generate every capture
            numactions = actionpl.numactions;
            for(int i = 0; i < numactions; i++){
                virtualresults[0] = mypos[rds]; 
                virtualresults[1] = mypos[bls];
                virtualresults[2] = mypos[kngs];
                virtualCapture(actionpl.actions[i], virtualresults, maxplayer);
                eval_t movescore = minmax_abp(height, depth+1, i, alpha, beta, maxplayer, virtualresults, 1);
                if (movescore.score < bestValue) {
                    bestValue = movescore.score;
                    bestchoice = movescore;
                    bestchoice.index = i;
                }
                if (bestValue < beta)
                    beta = bestValue;
                if (beta <= alpha)      //pruning
                    break;
            }
            freeActions(actionpl);
            //this has scowered all best moves, selecting the best move for a certain node
            //make capture then see if there is another capture
        } else if (prevcapt == 0) {    //if no captures
            moves = getBlueMoves(mypos[rds], mypos[bls], mypos[kngs]);
            if (moves == 0) { //No moves and no captures available
                //check for this so tree ends
                //return 0;
                bestchoice.index = index;           //index that brought about such
                bestchoice.score = INT_MAX;         //Min losing is best for Max (infinity max)
                //free(virtualresults);
                return bestchoice;
            }
            movables = getBlueMovers(mypos[rds], mypos[bls], mypos[kngs]);
            actionpl = getMovesAI(movables, moves, mypos[kngs], maxplayer);
            //printBoard(actionpl.actions[0]);
            //now having all generated moves
            numactions = actionpl.numactions;
            for(int i = 0; i < numactions; i++){
                //do every move
                //actionpl.actions[i]
                //passing mypos will change them. Make another bitboard array with current position
                //virtualresults has different memory address
                
                virtualresults[0] = mypos[rds]; 
                virtualresults[1] = mypos[bls];
                virtualresults[2] = mypos[kngs];
                //make capture to change virtual results
                virtualMove(actionpl.actions[i], virtualresults, maxplayer);
                //at this point, we have a resultant board (VIRTUALRESULTS WILL HAVE CHANGED)
                //call minmax again to go further with the other capture
                eval_t movescore = minmax_abp(height, (depth+1), i, alpha, beta, (!(maxplayer)), virtualresults, 0);
                if (movescore.score < bestValue) {
                    bestValue = movescore.score;
                    bestchoice = movescore;
                    bestchoice.index = i;
                }
                if (bestValue < beta)
                    beta = bestValue;
                if (beta <= alpha)  //Pruning
                    break;
            }
            //this has scowered all best moves, selecting the best move for a certain node
            freeActions(actionpl);
        }
        //free(virtualresults);
        if (numactions == 0){
            bestchoice.index = index;
            bestchoice.score = totalEval(mypos);
            return bestchoice;
        }
        return bestchoice;
    }
    bestchoice.index = index;
    bestchoice.score = totalEval(mypos);
    return bestchoice;
}

//array of moves[a, b, c, d, e]

// //initial: height = 3, depth = 0, alpha = -inf, beta = inf, maxplayer = 1, mypos = mypos
//     bitboard movables, moves;
//     if (maxplayer) {
//         moves = getRedJumps(allboards[rds], allboards[bls], allboards[kngs]);
//         actiongroup_t actionpl;
//         if (anycaptures){
//             movables = getRedJumpers(allboards[rds], allboards[bls], allboards[kngs]);
//             actionpl = getJumps(movables, anycaptures, allboards[kngs]);     //generate every capture
//             //iterate over the captures
//             for(int i = 0; i < numactions; i++){

//             }
//             makeCapture(selectMove(actionpl));

//             freeActions(actionpl);
//             //makeMove(selectMove(actionpl));
//             //make capture then see if there is another capture
//             if (!(promotePiece())){
//                 printf("\nPiece Promoted!\n");
//             }
//             anycaptures = getRedJumps(allboards[rds], allboards[bls], allboards[kngs]);
//         } else {    //no captures
//             anycaptures = getRedMoves(allboards[rds], allboards[bls], allboards[kngs]);
//             if (anycaptures == 0) { //No moves and no captures available
//                 printf("\nNo Moves for Red! Blue Wins!!!!\n\n");
//                 return 0;
//             }
//             printMoves(allboards[rds], allboards[bls], allboards[kngs]);
//             movables = getRedMovers(allboards[rds], allboards[bls], allboards[kngs]);
//             actionpl = getMoves(movables, anycaptures, allboards[kngs]);
//             makeMove(selectMove(actionpl));
//             freeActions(actionpl);
//             if (!(promotePiece())){
//                 printf("\nPiece Promoted!\n");
//             }
//         }
//     }
//     //there could be captures
    
// if (maxplayer)
//  movers = getRedMovers(allboards[rds], allboards[bls], allboards[kngs]);
//  moves = getRedMoves(allboards[rds], allboards[bls], allboards[kngs]);


//for loop
//recursive
//keep track of best value AND index
//further array of moves [aa, ab, ac, ad]

//further array of moves [aba, abb, abc]




