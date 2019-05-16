

#include <string>
#include "Piece.h"
#include "Player.h"
#include "AI.h"
#include "Human.h"
#include "CheckerBoard.h"
#include "Utils.h"
#include <iostream>
#include "serialport.h"
#include <time.h>



using namespace std;

/*
Board:
x = column (0 to 7)
y = row (0 to 7)

* * * * * * * *
* * * * * * * *
* * * * * * * *
* * * * * * * *
* * * * * * * *
* * * * * * * *
* * * * * * * 2
* * * * * * * *

Position of 2: 
x = 7
y = 6

*/
// Globals:
// 05 to 14
// foreground, background, screen
// initializing Serial port
SerialPort Serial("/dev/ttyACM0");
// inititalizing the board which will repesent the game
// and has top side white
CheckerBoard *table = new CheckerBoard(WHITE);
// Create a human class for human
// the Base is a player class
Player *player_black = new Human(BLACK, table);
// Create an AI class for the AI
// the base is a player class
AI *player_white = new AI(WHITE, table);

string x1,y1,x,y;

// function draws thw board on to the terminal
void draw_board(){
    // initialize 8 by 8 array (board size)
    char print[8][8];
    // initialize all the elemets with "-"
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            print[i][j] = '-';
        }
    }
    // iterate through the white peieces and replace the
    // element with 'O'
    for (auto i : table->WhitePieces){
        print[i.X()][i.Y()] = 'o';
    }
    // iterate through the white peieces and replace the
    // element with 'X'
    for (auto j : table->BlackPieces){
        print[j.X()][j.Y()] = 'x';
    }
    // iterate through the array and print the elements
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            cout << print[j][i] << " ";
        }
        cout << endl;
    }
}

// Move function will move the piece from initial position to final
// and communicate with the client
bool move(GameMove *pMove, piece_color color){
    //if the move is pointing to a move
    if(pMove != NULL){
        // points represents if the move is valid and
        // wether there was a kill
        int points=table->isMoveValid(pMove->start,
                                   pMove->end,
                                   color);
        //if the move is valid
        if(points >= 0)
        {
            //print the move
            cout<<pMove->start.first<<" "<<pMove->start.second<<endl;
            cout<<pMove->end.first<<" "<<pMove->end.second<<endl;
            Piece * p = table->getPiece(pMove->start,color);
            // to check if the move made created a king
            table->checkPiecePromotion(p,pMove->end,color) ;
            string king;
            // if the move did make king
            // then set the string king as 1
            (p->isKing()) ? king="1":king="0";
            //if the points are greater than 0
            //there was a piece attacked therefore
            // communicate with the server about it
            string del;
            (points>0)?del="1":del="0";
            //if the color of the player was white (AI)
            if(color==player_white->Color())
            {
                //make arduino get ready for the input for AI
                Serial.writeline("A\n");
                //communicate with the server
                string output =to_string(pMove->start.first)+' '+to_string(pMove->start.second)+' '+to_string(pMove->end.first)+' '+to_string(pMove->end.second)+' '+king+' '+del+"\n";
                cout<<output ;
                Serial.writeline(output);
            }
            //if the color of the player was black (Human)
            else if(color==player_black->Color())
            {
                //make arduino get ready for the input for Human
                Serial.writeline("B\n");
                //communicate with the server
                string output =king+' '+del+"\n";
                cout<<output ;
                Serial.writeline(output );

            }
            // print the move summary
            cout<<"Executing move!"<<endl;
            table->executeMove(pMove->start,
                                             pMove->end,
                                             color);
            cout << "From: ";
            cout << pMove->start.first << " " << pMove->start.second << endl;
            cout << "To: " << pMove->end.first << " " << pMove->end.second << endl;
            // draw the board
            draw_board();
            

        }
        else
        {   
            // print the error message
            cout<<"Invalid move to : "<<pMove->end.first <<","<<pMove->end.second <<endl;
            return false;
        }

        return true;
    }
    return false;
}

//General function which interprets client outputs and checks plimaniary errors and calls AI for move
void gameprocess()
{
    GameMove TempMove;
    cout << x << endl;
    position starting;
    starting.first = stoi(x);
    starting.second = stoi(y);
    TempMove.start = starting;
    TempMove.end.first = stoi(x1);
    TempMove.end.second = stoi(y1); 
    Piece *check = table->getPiece(starting, BLACK);
    Piece *check2 = table->getPiece(TempMove.end, BLACK);


    if (check == NULL)
    {
        cout << "The Piece does not exist" << endl;
        cout<<x<<" "<<y<<" "<<x1<<" "<<y1<<endl;

        return ;
    }

    else if(check2!=NULL)
    {
        cout << "Same Position" << endl;
        cout<<x<<" "<<y<<" "<<x1<<" "<<y1<<endl;
        return ;
    }

    else
    {
        GameMove *Move = &TempMove;

        bool flag = move(Move, player_black->Color());
        if(!flag)
            return ;
        if (table->WhitePieces.size() == 0){
            cout << "You have won the game!";
            return;
        }
        GameMove *TempMoveAI = player_white->play();
        flag = move(TempMoveAI, player_white->Color());
        if (flag)
            return;
        if (table->BlackPieces.size() == 0){
            cout << "AI has won the game! :)" << endl;
            return;
        }


    }
}


int main() {
    
    bool running = true;
    string welcome="Welcome to checkers\n";
    cout<<welcome;
    draw_board();
    int mode=0;
    string line;

    while(true)
    {
        switch(mode)
        {
            case 0:
            {
                clock_t timer=clock();
                while(clock()-timer<10000)
                {
                    line = Serial.readline();
                    x = Serial.readline();
                    y = Serial.readline();
                    x1 = Serial.readline();
                    y1 = Serial.readline();
                                        if(line=="M\n")
                    {
                        //Serial.writeline("A\n");
                        mode=1;
                        break;
                    }

                }

                break;

            }

            case 1:
            {
                gameprocess();
                line=Serial.readline();
                if(line=="A\n")
                {
                    mode=0;
                    break;
                }

                mode=0;
                break;
            }
        }

    }

    return 0;
}
