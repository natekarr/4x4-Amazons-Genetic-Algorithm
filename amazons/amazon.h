// Nathan Karr
// Honors Research
// Game of Amazons

#ifndef AMAZON_H
#define AMAZON_H

#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

class Game{
   private:
   int n;
   int numBlocks = 0;
   bool playerOneTurn = true;
   public:
   vector<int> blackRows;
   vector<int> blackCols;
   vector<int> redRows;
   vector<int> redCols;
   int size;
   vector<string> possibleMoves;
   vector<vector<string>> board;
   Game(int sizeInput) {size = sizeInput;
   resetBoard();}
   void newGame(){resetBoard();
   playerOneTurn = false;}
   void resetBoard();
   void changeTurn();
   bool isPlayerOneTurn();
   void printBoard();
   void shootBlock(int blockRow, int blockCol);
   vector<vector<string>> returnBoard(){return board;}
   void makeMove(string piece, int moveRow, int moveCol);
   string getPieceLocation(string piece);  
   bool isValidMove(int pieceRow, int pieceCol, int moveRow, int moveCol);
   int checkWin();
   bool checkPiece(int row, int col);
   vector<string> stackValidMoves(int row, int col);
   vector<string> stackValidMovesOneAway(int row, int col);
   string getSpot(int row, int col){return board[row][col];}
   int getSize(){return size;}
   int getNumBlocks(){return numBlocks;}
};
class Network;
string agentRandom(Game game, const Network &neuralNet);
double miniMaxValue(Game game, int depth, const Network &neuralNet);
string agentMinimax (Game game, const Network &neuralNet);
string playerAgent(Game game, const Network &neuralNet);
int playAVisualGame(string (*blackAgent)(Game game, const Network &neuralNet), string (*redAgent)(Game game, const Network &neuralNet), const Network &blackNeuralNet, const Network &redNeuralNet, int boardSize);
int playAGame(string (*blackAgent)(Game game, const Network &neuralNet), string (*redAgent)(Game game, const Network &neuralNet), const Network &blackNeuralNet, const Network &redNeuralNet, int boardSize);

#endif