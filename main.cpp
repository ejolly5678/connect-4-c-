/**
Elizabeth Jolly
main file runs the connect 4 game

**/

#include <iostream>
#include <fstream>
//#include <thread>
//#include <mutex>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



using namespace std;
static int board[6][7]; //this is global because i was getting a lot of segmentation errors without it global and didn't have time to figure out how else to do it
//I know this is bad practice
bool winnerFound = false; //tells if the winner if found
pthread_mutex_t lock; //allows for the mutex lock on t1 and t2

//functions
void fillEmptyBoard();
void printBoard();
bool playPiece(int column, int player);
void checkHorizontal(int row, int column);
void endGame(int player);
void checkVertical(int row, int column);
bool boardFull();
void *playing(void *i);

int main(int argc, char** argv){
     
     //board set up
    fillEmptyBoard();
    //C4helper C4; //for helper methods
    //printBoard();
    ///thread main 
    cout << "main thread created" << endl;
     int p1 = 1; //representation of player 1
     int p2 = 2; //representation of palyer 2
     pthread_t t1, t2; //threads
     cout << "t1 created" << endl;
     cout << "t2 created" <<endl;
      pthread_mutex_init(&lock, NULL);
    while(!winnerFound && boardFull()){
        
        pthread_create(&t1, NULL, playing, (void *) &p1);
        sleep(1);
	    pthread_create(&t2, NULL, playing, (void *) &p2);
    }
     pthread_mutex_destroy(&lock); 
     cout << "mutex destroyed";
    // pthread_exit(t1);
     //cout << "t1 destroyed";

    
    return 0;
}

//fills the board with empty/0
void fillEmptyBoard(){
 for(int i = 0; i < 6; i++){
        for(int j = 0; j < 7; j++){
            board[i][j] = 0;
        }
    }
}

//prints out the current state of the board
void printBoard(){
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 7; j++){
            if(board[i][j] == 0)
                cout << "-  ";
            else if(board[i][j] == 1)
                cout << "⚫ ";
            else if(board[i][j] == 2)
                cout << "⚪ ";
            else  //for testing comment out later
                cout << "wtf";
        }
        cout << endl;

    }
    cout << endl;
}

//plays a piece given the column to put the piece in and the player
 bool playPiece(int column, int player){
    if(column > 6 || column < 0 || (player != 2 && player != 1))
        return false; //represents that it couldn't be added to the board
    bool foundLoc = false;
    for(int row = 5; row > 0; row--){
        if(board[row][column] == 0){
            if(foundLoc == false)
                board[row][column] = player;
            checkHorizontal(row, column);
            checkVertical(row, column);
            foundLoc = true;
            return foundLoc;
        }
    }
    
    return false;
 }

//checks the horizontal for if it's correct given the row and column of the last placed piece
 void checkHorizontal(int row, int column){
    int next_to_it = 0;
    if(column > 0){ //left side
        bool blocked = false;
        for(int i = column - 1; i >= 0; i --){
            if(board[row][i] == board[row][column] && blocked == false)
                next_to_it ++;
            else 
                blocked = true;
        }
    }
    if(column < 7){ //right side
         bool blocked = false;
          for(int i = column + 1; i < 7; i ++){
            
            if(board[row][i] == board[row][column] && blocked == false)
                next_to_it ++;
            else 
                blocked = true;
        }
    }
    if (next_to_it >= 3)
        endGame(board[row][column]);
 }

 //checks the vertical for if it's correct given the row and column of the last places piece
 void checkVertical(int row, int column){
     int next_to_it = 0;
     
    for(int i = 1; i < 6; i ++){
        if(board[i][column] != 0 && board[i][column] == board[i -1][column]){
            next_to_it ++;
            if(next_to_it > 3){
                endGame(board[row][column]);
        }
        }
        else 
            next_to_it = 0;
    }
 }

 void checkDiagnal(int row, int column){
     int next_to_it = 0;
     for(int i = 0; i < 6 - 3; i ++){
         for(int j = 0; j < 7; j ++){
             if(board[i][j] != 0 && board[i][j] == board[i+1][j+1] 
                && board[i][j] == board[i+2][i+2] && board[i][j] == board[i+3][j+3])
                    endGame(board[i][j]);
         }
     }
 }

bool boardFull(){
    bool zerosFound = false;
    for(int row = 0; row < 6; row ++){
        for(int column=0; column < 7; column++){
            if(board[row][column] == 0)
                zerosFound = true;
        }
    }
    return zerosFound;
}
//condition for winning the game
 void endGame(int player){
     cout << to_string(player) + " won!";
     //winnerFound == true; 
     
 }

 

 void *playing(void *i){
     pthread_mutex_lock(&lock); 
     printBoard();
    unsigned int player = *((unsigned int *) i);
    cout << "thread " + to_string(player) + " activated" <<endl;
     cout << "enter number 0-6 for column to put player " + to_string(player) + "'s move";
     int column; //column that user inters
     cin >> column;
     playPiece(column, player);
     pthread_mutex_unlock(&lock); 
     cout << "thread " + to_string(player) + " deactivated" <<endl;
 }