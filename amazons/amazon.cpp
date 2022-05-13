// Nathan Karr
// Honors Research
// Game of Amazons
#include "amazon.h"
#include "neural.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stack>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <float.h>
using namespace std;

void Game::resetBoard(){
   // Change to accept any size
   for (int i = 0; i < size; i++){
      board.resize(size);
      n = 1;
      for (int j = 0; j < size; j++){
         if((i == 0 && j == 0)){
            string input = "R" + to_string(n);
            board[i].push_back(input);
            redRows.push_back(i);
            redCols.push_back(j);
            n++;
         }
         else if ((i == size - 1 && j == size - 1)){
            string input = "B" + to_string(n);
            board[i].push_back(input);
            blackRows.push_back(i);
            blackCols.push_back(j);
            n++;
         }
         else{
            board[i].push_back("0");
         }
      }
   }
}
void Game::changeTurn(){
   if(playerOneTurn){
      playerOneTurn = false;
   }
   else{
      playerOneTurn = true;
   }
}
bool Game::isPlayerOneTurn(){
   if(playerOneTurn){
      return true;
   }
   else{
      return false;
   }      
}
void Game::printBoard(){
   n = 0;
   for (int i = 0; i < size; i++){
      cout << n << " | ";
      n++;
      for (int j = 0; j < size; j++){
         if(board[i][j] == "0" || board[i][j] == "1"){
            cout << board[i][j] << "  ";
         }
         else{
            cout << board[i][j] << " ";
         }
         
      }
      cout << endl;
   }
   n = 0;
   cout << "   ";
   for (int i = 0; i < size; i++){
      cout << "---";
   }
   cout << endl;
   cout << "   ";
   for (int i = 0; i < size; i++){
      cout << n << "  ";
      n++;
   }
   cout << endl;
}
void Game::shootBlock(int blockRow, int blockCol){
   board[blockRow][blockCol] = "1";
   numBlocks += 1;
}
void Game::makeMove(string piece, int moveRow, int moveCol){
   if(piece == "B1"){
      board[blackRows.at(0)][blackCols.at(0)] = "0";
      board[moveRow][moveCol] = piece;
      blackRows.at(0) = moveRow;
      blackCols.at(0) = moveCol;
   }
   else if(piece == "B2"){
      board[blackRows.at(1)][blackCols.at(1)] = "0";
      board[moveRow][moveCol] = piece;
      blackRows.at(1) = moveRow;
      blackCols.at(1) = moveCol;
   }
   else if(piece == "R1"){
      board[redRows.at(0)][redCols.at(0)] = "0";
      board[moveRow][moveCol] = piece;
      redRows.at(0) = moveRow;
      redCols.at(0) = moveCol;
   }
   else if(piece == "R2"){
      board[redRows.at(1)][redCols.at(1)] = "0";
      board[moveRow][moveCol] = piece;
      redRows.at(1) = moveRow;
      redCols.at(1) = moveCol;
   }
}
string Game::getPieceLocation(string piece){
   string pieceLocale;
   if(piece == "B1"){
      pieceLocale = to_string(blackRows.at(0)) + to_string(blackCols.at(0));
   }
   else if(piece == "B2"){
      pieceLocale = to_string(blackRows.at(1)) + to_string(blackCols.at(1));
   }
   else if(piece == "R1"){
      pieceLocale = to_string(redRows.at(0)) + to_string(redCols.at(0));
   }
   else if(piece == "R2"){
      pieceLocale = to_string(redRows.at(1)) + to_string(redCols.at(1));
   }
   return pieceLocale;
}

bool Game::isValidMove(int pieceRow, int pieceCol, int moveRow, int moveCol){
   if(pieceRow == moveRow && pieceCol == moveCol){ // Checks if player tried to move the piece to where it already is
      return false;
   }
   else if(board[moveRow][moveCol] != "0"){ // Checks if player tried to move to a block or another piece
      return false;
   }
   else if(pieceRow == moveRow){ // Checks if the move was made along the row
      if(moveCol < pieceCol){ // If moving left
         for(int i = pieceCol - 1; i >= moveCol; i--){ // Checks for any pieces or blockers in the way of the designated path
            if(board[pieceRow][i] != "0"){
               return false;
            }
         }
      }
      else{
         for(int i = pieceCol + 1; i < moveCol; i++){ // If moving right
            if(board[pieceRow][i] != "0"){ // Checks for any pieces or blockers in the way of the designated path
               cout << "4";
               return false;
            }
         }
      }
      return true;
   }
   else if(pieceCol == moveCol){ // Checks if move was made along the col
      if(moveRow < pieceRow){ // If moving up
         for (int i = pieceRow - 1; i >= moveRow; i--){ // Checks for any pieces or blockers in the way of the designated path
            if(board[i][pieceCol] != "0"){
               return false;
            }
         }
      }
      else{ // If moving down
         for (int i = pieceRow + 1; i < moveRow; i++){ // Checks for any pieces or blockers in the way of the designated path
            if(board[i][pieceCol] != "0"){
               return false;
            }
         }
      }
      return true;
   }
   else{
      int n = 1;
      if(moveRow < pieceRow && moveCol < pieceCol){ // Move is up and to the left
         for (int i = pieceRow - 1; i >= 0; i--){
            if(board[i][pieceCol - n] != "0"){
               return false;
            }
            if(i == moveRow && pieceCol - n == moveCol){ // Returns true if scanner has reached the move spot
               return true;
            }
            n++;
         }
      }
      else if(moveRow < pieceRow && moveCol > pieceCol){ // Move is up and to the right
         cout << "Up and to the right" << endl;
         for (int i = pieceRow - 1; i >= 0; i--){
            if(board[i][pieceCol + n] != "0"){
               return false;
            }
            if(i == moveRow && pieceCol + n == moveCol){ // Returns true if scanner has reached the move spot
               return true;
            }
            n++;
         }
      }
      else if(moveRow > pieceRow && moveCol < pieceCol){ // Move is down and to the left
         cout << "Down and to the left" << endl;
         for (int i = pieceRow + 1; i < size; i++){
            if (board[i][pieceCol - n] != "0"){
               return false;
            }
            if(i == moveRow && pieceCol - n == moveCol){ // Returns true if scanner has reached the move spot
               return true;
            }
            n++;
         }
      }
      else if(moveRow > pieceRow && moveCol > pieceCol){ // Move is down and to the right
         cout << "Down and to the right" << endl;
         for (int i = pieceRow + 1; i < size; i++){
            if (board[i][pieceCol + n] != "0"){
               return false;
            }
            if(i == moveRow && pieceCol + n == moveCol){ // Returns true if scanner has reached the move spot
               return true;
            }
            n++;
         }
      }
      return true;
   }
   return false;
}
int Game::checkWin(){
   if(!checkPiece(blackRows[0], blackCols[0]) && !checkPiece(redRows[0], redCols[0])){
      if(playerOneTurn){
         return 1;
      }
      else{
         return 2;
      }
   }
   if(!checkPiece(blackRows[0], blackCols[0])){
      return 2;
   }
   else if(!checkPiece(redRows[0], redCols[0])){
      return 1;
   }
   return 0;
}
bool Game::checkPiece(int row, int col){ // Checks given piece to see if it has any valid moves
   bool rowLess = false;
   bool colLess = false;
   bool rowGreat = false;
   bool colGreat = false;
   
   if(row - 1 >= 0){
      if(board[row - 1][col] == "0"){
         return true;
      }
      rowLess = true;
   }
   if(col - 1 >= 0){
      if(board[row][col - 1] == "0"){
         return true;
      }
      colLess = true;
   }
   if(rowLess && colLess){
      if(board[row - 1][col - 1] == "0"){
         return true;
      }
   }
   if(row + 1 < size){
      if(board[row + 1][col] == "0"){
         return true;
      }
      rowGreat = true;
   }
   if(col + 1 < size){
      if(board[row][col + 1] == "0"){
         return true;
      }
      colGreat = true;
   }
   if(rowGreat && colLess){
      if(board[row + 1][col - 1] == "0"){
         return true;
      }
   }
   if(rowGreat && colGreat){
      if(board[row + 1][col + 1] == "0"){
         return true;
      }
   }
   if(rowLess && colGreat){
      if(board[row - 1][col + 1] == "0"){
         return true;
      }
   }
   return false;
}
vector<string> Game::stackValidMoves(int row, int col){
   bool rowLess = false;
   bool colLess = false;
   bool rowGreat = false;
   bool colGreat = false;
   int tempRow;
   int tempCol;
   possibleMoves.clear();
   if(row - 1 >= 0){
      tempRow = row;
      while(tempRow - 1 >= 0){
         if(board[tempRow - 1][col] == "0"){
            if(tempRow - 1 == 0){
               possibleMoves.push_back("0" + to_string(col));
            }
            else{
               possibleMoves.push_back(to_string((tempRow - 1) * 10 + col));
            }
         }
         else{
            break;
         }
         tempRow--;
      }
      rowLess = true;
   }
   if(col - 1 >= 0){
      tempCol = col;
      while(tempCol - 1 >= 0){
         if(board[row][tempCol - 1] == "0"){
            if(row == 0){
               possibleMoves.push_back("0" + to_string(tempCol - 1));
            }
            else{
               possibleMoves.push_back(to_string(row * 10 + tempCol - 1));
            }
         }
         else{
            break;
         }
         tempCol--;
      }
      colLess = true;
   }
   if(rowLess && colLess){
      tempRow = row;
      tempCol = col;
      while(tempRow - 1 >= 0 && tempCol - 1 >= 0){
         if(board[tempRow - 1][tempCol - 1] == "0"){
            if(tempRow - 1 == 0){
               possibleMoves.push_back("0" + to_string(tempCol - 1));
            }
            else{
               possibleMoves.push_back(to_string((tempRow - 1) * 10 + tempCol - 1));
            }
         }
         else{
            break;
         }
         tempRow--;
         tempCol--;
      } 
   }
   if(row + 1 < size){
      tempRow = row;
      while(tempRow + 1 < size){
         if(board[tempRow + 1][col] == "0"){
            possibleMoves.push_back(to_string((tempRow + 1) * 10 + col));
         }
         else{
            break;
         }
         tempRow++;
      }
      
      rowGreat = true;
   }
   if(col + 1 < size){
      tempCol = col;
      while(tempCol + 1 < size){
         if(board[row][tempCol + 1] == "0"){
            if(row == 0){
               possibleMoves.push_back("0" + to_string(tempCol + 1));
            }
            else{
               possibleMoves.push_back(to_string(row * 10 + tempCol + 1));
            }
         }
         else{
            break;
         }
         tempCol++;
      }
      colGreat = true;
   }
   if(rowGreat && colLess){
      tempRow = row;
      tempCol = col;
      while(tempRow + 1 < size && tempCol - 1 >= 0){
         if(board[tempRow + 1][tempCol - 1] == "0"){
            possibleMoves.push_back(to_string((tempRow + 1) * 10 + tempCol - 1));
         }
         else{
            break;
         }
         tempRow++;
         tempCol--;
      }   
   }
   if(rowGreat && colGreat){
      tempRow = row;
      tempCol = col;
      while(tempRow + 1 < size && tempCol + 1 < size){
         if(board[tempRow + 1][tempCol + 1] == "0"){
            possibleMoves.push_back(to_string((tempRow + 1) * 10 + tempCol + 1));
         }
         else{
            break;
         }
         tempRow++;
         tempCol++;
      }
   }
   if(rowLess && colGreat){
      tempRow = row;
      tempCol = col;
      while(tempRow - 1 >= 0 && tempCol + 1 < size){
         if(board[tempRow - 1][tempCol + 1] == "0"){
            if(tempRow - 1 == 0){
               possibleMoves.push_back("0" + to_string(tempCol + 1));
            }
            else{
               possibleMoves.push_back(to_string((tempRow - 1) * 10 + tempCol + 1));
            }
         }
         else{
            break;
         }
         tempRow--;
         tempCol++;
      }
   }
   return possibleMoves;
}
vector<string> Game::stackValidMovesOneAway(int row, int col){
   bool rowLess = false;
   bool colLess = false;
   bool rowGreat = false;
   bool colGreat = false;
   possibleMoves.clear();
   if(row - 1 >= 0){
      if(board[row - 1][col] == "0"){
         if(row - 1 == 0){
            possibleMoves.push_back("0" + to_string(col));
         }
         else{
            possibleMoves.push_back(to_string((row - 1) * 10 + col));
         }
      }
      rowLess = true;
   }
   if(col - 1 >= 0){
      if(board[row][col - 1] == "0"){
         if(row == 0){
            possibleMoves.push_back("0" + to_string(col - 1));
         }
         else{
            possibleMoves.push_back(to_string(row * 10 + col - 1));
         }
      }
      colLess = true;
   }
   if(rowLess && colLess){
      if(board[row - 1][col - 1] == "0"){
         if(row - 1 == 0){
            possibleMoves.push_back("0" + to_string(col - 1));
         }
         else{
            possibleMoves.push_back(to_string((row - 1) * 10 + col - 1));
         }
      }
   }
   if(row + 1 < size){
      if(board[row + 1][col] == "0"){
         possibleMoves.push_back(to_string((row + 1) * 10 + col));
      }
      rowGreat = true;
   }
   if(col + 1 < size){
      if(board[row][col + 1] == "0"){
         if(row == 0){
            possibleMoves.push_back("0" + to_string(col + 1));
         }
         else{
            possibleMoves.push_back(to_string(row * 10 + col + 1));
         }
      }
      colGreat = true;
   }
   if(rowGreat && colLess){
      if(board[row + 1][col - 1] == "0"){
         possibleMoves.push_back(to_string((row + 1) * 10 + col - 1));
      }
   }
   if(rowGreat && colGreat){
      if(board[row + 1][col + 1] == "0"){
         possibleMoves.push_back(to_string((row + 1) * 10 + col + 1));
      }
   }
   if(rowLess && colGreat){
      if(board[row - 1][col + 1] == "0"){
         if(row - 1 == 0){
            possibleMoves.push_back("0" + to_string(col + 1));
         }
         else{
            possibleMoves.push_back(to_string((row - 1) * 10 + col + 1));
         }
      }
   }
   return possibleMoves;
}
/*
double miniMaxValue(Game game, int depth, const Network &neuralNet){ // Recursive part of minimax
   string bestMove;
   string pieceLocale;
   string piece;
   string testMove;
   string blockMove;
   vector<string> moves;
   vector<string> blocks;
   double evaluation;
   double bestOutcome;
   double outcome;
   // Currently unsure if anything in my minimax needs to be different for player one vs two or if thats all handled by the unwritten eval function, also unsure where the eval function is called
   // an example I saw online called it where my outcome check is
   if(game.checkWin() == 2){
      return -1 - depth;
      //return -1.0;
   }
   if(game.checkWin() == 1){
      return 1 + depth;
      //return 1.0;
   }
   if(depth == 0){
      evaluation = buildInputAndEvaluate(game, neuralNet);
      return evaluation;
      // return ???, I know this should end the recursive loop but I'm unsure what it should return once I've checked the end of my depth of board states
   }
   game.changeTurn();
   if(game.isPlayerOneTurn()){ // Player One eval
      piece = "B1"; // MUST CHANGE THIS LINE WHEN BOARD IS EXPANDED
      pieceLocale = game.getPieceLocation(piece);
      bestOutcome = INT_MIN;
      moves = game.stackValidMoves(pieceLocale.at(0) - '0', pieceLocale.at(1) - '0'); // Returns a vector of ints, ints are 2 digits and represent moves, for example 43 would be row 4 col 3
      for(uint i = 0; i < moves.size(); i++){ //Change first for to stackValidMoves
         Game tempGame = game;
         testMove = moves.at(i);
         tempGame.makeMove(piece, testMove.at(0) - '0', testMove.at(1) - '0'); // Makes a given move for given piece
         blocks = tempGame.stackValidMoves(testMove.at(0) - '0', testMove.at(1) - '0'); // Once move is made, moves on to testing block moves
         for(uint j = 0; j < blocks.size(); j++){
            Game blockTemp = tempGame;
            blockMove = blocks.at(j);
            blockTemp.shootBlock(blockMove.at(0) - '0', blockMove.at(1) - '0'); // Shoots block on temp board
            outcome = miniMaxValue(blockTemp, depth - 1, neuralNet); // Begins recursive loop by calling my minimax
            if(outcome > bestOutcome){
               bestOutcome = outcome;
            }
         }
      }
   }
   else{ // Player two eval
      piece = "R1"; // MUST CHANGE THIS LINE WHEN BOARD IS EXPANED
      pieceLocale = game.getPieceLocation(piece);
      bestOutcome = INT_MAX;
      moves = game.stackValidMoves(pieceLocale.at(0) - '0', pieceLocale.at(1) - '0'); // Returns a vector of ints, ints are 2 digits and represent moves, for example 43 would be row 4 col 3
      for(uint i = 0; i < moves.size(); i++){ //Change first for to stackValidMoves
         Game tempGame = game;
         testMove = moves.at(i);
         tempGame.makeMove(piece, testMove.at(0) - '0', testMove.at(1) - '0'); // Makes a given move for given piece
         blocks = tempGame.stackValidMoves(testMove.at(0) - '0', testMove.at(1) - '0'); // Once move is made, moves on to testing block moves
         for(uint j = 0; j < blocks.size(); j++){
            Game blockTemp = tempGame;
            blockMove = blocks.at(j);
            blockTemp.shootBlock(blockMove.at(0) - '0', blockMove.at(1) - '0'); // Shoots block on temp board
            outcome = miniMaxValue(blockTemp, depth - 1, neuralNet); // Begins recursive loop by calling my minimax
            if(outcome < bestOutcome){
               bestOutcome = outcome;
            }
         }
      }
   }
   return bestOutcome;
}
*/

double miniMaxValue(Game game, int depth, const Network &neuralNet, double alpha, double beta){ // Recursive part of minimax
   string bestMove;
   string pieceLocale;
   string piece;
   string testMove;
   string blockMove;
   vector<string> moves;
   vector<string> blocks;
   double evaluation;
   double outcome;
   bool breakLoop = false;
   // Currently unsure if anything in my minimax needs to be different for player one vs two or if thats all handled by the unwritten eval function, also unsure where the eval function is called
   // an example I saw online called it where my outcome check is
   if(game.checkWin() == 2){
      return -1;
      //return -1.0;
   }
   if(game.checkWin() == 1){
      return 1;
      //return 1.0;
   }
   if(depth == 0){
      evaluation = buildInputAndEvaluate(game, neuralNet);
      return evaluation;
      // return ???, I know this should end the recursive loop but I'm unsure what it should return once I've checked the end of my depth of board states
   }
   game.changeTurn();
   if(game.isPlayerOneTurn()){ // Player One eval
      piece = "B1"; // MUST CHANGE THIS LINE WHEN BOARD IS EXPANDED
      pieceLocale = game.getPieceLocation(piece);
      outcome = INT_MIN;
      moves = game.stackValidMoves(pieceLocale.at(0) - '0', pieceLocale.at(1) - '0'); // Returns a vector of ints, ints are 2 digits and represent moves, for example 43 would be row 4 col 3
      for(uint i = 0; i < moves.size(); i++){ //Change first for to stackValidMoves
         Game tempGame = game;
         testMove = moves.at(i);
         tempGame.makeMove(piece, testMove.at(0) - '0', testMove.at(1) - '0'); // Makes a given move for given piece
         blocks = tempGame.stackValidMoves(testMove.at(0) - '0', testMove.at(1) - '0'); // Once move is made, moves on to testing block moves
         for(uint j = 0; j < blocks.size(); j++){
            Game blockTemp = tempGame;
            blockMove = blocks.at(j);
            blockTemp.shootBlock(blockMove.at(0) - '0', blockMove.at(1) - '0'); // Shoots block on temp board
            outcome = max(outcome, miniMaxValue(blockTemp, depth - 1, neuralNet, alpha, beta)); // Begins recursive loop by calling my minimax
            alpha = max(alpha, outcome);
            if(outcome >= beta){
               breakLoop = true;
            }
            if(breakLoop){
               break;
            }
         }
         if(breakLoop){
            break;
         }
      }
   }
   else{ // Player two eval
      piece = "R1"; // MUST CHANGE THIS LINE WHEN BOARD IS EXPANED
      pieceLocale = game.getPieceLocation(piece);
      outcome = INT_MAX;
      moves = game.stackValidMoves(pieceLocale.at(0) - '0', pieceLocale.at(1) - '0'); // Returns a vector of ints, ints are 2 digits and represent moves, for example 43 would be row 4 col 3
      for(uint i = 0; i < moves.size(); i++){ //Change first for to stackValidMoves
         Game tempGame = game;
         testMove = moves.at(i);
         tempGame.makeMove(piece, testMove.at(0) - '0', testMove.at(1) - '0'); // Makes a given move for given piece
         blocks = tempGame.stackValidMoves(testMove.at(0) - '0', testMove.at(1) - '0'); // Once move is made, moves on to testing block moves
         for(uint j = 0; j < blocks.size(); j++){
            Game blockTemp = tempGame;
            blockMove = blocks.at(j);
            blockTemp.shootBlock(blockMove.at(0) - '0', blockMove.at(1) - '0'); // Shoots block on temp board
            outcome = min(outcome, miniMaxValue(blockTemp, depth - 1, neuralNet, alpha, beta)); // Begins recursive loop by calling my minimax
            beta = max(beta, outcome);
            if(outcome <= alpha){
               breakLoop = true;
            }
            if(breakLoop){
               break;
            }
         }
         if(breakLoop){
            break;
         }
      }
      
   }
   return outcome;
}

string agentMinimax (Game game, const Network &neuralNet){ // Minimax agent
   // Select piece R1
   // Give move, if valid move, make it, and give block move, if also valid, recurse, then setup evaluate
   // Repeat for R2
   // Pick highest score
   // Return
   string bestMove;
   string pieceLocale;
   string piece;
   string testMove;
   string blockMove;
   vector<string> moves;
   vector<string> blocks;
   double bestOutcome;
   double outcome;
   if(game.isPlayerOneTurn()){
      bestOutcome = -DBL_MAX;
      piece = "B1";
   }
   else{
      bestOutcome = DBL_MAX;
      piece = "R1";
   }
   pieceLocale = game.getPieceLocation(piece); // Currently just checks one piece, need to edit so it checks all pieces, I made the ai Red so it needs to check pieces R1 and R2 (Potentially R3 and R4 if we expand board)
   moves = game.stackValidMoves(pieceLocale.at(0) - '0', pieceLocale.at(1) - '0'); // Returns a vector of ints, ints are 2 digits and represent moves, for example 43 would be row 4 col 3
   for(uint i = 0; i < moves.size(); i++){ //Change first for to stackValidMoves
         testMove = moves.at(i);
         Game tempGame = game;
         tempGame.makeMove(piece, testMove.at(0) - '0', testMove.at(1) - '0'); // Makes a given move for given piece
         blocks = tempGame.stackValidMoves(testMove.at(0) - '0', testMove.at(1) - '0'); // Once move is made, moves on to testing block moves
         for(uint j = 0; j < blocks.size(); j++){
            Game blockTemp = tempGame;
            blockMove = blocks.at(j);
            blockTemp.shootBlock(blockMove.at(0) - '0', blockMove.at(1) - '0'); // Shoots block on temp board
            outcome = miniMaxValue(blockTemp, 0, neuralNet, DBL_MIN, DBL_MAX); // Begins recursive loop by calling my minimax
            //cout << "Outcome: " << outcome << endl;
            // if (p1 ? o > b : o < b)
            /*
            cout << "Outcome" << outcome << endl;
            cout << "Best Outcome: " << bestOutcome << endl;
            */
            if (game.isPlayerOneTurn() ? outcome > bestOutcome : outcome < bestOutcome){
               bestOutcome = outcome;
               bestMove = piece + testMove + blockMove;
            }
         }
   }
   /*
   cout << "Piece: " << piece << endl;
   cout << "testMove: " << testMove << endl;
   cout << "blockMove: " << blockMove << endl;
   cout << "bestMove: " << bestMove << endl;
   */
   return bestMove; // Currently returns 0 just so make doesn't yell
}

string agentRandom(Game game, const Network &neuralNet){
   int randSize;
   string move;
   string block;
   string selectedPiece;
   string pieceLocale;
   vector<string> possibleMoves;
   Game tempGame = game;
   int randNum;
   string moveString;
   selectedPiece = "R1";
   pieceLocale = game.getPieceLocation(selectedPiece);
   //if(num == 1){
      /*
      if(!game.checkPiece(pieceLocale[0] - '0', pieceLocale[1] - '0')){
         selectedPiece = "R2";
         pieceLocale = game.getPieceLocation(selectedPiece);
      }
      */
   //}
   /*
   else{
      selectedPiece = "R2";
      pieceLocale = game.getPieceLocation(selectedPiece);
      if(!game.checkPiece(pieceLocale[0] - '0', pieceLocale[1] - '0')){
         selectedPiece = "R1";
         pieceLocale = game.getPieceLocation(selectedPiece);
      }
   }
   */
   // Check all valid moves for piece
   possibleMoves = tempGame.stackValidMoves(pieceLocale[0] - '0', pieceLocale[1] - '0');
   // Select random move
   randSize = tempGame.possibleMoves.size();
   randNum = random() % randSize;
   move = possibleMoves.at(randNum);
   tempGame.makeMove(selectedPiece, move.at(0) - '0', move.at(1) - '0');
   possibleMoves = tempGame.stackValidMoves(move.at(0) - '0', move.at(1) - '0');
   randSize = tempGame.possibleMoves.size();
   randNum = random() % randSize;
   block = possibleMoves.at(randNum);
   moveString = selectedPiece + move + block;
   return moveString;
}
string playerAgent(Game game, const Network &neuralNet){
   string player_move;
   string piece;
   string pieceLocale;
   string blockSpot;
   string selectedPiece;
   string moveString;
   bool pieceSelect = false;
   bool moveMade = false;
   bool blockShot = false;
   bool test;
   while(!pieceSelect){
      if (game.isPlayerOneTurn()){
         cout << "Player 1, Select Piece: ";
         cin >> piece;
         if(!isalpha(piece[0]) || piece[1] - '0' > 2 || piece.length() != 2){
            cout << "Not a valid piece, select either B1 or B2" << endl;
         }
         else if(piece[0] != 'B'){
            cout << "Not your piece player 1, select a black piece" << endl;
         }
         pieceLocale = game.getPieceLocation(piece);
         if(!game.checkPiece(pieceLocale[0] - '0', pieceLocale[1] - '0')){
            cout << "That piece is stuck! Select a different one" << endl;
         }
         else{
            pieceSelect = true;
         }
      }
   }
   while(!moveMade){
      cout << "Make Move: ";
      cin >> player_move;
      // If move is valid, perform move
      test = game.isValidMove(pieceLocale[0] - '0', pieceLocale[1] - '0', player_move[0] - '0', player_move[1] - '0');
      if(test){
         game.makeMove(piece, player_move[0] - '0', player_move[1] - '0');
         game.printBoard();
         moveMade = true;
      }
      else{
         cout << "Invalid move, enter valid move" << endl;
      }
   }
   pieceLocale = player_move;
   while(!blockShot){
      cout << "Shoot Block: ";
      cin >> blockSpot;
      test = game.isValidMove(pieceLocale[0] - '0', pieceLocale[1] - '0', blockSpot[0] - '0', blockSpot[1] - '0');
      if(test){
         blockShot = true;
      }
      else{
         cout << "Invalid spot, enter valid move" << endl;
      }
   }
   moveString = piece + player_move + blockSpot;
   return moveString;
}
int playAVisualGame(string (*blackAgent)(Game game, const Network &neuralNet), string (*redAgent)(Game game, const Network &neuralNet), const Network &blackNeuralNet, const Network &redNeuralNet, int boardSize){
   //int size;
   //cout << "Set board size: ";
   //cin >> size;
   Game game(boardSize);
   string agentPiece;
   string agentMove;
   int agentMoveRow;
   int agentMoveCol;
   int agentBlockRow;
   int agentBlockCol;
   bool gameOver = false;
   
   game.printBoard();
   while(!gameOver){
      if(game.isPlayerOneTurn()){
         agentMove = (*blackAgent)(game, blackNeuralNet);
         cout << "Player one move:" << agentMove << endl;
         agentPiece = agentMove.substr(0, 2);
         agentMoveRow = agentMove.at(2) - '0';
         agentMoveCol = agentMove.at(3) - '0';
         agentBlockRow = agentMove.at(4) - '0';
         agentBlockCol = agentMove.at(5) - '0';
         game.makeMove(agentPiece, agentMoveRow, agentMoveCol);
         game.shootBlock(agentBlockRow, agentBlockCol);
         game.printBoard();
      }
      else{ // Agent Moving
         agentMove = (*redAgent)(game, redNeuralNet);
         cout << "Player two move:" << agentMove << endl;
         agentPiece = agentMove.substr(0, 2);
         agentMoveRow = agentMove.at(2) - '0';
         agentMoveCol = agentMove.at(3) - '0';
         agentBlockRow = agentMove.at(4) - '0';
         agentBlockCol = agentMove.at(5) - '0';
         game.makeMove(agentPiece, agentMoveRow, agentMoveCol);
         game.shootBlock(agentBlockRow, agentBlockCol);
         game.printBoard();
      }
      if(game.checkWin() == 1){
         return 1;
      }
      else if(game.checkWin() == 2){;
         return 2;
      }
      game.changeTurn();
   }
   return 0;
}

int playAGame(string (*blackAgent)(Game game, const Network &neuralNet), string (*redAgent)(Game game, const Network &neuralNet), const Network &blackNeuralNet, const Network &redNeuralNet, int boardSize){
   //int size;
   //cout << "Set board size: ";
   //cin >> size;
   Game game(boardSize);
   string agentPiece;
   string agentMove;
   int agentMoveRow;
   int agentMoveCol;
   int agentBlockRow;
   int agentBlockCol;
   int finalScore = 0;
   int numPieces = 2;
   bool gameOver = false;
   while(!gameOver){
      if(game.isPlayerOneTurn()){
         agentMove = (*blackAgent)(game, blackNeuralNet);
         agentPiece = agentMove.substr(0, 2);
         agentMoveRow = agentMove.at(2) - '0';
         agentMoveCol = agentMove.at(3) - '0';
         agentBlockRow = agentMove.at(4) - '0';
         agentBlockCol = agentMove.at(5) - '0';
         game.makeMove(agentPiece, agentMoveRow, agentMoveCol);
         game.shootBlock(agentBlockRow, agentBlockCol);
      }
      else{ // Agent Moving
         agentMove = (*redAgent)(game, redNeuralNet);
         agentPiece = agentMove.substr(0, 2);
         agentMoveRow = agentMove.at(2) - '0';
         agentMoveCol = agentMove.at(3) - '0';
         agentBlockRow = agentMove.at(4) - '0';
         agentBlockCol = agentMove.at(5) - '0';
         game.makeMove(agentPiece, agentMoveRow, agentMoveCol);
         game.shootBlock(agentBlockRow, agentBlockCol);
      }
      if(game.checkWin() == 1){
         finalScore += 16 + (boardSize * boardSize - game.getNumBlocks() - numPieces);
         return finalScore;
      }
      else if(game.checkWin() == 2){
         finalScore += -16 - (boardSize * boardSize - game.getNumBlocks() - numPieces);
         return finalScore;
      }
      game.changeTurn();
   }
}

int main(){
   srandom (time(0));
   // https://www.delftstack.com/howto/cpp/how-to-generate-random-doubles-cpp/
   /*
   constexpr int MIN = -1;
   constexpr int MAX = 1;
   std::random_device rd;
   std::default_random_engine eng(rd());
   std::uniform_real_distribution<double> distr(MIN, MAX);
   */
   vector<int> numPerceptPerLayer = {6, 8, 1};
   int result;
   int activationNum = 3;
   int boardSize = 4;
   string response;
   //cout << setprecision(10) << distr(eng) << endl;
   //int neuronTest = neuron();
   //cout << "Neuron Test: " << neuronTest << endl;
   Network blackNeuralNet(numPerceptPerLayer,0);
   Network redNeuralNet(numPerceptPerLayer,0);
   Network bestPlayer(numPerceptPerLayer,0);
   Game board(4);
   //outPutNeuronWeights();
   cout << "Load or Run?" << endl;
   cin >> response;
   if(response == "load"){
      cout << "Player or Population?";
      cin >> response;
      if(response == "Player"){
         while(response != "no"){
            Network indiv1 = loadIndiv();
            Network indiv2 = loadIndiv();
            result = playAVisualGame(&agentMinimax, &agentMinimax, indiv1, indiv2, 4);
            cout << "Result: " << result << endl;
            cout << "Play Again?";
            cin >> response;
            cout << endl;
         }
         
      }
      else{
         while(response != "no"){
            Population pop1 = loadPopulation();
            Population pop2 = loadPopulation();
            result = playPopulations(pop1, pop2);
            cout << "Result: " << result << endl;
            cout << "Play Again?";
            cin >> response;
            cout << endl;
         }  
      }
        
   }
   else{
      bestPlayer = genetic(6, 6, numPerceptPerLayer, activationNum, boardSize, true);
      result = playAVisualGame(&playerAgent, &agentMinimax, bestPlayer, bestPlayer, 4);
      cout << "Player " << result << " won." << endl; 
      result = playAVisualGame(&agentMinimax, &agentMinimax, bestPlayer, bestPlayer, 4);
      cout << "Player " << result << " won." << endl;
   }
   //buildInputAndEvaluate(board, blackNeuralNet);
   //result = playAGame(&playerAgent, &agentMinimax, blackNeuralNet, redNeuralNet, 4);
   //result = playAGame(&agentMinimax, &agentMinimax, blackNeuralNet, redNeuralNet, 4);
   //cout << "One game played" << endl;
}

/*
   int barValue(const ChocolateBar &bar, int depth)
{
   ChocolateBar newBar;
   ChocolateSquareLocation bestBite, bite;
   int bestOutcome, biteColumn, biteRow, outcome;
   if bar has no more squares left:
      return depth % 2 == 0 ? 100 : -100;
   if depth is too deep:
      return 0;
   bestOutcome = depth % 2 == 0 ? INT_MIN : INT_MAX;
   for each column biteColumn:
      for each row biteRow:
         bite = ChocolateSquareLocation(biteColumn, biteRow);
         if bite is a legal move on bar:
            newBar = bar;
            newBar.takeBite(bite);
            outcome = barValue(newBar, depth + 1);
            if outcome is better than bestOutcome:
               bestOutcome = outcome;
   return bestOutcome;
}

ChocolateSquareLocation crrAgentPlainMinimax(const ChocolateBar &bar)
{
   ChocolateBar newBar;
   ChocolateSquareLocation bestBite, bite;
   int bestOutcome, biteColumn, biteRow, outcome;
   bestOutcome = INT_MIN;
   for each column biteColumn:
      for each row biteRow:
         bite = ChocolateSquareLocation(biteColumn, biteRow);
         if bite is a legal move on bar:
            newBar = bar;
            newBar.takeBite(bite);
            outcome = barValue(newBar, 1);
            if outcome is better than bestOutcome:
               bestBite = bite;
               bestOutcome = outcome;
   return bestBite;
}
*/