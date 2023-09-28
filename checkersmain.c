
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "checkers.h"


//~~~~~~~~~~~~~~~~~~~~!!!!!!!!!!!!!!!!!!!!!!!!!!!!////////////////////
//Written By Tarik Merzkani (5/11/2022)
//~~~~~~~~~~~~~~~~~~~~!!!!!!!!!!!!!!!!!!!!!!!!!!!!////////////////////


int userGame(int diff){      
    printf(">>>>>>>>> Board: Red's Turn! >>>>>>>>>\n");
    //printf("<<<<<<<<< Board: Blue's Turn! <<<<<<<<<\n");
    //check if any captures can be done first
    bitboard anycaptures = getRedJumps(allboards[rds], allboards[bls], allboards[kngs]);
    bitboard movables;
    actiongroup_t actionpl;
    if (anycaptures){
        printf("Captures can be done:\n");
        while (anycaptures) {
            printJumps(allboards[rds], allboards[bls], allboards[kngs]);
            movables = getRedJumpers(allboards[rds], allboards[bls], allboards[kngs]);
            actionpl = getJumps(movables, anycaptures, allboards[kngs]);
            makeCapture(selectMove(actionpl));
            freeActions(actionpl);
            //makeMove(selectMove(actionpl));
            //make capture then see if there is another capture
            if (!(promotePiece())){
                printf("\nPiece Promoted!\n");
            }
            anycaptures = getRedJumps(allboards[rds], allboards[bls], allboards[kngs]);
        }
    } else {    //no captures
        anycaptures = getRedMoves(allboards[rds], allboards[bls], allboards[kngs]);
        if (anycaptures == 0) { //No moves and no captures available
            printf("\nNo Moves for Red! Blue Wins!!!!\n\n");
            return 0;
        }
        printMoves(allboards[rds], allboards[bls], allboards[kngs]);
        movables = getRedMovers(allboards[rds], allboards[bls], allboards[kngs]);
        actionpl = getMoves(movables, anycaptures, allboards[kngs]);
        makeMove(selectMove(actionpl));
        freeActions(actionpl);
        if (!(promotePiece())){
            printf("\nPiece Promoted!\n");
        }
    }
    turn++;
    
    printf("<<<<<<<<< Board: Blue's Turn! <<<<<<<<<\n");
    //check if any captures can be done first
    anycaptures = getBlueJumps(allboards[rds], allboards[bls], allboards[kngs]);
    if (anycaptures){
        printf("Captures can be done:\n");
        while (anycaptures) {
            printJumps(allboards[rds], allboards[bls], allboards[kngs]);
            movables = getBlueJumpers(allboards[rds], allboards[bls], allboards[kngs]);
            actionpl = getJumps(movables, anycaptures, allboards[kngs]);
            if (turn < 3) {
                makeCapture(move_AI(actionpl, (0), 0));
            } else {
                makeCapture(move_AI(actionpl, (0), diff));
            }
            freeActions(actionpl);
            //makeMove(selectMove(actionpl));
            //make capture then see if there is another capture
            if (!(promotePiece())){
                printf("\nPiece Promoted!\n");
            }
            anycaptures = getBlueJumps(allboards[rds], allboards[bls], allboards[kngs]);
        }
    } else {    //no captures
        anycaptures = getBlueMoves(allboards[rds], allboards[bls], allboards[kngs]);
        if (anycaptures == 0) { //No moves and no captures available
            printf("\nNo Moves for Blue! Red Wins!!!!\n");
            return 0;
        }
        printMoves(allboards[rds], allboards[bls], allboards[kngs]);
        movables = getBlueMovers(allboards[rds], allboards[bls], allboards[kngs]);
        actionpl = getMoves(movables, anycaptures, allboards[kngs]);
        if (turn < 7) {
            makeMove(move_AI(actionpl, (0), 0));
        } else {
            makeMove(move_AI(actionpl, (0), diff));
        }
        freeActions(actionpl);
        if (!(promotePiece())){
            printf("\nPiece Promoted!\n");
        }
    }
    turn++;
    return 1;
}

/*
clock_t begin = clock();      // current cpu moment

  Perform computation that takes a while;

  clock_t end = clock();        // later cpu moment

  double cpu_time =             // convert into seconds
    ((double) (end-begin)) / CLOCKS_PER_SEC;

  printf("Elapsed CPU Time: %f second\n", cpu_time);

*/

int normalGame(int diff1, int diff2, double* totaltime){           //DONEEEEEEEEEEEEE
    //printf(">>>>>>>>> Board: Red's Turn! >>>>>>>>>\n");
    //printPieces(allboards[rds], allboards[bls], allboards[kngs]);
    bitboard anycaptures = getRedJumps(allboards[rds], allboards[bls], allboards[kngs]);
    bitboard movables;
    actiongroup_t actionpl;
    if (anycaptures){
        while (anycaptures) {
            //printJumps(allboards[rds], allboards[bls], allboards[kngs]);
            stalltimer = turn;
            movables = getRedJumpers(allboards[rds], allboards[bls], allboards[kngs]);
            actionpl = getJumps(movables, anycaptures, allboards[kngs]);
            if (turn < 7) {
                makeCapture(move_AI(actionpl, (1), 0));
            } else {
                clock_t begin = clock();      // current cpu moment
                action mymove = move_AI(actionpl, (1), diff1);
                clock_t end = clock();        // later cpu moment
                double cpu_time =             // convert into seconds
                    ((double) (end-begin)) / CLOCKS_PER_SEC;
                *totaltime += cpu_time;
                //printf("%lf\n", cpu_time);
                makeCapture(mymove);
            }
            freeActions(actionpl);
            //makeMove(selectMove(actionpl));
            //make capture then see if there is another capture
            if (!(promotePiece())){
                //printf("Piece Promoted!\n");
            }
            anycaptures = getRedJumps(allboards[rds], allboards[bls], allboards[kngs]);
        }
    } else {    //no captures
        anycaptures = getRedMoves(allboards[rds], allboards[bls], allboards[kngs]);
        //printMoves(allboards[rds], allboards[bls], allboards[kngs]);
        if (anycaptures == 0) { //No moves and no captures available
            printf("\nBlue Wins!!!!\n");
            return 0;
        }
        movables = getRedMovers(allboards[rds], allboards[bls], allboards[kngs]);
        actionpl = getMoves(movables, anycaptures, allboards[kngs]);
        if (turn < 7) {
            makeMove(move_AI(actionpl, (1), 0));
        } else {
            clock_t begin = clock();      // current cpu moment
            action mymove = move_AI(actionpl, (1), diff1);
            clock_t end = clock();        // later cpu moment
            double cpu_time =             // convert into seconds
                    ((double) (end-begin)) / CLOCKS_PER_SEC;
            *totaltime += cpu_time;
            //printf("%lf\n", cpu_time);
            makeMove(mymove);
        }
        freeActions(actionpl);
        if (!(promotePiece())){
            //printf("Piece Promoted!\n");
        }
    }
    turn++;
    if ((turn-stalltimer) == 50){
        printf("\nSTALEMATE!\n");
        stalltimer = -1;
        return -1;
    }

    //RANDOM AI is difficulty 0 or lower    

    //printf("<<<<<<<<< Board: Blue's Turn! <<<<<<<<<\n");
    //printMoves(allboards[rds], allboards[bls], allboards[kngs]);
    anycaptures = getBlueJumps(allboards[rds], allboards[bls], allboards[kngs]);
    if (anycaptures){
        //printf("Captures can be done:\n");
        while (anycaptures) {
            //printJumps(allboards[rds], allboards[bls], allboards[kngs]);
            stalltimer = turn;
            movables = getBlueJumpers(allboards[rds], allboards[bls], allboards[kngs]);
            actionpl = getJumps(movables, anycaptures, allboards[kngs]);
            if (turn < 7) {
                makeCapture(move_AI(actionpl, (0), 0));
            } else {
                makeCapture(move_AI(actionpl, (0), diff2));
            }
            freeActions(actionpl);
            //makeMove(selectMove(actionpl));
            //make capture then see if there is another capture
            if (!(promotePiece())){
                //printf("Piece Promoted!\n");
            }
            anycaptures = getBlueJumps(allboards[rds], allboards[bls], allboards[kngs]);
        }
    } else {    //no captures
        anycaptures = getBlueMoves(allboards[rds], allboards[bls], allboards[kngs]);
        //printMoves(allboards[rds], allboards[bls], allboards[kngs]);
        if (anycaptures == 0) { //No moves and no captures available
            printf("\nRed Wins!!!!\n");
            return 0;
        }
        movables = getBlueMovers(allboards[rds], allboards[bls], allboards[kngs]);
        actionpl = getMoves(movables, anycaptures, allboards[kngs]);
        if (turn < 7) {
            makeMove(move_AI(actionpl, (0), 0));
        } else {
            makeMove(move_AI(actionpl, (0), diff2));
        }
        freeActions(actionpl);
        if (!(promotePiece())){
            //printf("Piece Promoted!\n");
        }
    }
    turn++;
    if ((turn-stalltimer) == 50){
        printf("\nSTALEMATE!\n");
        stalltimer = -1;
        return -1;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));   // Initialization, should only be called once.
    int run = 0;
    turn = 1;           //remember to change this for SELECTMOVE
    stalltimer = 0;
    int userFlag = 0;
    char someinput[10];
    int difficultygiven = 0, numgames = 0;
    if ((argc<2)){
        userFlag = 1;
        printf("========= ~~~~~~~~~~~~~~~~~~~ =========\n%s%s%s",
               "========= WELCOME TO CHECKERS =========\n",
               "========= ~~~~~~~~~~~~~~~~~~~ =========\n\n",
               "Please select a difficulty for the computer (0 for random, bigger number for better depth): \n");
        difficultygiven = scanf("%s", someinput);
        difficultygiven = atoi(someinput);
    }               
    if (difficultygiven<0) difficultygiven = 0;     
    printf("Please enter the amount of games to be played: \n");
    numgames = scanf("%s", someinput);
    numgames = atoi(someinput);
    create_board();
    // printBoard(getRedMoves(allboards[rds], allboards[bls], allboards[kngs]));
    // printBoard(getRedMovers(allboards[rds], allboards[bls], allboards[kngs]));
    int win1 = 0, win2 = 0, win3 = 0;
    double totaltime = 0;
    for(int i = 0; i < numgames; i++){
        //srand(time(NULL));   // Initialization, should only be called once.
        create_board();
        turn = 1;           //remember to change this for SELECTMOVE
        stalltimer = 0;
        run = 1;
        while(run){         //event loop
            //Red's turn
            if (userFlag){
                run = userGame(difficultygiven);
            } else {
                run = normalGame(9, 9, &totaltime);
            }
            //for now let's try user input
            //move by p1 (reds)==================

            //update probably?
            //move by p2 (blue)==================
            
            //Remember to free actions and jumps!!!
            if (run == -1) {
                //printf("ACTUAL STALEMATE\n");
                break;
            }
            //update everything
        }
        //If game ends on red's turn, blue wins, if game ends on blue's turn, red wins
        //(turn&1) = 1 if red turn (blue win), = 0 if blue turn (red win)
        //returns 0 red won, returns 1 blue won
        //printMoves(allboards[rds], allboards[bls], allboards[kngs]);
        //return (turn&1);
        if (run != -1){
            if (turn&1) {   //BLUE WON
                win2++;
            } else {        //REDWON
                win1++;
            }
        } else {            //STALEMATE
            win3++;
        }
        
    }
    printf("\n///////////\n\n%d, %d, %d\n\n///////////\n\n", win1, win2, win3);
    printf("Total Time taken all games: %lf , Time Averaged Per Game = %lf", totaltime, (totaltime)/numgames);
    return 0;
}


// bitboard randomposred = 0x0004000000440045;
    // bitboard randomposblue = 0x004008108210A200;
    // bitboard randomposred = 0x0004000000002804;     //AI POSITION TESTING. DIFFICULTY OF 3 SHOULD WIN
    // bitboard randomposblue = 0x0000020028000000;
    // bitboard randomposred = 0x0200000000000A40;
    // bitboard randomposblue = 0x000000002A000000;
    // allboards[0] = randomposred;
    // allboards[1] = randomposblue;
    // allboards[2] = 0x0;
    // printf("Board before move: \n");
    //printMoves(allboards[0], allboards[1], allboards[kngs]);
    //printMoves(allboards[rds], allboards[bls], allboards[kngs]);
    


    // SOME EVIDENCE OF TESTING FOR GAME ////////////////////////////////////////////////////
    // bitboard randomposred = 0x0004000000440045;
    // bitboard randomposblue = 0x004008108210A200;
    // allboards[0] = randomposred;
    // allboards[1] = randomposblue;
    // allboards[2] = 0x04080080400000;
    // printf("Board before move: \n");
    // printPieces(allboards[0], allboards[1], allboards[kngs]);
    // action to_do = selectMove(getJumps(getRedJumpers(allboards[0], allboards[1], allboards[2]), 
    //         getRedJumps(allboards[0], allboards[1], allboards[2]), allboards[2], 1));
    // makeCapture(to_do);
    // printf("Board after move: \n");
    // printPieces(allboards[0], allboards[1], allboards[kngs]);
    //printJumps(randomposred, randomposblue, 0, 1);



    //TO TEST AI MOVES//////////////////////////////////////////////////////////////////////
    // bitboard myboards[3];
    // bitboard randomposred = 0x0004000000440045;
    // bitboard randomposblue = 0x004008108210A200;
    // myboards[0] = randomposred;
    // myboards[1] = randomposblue;
    // myboards[2] = 0x04080080400000;

    // allboards[0] = randomposred;
    // allboards[1] = randomposblue;
    // allboards[2] = 0x04080080400000;
    // printPieces(allboards[0], allboards[1], allboards[kngs]);
    // printPieces(myboards[0], myboards[1], myboards[kngs]);

    // bitboard movables, moves;
    // moves = getBlueJumps(myboards[rds], myboards[bls], myboards[kngs]);
    // actiongroup_t actionpl;
    // movables = getBlueJumpers(myboards[rds], myboards[bls], myboards[kngs]);
    // printBoard(moves);
    // actionpl = getJumps(movables, moves, myboards[kngs]);     //generate every capture
    // makeCapture(selectMove(actionpl));
    
    // virtualCapture(actionpl.actions[0], myboards, 0);
    // printPieces(allboards[0], allboards[1], allboards[kngs]);
    // printPieces(myboards[0], myboards[1], myboards[kngs]);
    // printf("The score for BLUEpieces: %d\n", evaluatePos(allboards[1], 0));