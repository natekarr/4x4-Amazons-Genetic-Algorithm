// Nathan Karr
// Honors Research
// Game of Amazons
#include "neural.h"
#include "amazon.h"
#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <climits>
#include <random>
#include <deque>          // std::deque
#include <queue> 
#include <numeric>
#include <fstream>
using namespace std;

default_random_engine generator(time(0));
normal_distribution<double> PerceptronDistribution(0.0, 0.1);
normal_distribution<double> mutationDistribution(0.0, 0.01);

Perceptron::Perceptron(int numWeights, int activationNum){
   vector<double> newWeights;
   // Normal distribution of random weights in new weights
   for(int i = 0; i < numWeights; i++){
      newWeights.push_back(PerceptronDistribution(generator));
   }
   weights = newWeights;
   numInput = weights.size() - 1;
   this->activationNum = activationNum;
}
Perceptron::Perceptron(vector<double> newWeights, int activationNum){
   this->activationNum = activationNum;
   weights = newWeights;
   numInput = newWeights.size() - 1;
}
void Perceptron::changeWeights(vector<double> newWeights){
   weights = newWeights;
   numInput = newWeights.size() - 1;
}
void Perceptron::changeOneWeight(double weight, int k){
   weights.at(k) = weight;
}
vector<double> Perceptron::getWeights(){
   return weights;
}
double Perceptron::evaluate(vector<double> inputs){
   double sum = weights.at(0);
   for(int i = 0; i < numInput; i++){
      sum += inputs.at(i) * weights.at(i + 1);
   }
   //cout << "Sum is: " << sum << endl;
   //cout << "Activation num is: " << activationNum << endl;
   switch (activationNum){
      case 0:
         return sigmoid(sum);
         break;
      case 1:
         return tanh(sum);
         break;
      case 2:
         return ReLu(sum);
         break;
      case 3:
         return softPlus(sum);
         break;
   }
   return 0.0;
}
// Win is 1, loss is -1, not want to use sigmoid for final perceptron
double Perceptron::sigmoid(double sum){
   return 1.0 / (1.0 + exp(-sum));
}
double Perceptron::tanh(double sum){
   return ((exp(sum) - exp(-sum)) / (exp(sum) + exp(-sum)));
}
double Perceptron::ReLu(double sum){
   return max(0.0, sum);
}
double Perceptron::softPlus(double sum){
   return log(1.0 + exp(sum));
}
Layer::Layer(int numPercept, int activationNum, int numWeights){
    this->numPercept = numPercept;
    for(int i = 0; i < numPercept; i++){
       Perceptron p(numWeights, activationNum);
       perceptrons.push_back(p);
    }
}
vector<vector<double>> Layer::getWeights(){
   vector<vector<double>> layerWeights;
   vector<double> perceptronWeights;
   for(int i = 0; i < numPercept; i++){
      perceptronWeights = perceptrons.at(i).getWeights();
      layerWeights.push_back(perceptronWeights);
   }
   return layerWeights;
}
void Layer::changeWeights(vector<double> weights, int instruction){
   perceptrons.at(instruction).changeWeights(weights);
}
void Layer::changeOneWeight(double weight, int j, int k){
   perceptrons.at(j).changeOneWeight(weight, k);
}
vector<double> Layer::evaluate(vector<double> input){
   // Build vector of doubles of outputs for the layer, one for loop that goes through perceptrons in layer and get outputs
   vector<double> output;
   for(uint i = 0; i < perceptrons.size(); i++){
      output.push_back(perceptrons.at(i).evaluate(input));
   }
   return output;
}

Network::Network(Network father, Network mother, vector<int> numPerceptPerLayer){
   vector<vector<vector<double>>> fatherWeights = father.getWeights();
   vector<vector<vector<double>>> motherWeights = mother.getWeights();
   vector<vector<vector<double>>> childWeights;
   Network child = father;
   int numLayers = father.getNumLayers();
   for(int i = 0; i < numLayers; i++){
      int randOne = numLayers;
      //cout << "Number of perceptrons: " << numPerceptPerLayer.at(i + 1) << endl;
      for(int j = 0; j < numPerceptPerLayer.at(i + 1); j++){
         int randTwo = numPerceptPerLayer.at(i + 1);
         if(j == 0){
            //cout << "Number of weights: " << numPerceptPerLayer.at(i) + 1 << endl;
         }
         for(int k = 0; k < numPerceptPerLayer.at(i) + 1; k++){
            int randThree = numPerceptPerLayer.at(i) + 1;
            //cout << "i: " << i << endl;
            //cout << "j: " << j << endl;
            //cout << "k: " << k << endl;
            int randNum = random() % 100 + 1;
            int iRand = random() % randOne;
            int jRand;
            if(iRand == 0){
               jRand = random() % 7;
            }
            else{
               jRand = 0;
            }
            int kRand;
            if(i == 2){
               kRand = random() % 2;
            }
            else if (i == 1){
               kRand = random() % 7;
            }
            else{
               kRand = random() % 6;
            }
            /*
            if(randNum == 1){
               child.changeOneWeight(PerceptronDistribution(generator), i, j ,k);
            }
            */
            if(random() % 2 == 1){
               if(randNum == 1){
                  child.changeOneWeight(motherWeights.at(iRand).at(jRand).at(kRand), i, j ,k);
               }
               else{
                  child.changeOneWeight(motherWeights.at(i).at(j).at(k), i, j, k);
               }
               //child.changeOneWeight(motherWeights.at(i).at(j).at(k) + mutationDistribution(generator), i, j, k); // Changes weight to mother weight with a small mutation
               
               childWeights = child.getWeights();
               //cout << "Child weight at layer " << i << " perceptron " << j << " weight " << k << " " << childWeights.at(i).at(j).at(k) << endl;   
            }
            else{
               if(randNum == 1){
                  child.changeOneWeight(fatherWeights.at(iRand).at(jRand).at(kRand), i, j ,k);
               }
               else{
                  child.changeOneWeight(fatherWeights.at(i).at(j).at(k), i, j, k);
               }
               //child.changeOneWeight(fatherWeights.at(i).at(j).at(k) + mutationDistribution(generator), i, j, k); // Changes weight to father weight with a small mutation
               childWeights = child.getWeights();
               //cout << "Child weight at layer " << i << " perceptron " << j << " weight " << k << " " << childWeights.at(i).at(j).at(k) << endl;
            }
         }
      }
   }
   neuralNet = child.getNeuralNet();
}
// NumPerceptperLayer = first num, is num input, next nums are number of perceptron per layer, so 42321, would be 4 inputs, 2 nuerons first layer, 3 neurons next layer, etc. 
Network::Network(vector<int> numPerceptPerLayer, int activationNum){
   int numPercept;
   for(uint i = 1; i < numPerceptPerLayer.size(); i++){
      numPercept = numPerceptPerLayer.at(i);
      const int numWeights = numPerceptPerLayer.at(i - 1) + 1;
      // Activation num always 1 (tanh) on last perceptron, never call just one perceptron unless its last one
      Layer normalLayer(numPercept, (numPercept > 1 ? activationNum : 1), numWeights);
      neuralNet.push_back(normalLayer);
   }
}
vector<vector<vector<double>>> Network::getWeights(){
   vector<vector<vector<double>>> networkWeights;
   vector<vector<double>> layerWeights;
   for(uint i = 0; i < neuralNet.size(); i++){
      layerWeights = neuralNet.at(i).getWeights();
      networkWeights.push_back(layerWeights);
   }
   return networkWeights;
}
void Network::changeWeights(vector<double> weights, int layer, int instruction){
   neuralNet.at(layer).changeWeights(weights, instruction);
}
void Network::changeOneWeight(double weight, int i, int j, int k){
   neuralNet.at(i).changeOneWeight(weight, j, k);
}
// Network will be given a board and needs to evaluate how good the board is
double Network::evaluate(vector<double> input){
   // For loop that goes through layers and calls layer evaluate, outputs of one layer are inputs for next
   // Start with inputs to outputs, copy output as next layers input
   vector<double> output;
   for(uint i = 0; i < neuralNet.size(); i++){
      output = neuralNet.at(i).evaluate(input);
      input = output;
   }
   //cout << "Output in network Evaluate is: " << output.at(0) << endl;
   return output.at(0); // Returns 16 * tanh of the last evaluation
}
Population::Population(int populationHeight, int populationWidth, vector<int> numPerceptPerLayer, int activationNum, bool BFS){
   this->populationHeight = populationHeight;
   this->populationWidth = populationWidth;
   for(int i = 0; i < populationHeight; i++){
      vector<Network> row;
      for(int j = 0; j < populationWidth; j++){
         Network individual(numPerceptPerLayer, activationNum);
         individual.setBFS(BFS);
         row.push_back(individual);
      }
      population.push_back(row);
   }
}
void Population::setIndividual(Network child, int row, int col){
   population.at(row).at(col) = child;
}
Network Population::getIndividual(int row, int col){
   return population.at(row).at(col);
}
void Population::outputAllWeights(vector<vector<vector<double>>> individualWeights, int numLayers, vector<int> numPerceptPerLayer){
   for(int i = 0; i < numLayers; i++){
      //cout << "Number of perceptrons: " << numPerceptPerLayer.at(i + 1) << endl;
      for(int j = 0; j < numPerceptPerLayer.at(i + 1); j++){
         if(j == 0){
            cout << "Number of weights: " << numPerceptPerLayer.at(i) + 1 << endl;
         }
         for(int k = 0; k < numPerceptPerLayer.at(i) + 1; k++){
            cout << "Weight of layer " << i << " perceptron " << j << " weight " << k << " " << individualWeights.at(i).at(j).at(k) << endl;
         }
      }
   }
}
void Population::handleFitness(int givenFitness, Network &blackPlayer, Network &redPlayer){
   if(givenFitness > 0){
      blackPlayer.setFitness(givenFitness);
      redPlayer.setFitness(givenFitness * -1);
   }
   else{
      redPlayer.setFitness(givenFitness * -1);
      blackPlayer.setFitness(givenFitness);
   }
}

void Population::playNeighbors(int row, int col, int gameBoardSize){
   const int neighborLeft = (col + populationWidth - 1) % populationWidth;
   const int neighborRight = (col + populationWidth + 1) % populationWidth;
   const int neighborUp = (row + populationHeight - 1) % populationHeight;
   const int neighborDown = (row + populationHeight + 1) % populationHeight;
   
   handleFitness(playAGame(&agentMinimax, &agentMinimax, population.at(row).at(col), population.at(neighborUp).at(neighborLeft), gameBoardSize), population.at(row).at(col), population.at(neighborUp).at(neighborLeft));
   //cout << "Fitness of player one: " << population.at(row).at(col).getFitness() << endl;
   handleFitness(playAGame(&agentMinimax, &agentMinimax, population.at(row).at(col), population.at(neighborUp).at(col), gameBoardSize), population.at(row).at(col), population.at(neighborUp).at(col));
   //cout << "Fitness of player one: " << population.at(row).at(col).getFitness() << endl;
   handleFitness(playAGame(&agentMinimax, &agentMinimax, population.at(row).at(col), population.at(neighborUp).at(neighborRight), gameBoardSize), population.at(row).at(col), population.at(neighborUp).at(neighborRight));
   //cout << "Fitness of player one: " << population.at(row).at(col).getFitness() << endl;
   handleFitness(playAGame(&agentMinimax, &agentMinimax, population.at(row).at(col), population.at(row).at(neighborRight), gameBoardSize), population.at(row).at(col), population.at(row).at(neighborRight));
   //cout << "Fitness of player one: " << population.at(row).at(col).getFitness() << endl;
   handleFitness(playAGame(&agentMinimax, &agentMinimax, population.at(row).at(col), population.at(neighborDown).at(neighborRight), gameBoardSize), population.at(row).at(col), population.at(neighborDown).at(neighborRight));
   //cout << "Fitness of player one: " << population.at(row).at(col).getFitness() << endl;
   handleFitness(playAGame(&agentMinimax, &agentMinimax, population.at(row).at(col), population.at(neighborDown).at(col), gameBoardSize), population.at(row).at(col), population.at(neighborDown).at(col));
   //cout << "Fitness of player one: " << population.at(row).at(col).getFitness() << endl;
   handleFitness(playAGame(&agentMinimax, &agentMinimax, population.at(row).at(col), population.at(neighborDown).at(neighborLeft), gameBoardSize), population.at(row).at(col), population.at(neighborDown).at(neighborLeft));
   //cout << "Fitness of player one: " << population.at(row).at(col).getFitness() << endl;
   handleFitness(playAGame(&agentMinimax, &agentMinimax, population.at(row).at(col), population.at(row).at(neighborLeft), gameBoardSize), population.at(row).at(col), population.at(row).at(neighborLeft));
   //cout << "Fitness of player one: " << population.at(row).at(col).getFitness() << endl;
}
Network Population::selectMate(int row, int col){
   const int neighborLeft = (col + populationWidth - 1) % populationWidth;
   const int neighborRight = (col + populationWidth + 1) % populationWidth;
   const int neighborUp = (row + populationHeight - 1) % populationHeight;
   const int neighborDown = (row + populationHeight + 1) % populationHeight;
   vector<int> weights;
   vector<Network> individuals;
   int fitnessTotal = 0;
   int chosenMate = 0;
   int rand;
   int min;
   individuals.push_back(population.at(neighborUp).at(neighborLeft));
   individuals.push_back(population.at(neighborUp).at(col));
   individuals.push_back(population.at(neighborUp).at(neighborRight));
   individuals.push_back(population.at(row).at(neighborRight));
   individuals.push_back(population.at(neighborDown).at(neighborRight));
   individuals.push_back(population.at(neighborDown).at(col));
   individuals.push_back(population.at(neighborDown).at(neighborLeft));
   individuals.push_back(population.at(row).at(neighborLeft));
   
   // Put all fitness values in weights vector
   for(uint i = 0; i < individuals.size(); i++){
      weights.push_back(individuals.at(i).getFitness());
   }
   // Find minimum weight
   min = *min_element(weights.begin(), weights.end());
   // Subtract min from all weights
   for(uint i = 0; i < individuals.size(); i++){
      weights.at(i) = weights.at(i) - min;
   }
   // Total all weights
   fitnessTotal = accumulate(weights.begin(), weights.end(), 0);
   if(fitnessTotal == 0){
      fitnessTotal = 1;
   }
   rand = random() % fitnessTotal;
   // https://stackoverflow.com/questions/1761626/weighted-random-numbers
   for(uint i = 0; i < individuals.size(); i++){
      if(rand < weights.at(i)){
         chosenMate = i;
         break;
      }
      rand -= weights.at(i);
   }
   //cout << "Chosen Mate: " << chosenMate << endl;
   return individuals.at(chosenMate);
}
void testPrint(vector<vector<int>> test, int size){
   for(int i = 0; i < size; i++){
      for(int j = 0; j < size; j++){
         cout << test[i][j] << " ";
      }
      cout << endl;
   }
}
double Genetic::territoryOwnership(Game board){
   vector<string> blackMoves;
   vector<string> redMoves;
   // Defining and filling a test board just to be able to output a board and prove territoryOwnership is working correctly
   vector<vector<int>> ownerBoard;
   ownerBoard.resize(board.getSize());
   for (int i = 0; i < board.getSize(); i++){
      for (int j = 0; j < board.getSize(); j++){
         ownerBoard[i].push_back(0);
      }
   }
   int blackRow = board.blackRows.at(0);
   int blackCol = board.blackCols.at(0);
   int redRow = board.redRows.at(0);
   int redCol = board.redCols.at(0);
   double total = 0.0;
   // Loops through board
   // Stack all valid moves for red piece and black piece
   blackMoves = board.stackValidMoves(blackRow, blackCol);
   redMoves = board.stackValidMoves(redRow, redCol);
   total = static_cast<int>(blackMoves.size()) - static_cast<int>(redMoves.size());
   
   //testPrint(ownerBoard, board.getSize());
   return total;
}
double Genetic::closeMoves(Game board){
   vector<string> blackMoves;
   vector<string> redMoves;
   
   int blackRow = board.blackRows.at(0);
   int blackCol = board.blackCols.at(0);
   int redRow = board.redRows.at(0);
   int redCol = board.redCols.at(0);
   double total = 0.0;
   // Loops through board
   // Stack all valid moves for red piece and black piece
   blackMoves = board.stackValidMovesOneAway(blackRow, blackCol);
   redMoves = board.stackValidMovesOneAway(redRow, redCol);
   total = static_cast<int>(blackMoves.size()) - static_cast<int>(redMoves.size());
   
   return total;
}
double Genetic::closerToPieceChebyshev(Game board){
   int blackDist;
   int redDist;
   int blackRow = board.blackRows.at(0);
   int blackCol = board.blackCols.at(0);
   int redRow = board.redRows.at(0);
   int redCol = board.redCols.at(0);
   double total = 0.0;
   for(int i = 0; i < board.getSize(); i++){
      for (int j = 0; j < board.getSize(); j++){
         blackDist = max(abs(i - blackRow), abs(j - blackCol));
         //cout << "Black piece dist from space: " << i << ", " << j << " : " << blackDist << endl;
         redDist = max(abs(i - redRow), abs(j - redCol));
         //cout << "Red piece dist from space: " << i << ", " << j << " : " << redDist << endl;
         if(blackDist == redDist){
            total += 0.0;
         }
         else if(redDist < blackDist){
            total -= 1.0;
         }
         else{
            total += 1.0;
         }
      }
   }
   return total;
}

double Genetic::closerToPieceEuclidian(Game board){
   int blackDist;
   int redDist;
   int blackRow = board.blackRows.at(0);
   int blackCol = board.blackCols.at(0);
   int redRow = board.redRows.at(0);
   int redCol = board.redCols.at(0);
   double total = 0.0;
   for(int i = 0; i < board.getSize(); i++){
      for (int j = 0; j < board.getSize(); j++){
         blackDist = sqrt(pow(blackRow - blackCol, 2) + pow(i - j, 2));
         //cout << "Black piece dist from space: " << i << ", " << j << " : " << blackDist << endl;
         redDist = sqrt(pow(redRow - redCol, 2) + pow(i - j, 2));
         //cout << "Red piece dist from space: " << i << ", " << j << " : " << redDist << endl;
         if(blackDist == redDist){
            total += 0.0;
         }
         else if(redDist < blackDist){
            total -= 1.0;
         }
         else{
            total += 1.0;
         }
      }
   }
   return total;
}

void Genetic::breadthFirstSearch(Game board, bool blackPlayerSearch){
   int row;
   int col;
   string piece;
   // May need to add more pieces
   if(blackPlayerSearch){
     row = board.blackRows.at(0);
     col = board.blackCols.at(0);
     piece = "B1";
   }
   else{
      row = board.redRows.at(0);
      col = board.redCols.at(0);
      piece = "R1";
   }
   vector<string> possibleMoves;
   string distance;
   queue<Game> moveQueue;
   vector<vector<int> > movesAway = vector<vector<int> >(board.getSize(), vector<int>(board.getSize(), INT_MAX));
   movesAway.at(row).at(col) = 0;
   moveQueue.push(board);
   
   while(!moveQueue.empty()){
      board = moveQueue.front();
      moveQueue.pop();
      if(blackPlayerSearch){
         row = board.blackRows.at(0);
         col = board.blackCols.at(0);
      }
      else{
         row = board.redRows.at(0);
         col = board.redCols.at(0);
      }
      possibleMoves = board.stackValidMoves(row, col);
      for(uint i = 0; i < possibleMoves.size(); i++){
         int destinationRow = possibleMoves.at(i).at(0) - '0';
         int destinationCol = possibleMoves.at(i).at(1) - '0';
         if(movesAway.at(destinationRow).at(destinationCol) == INT_MAX){
            Game tempBoard = board;
            tempBoard.makeMove(piece, destinationRow, destinationCol);
            moveQueue.push(tempBoard);
            movesAway.at(destinationRow).at(destinationCol) = movesAway.at(row).at(col) + 1;
         }
      }
   }
   if(blackPlayerSearch){
      blackBFS = movesAway;
   }
   else{
      redBFS = movesAway;
   }
}

double Genetic::squaresReachedFirst(Game board){ 
   breadthFirstSearch(board, true);
   breadthFirstSearch(board, false);
   
   vector<vector<int>> blackMovesAway = blackBFS;
   vector<vector<int>> redMovesAway = redBFS;
   // Player One
   double total = 0.0;
   for(int i = 0; i < board.getSize(); i++){
      for(int j = 0; j < board.getSize(); j++){
         if(blackMovesAway.at(i).at(j) == INT_MAX || redMovesAway.at(i).at(j) == INT_MAX){
            ;
         }
         else if(blackMovesAway.at(i).at(j) == redMovesAway.at(i).at(j)){
            ;
         }
         else if(blackMovesAway.at(i).at(j) < redMovesAway.at(i).at(j)){
            total += 1.0;
         }
         else{
            total -= 1.0;
         }
      }
   }
   return total;
}

double Genetic::squaresReached(Game board){ 
   vector<vector<int>> blackMovesAway = blackBFS;
   vector<vector<int>> redMovesAway = redBFS;
   // Player One
   double total = 0.0;
   for(int i = 0; i < board.getSize(); i++){
      for(int j = 0; j < board.getSize(); j++){
         if(blackMovesAway.at(i).at(j) == INT_MAX || redMovesAway.at(i).at(j) == INT_MAX){
            ;
         }
         else if(blackMovesAway.at(i).at(j) > 0 && redMovesAway.at(i).at(j) > 0){
            total += 0.0;
         }
         else if(blackMovesAway.at(i).at(j) > 0){
            total += 1.0;
         }
         else if(redMovesAway.at(i).at(j) > 0){
            total -= 1.0;
         }   
      }
   }
   return total;
}

double buildInputAndEvaluate(Game game, Network neuralNet){
   vector<double> input;
   Genetic geneTest;
   double evaluation;
   input.push_back(geneTest.territoryOwnership(game));
   if(neuralNet.getBFS()){
     input.push_back(geneTest.squaresReachedFirst(game));
     input.push_back(geneTest.squaresReached(game)); 
   }
   input.push_back(geneTest.closerToPieceChebyshev(game));
   input.push_back(geneTest.closerToPieceEuclidian(game));
   input.push_back(geneTest.closeMoves(game));
   evaluation = neuralNet.evaluate(input);
   //cout << "Evaluation is: " << evaluation << endl;
   return evaluation;
}
int playPopulations(Population pop1, Population pop2){
   int popSize = pop1.getSize();
   int gameBoardSize1 = pop1.getGameBoardSize();
   int gameBoardSize2 = pop2.getGameBoardSize();
   int bestFitnessOne = INT_MIN;
   int secondBestFitnessOne = INT_MIN;
   int bestFitnessTwo = INT_MIN;
   int secondBestFitnessTwo = INT_MIN;
   int numLayers = pop1.getIndividual(0,0).getNumLayers();
   ofstream outputPlayers;
   string fileName;
   string input = "";
   Network bestIndivOne = pop1.getIndividual(0,0);
   Network secondBestIndivOne = pop1.getIndividual(0,0);
   Network bestIndivTwo = pop2.getIndividual(0,0);
   Network secondBestIndivTwo = pop2.getIndividual(0,0);
   Network player1 = pop1.getIndividual(0,0);
   Network player2 = pop2.getIndividual(0,0);
   int fitnessOne = 0;
   int fitnessTwo = 0;
   int winsOne = 0;
   int winsTwo = 0;
   if(gameBoardSize1 != gameBoardSize2){
      cout << "Invalid player, board sizes not same" << endl;
      return 0;
   }
   int result1 = 0;
   int result2 = 0;
   int total = 0;
   int pop1Wins = 0;
   int pop2Wins = 0;
   int count = 0;
   for(int i = 0; i < popSize; i++){
      for(int j = 0; j < popSize; j++){
         player1 = pop1.getIndividual(i,j);
         for(int r = 0; r < popSize; r++){
            for(int c = 0; c < popSize; c++){
               player2 = pop2.getIndividual(r, c);
               result1 = playAGame(&agentMinimax, &agentMinimax, player1, player2, gameBoardSize1);
               if(result1 > 0){
                  pop1.setFitness(result1, i, j);
                  pop2.setFitness(result1 * -1, r, c);
               }
               else{
                  pop1.setFitness(result1, i, j);
                  pop2.setFitness(result1 * -1, r, c);
               }
               if(result1 < 0){
                  pop2.setWin(r, c);
                  pop2Wins += 1;
               }
               else{
                  pop1.setWin(i, j);
                  pop1Wins += 1;
               }
               result2 = playAGame(&agentMinimax, &agentMinimax, player2, player1, gameBoardSize1);
               if(result1 > 0){
                  pop1.setFitness(result2 * -1, i, j);
                  pop2.setFitness(result2, r, c);
               }
               else{
                  pop1.setFitness(result2 * -1, i, j);
                  pop2.setFitness(result2, r, c);
               }
               if(result2 < 0){
                  pop1.setWin(i, j);
                  pop1Wins += 1;
               }
               else{
                  pop2.setWin(r, c);
                  pop2Wins += 1;
               }
               count++;
            }
         }
      }
   }
   cout << "Count: " << count << endl;
   for (int i = 0; i < popSize; i++){
      for (int j = 0; j < popSize; j++){
         winsOne = pop1.getWins(i, j);
         winsTwo = pop2.getWins(i, j);
         cout << "Wins of pop 1, individual " << i << ", " << j << ": " << winsOne << endl;
         cout << "Wins of pop 2, individual " << i << ", " << j << ": " << winsTwo << endl;
         if(fitnessOne > bestFitnessOne){
            bestFitnessOne = fitnessOne;
            bestIndivOne = pop1.getIndividual(i,j);
         }
         else if (fitnessOne > secondBestFitnessOne){
            secondBestFitnessOne = fitnessOne;
            secondBestIndivOne = pop1.getIndividual(i,j);
         }
         if(fitnessTwo > bestFitnessTwo){
            bestFitnessTwo = fitnessTwo;
            bestIndivTwo = pop2.getIndividual(i,j);
         }
         else if(fitnessTwo > secondBestFitnessTwo){
            secondBestFitnessTwo = bestFitnessTwo;
            secondBestIndivTwo = pop2.getIndividual(i,j);
         }
      }
   }
   //Network player1 = pop1.getIndividual(0,0);
   //result1 = playAVisualGame(&playerAgent, &agentMinimax, player1, player1, gameBoardSize1);
   cout << "Player 1 Wins: " << pop1Wins << endl;
   cout << "Player 2 Wins: " << pop2Wins << endl;
   cout << "Best pop 1 fitness: " << bestFitnessOne << endl;
   cout << "Second best pop 1 fitness: " << secondBestFitnessOne << endl;
   cout << "Best pop 2 fitness: " << bestFitnessTwo << endl; 
   cout << "Second best pop 2 fitness: " << secondBestFitnessTwo << endl;
   cout << "Save pop 1 players? ";
   cin >> input;
   cout << endl;
   if(input == "yes"){
      cout << "Name player save: ";
      cin >> fileName;
      cout << endl;
      vector<int> numPerceptPerLayer = pop1.getNumPerceptPerLayer();
      int activationNum = pop1.getActivationNum();
      outputPlayers.open(fileName);
      for(uint i = 0; i < numPerceptPerLayer.size(); i++){
         outputPlayers << numPerceptPerLayer.at(i);
      }
      outputPlayers << endl;
      outputPlayers << numLayers << endl;
      outputPlayers << activationNum << endl;
      outputPlayers << gameBoardSize1 << endl;
      if(bestIndivOne.getBFS()){
         outputPlayers << "true" << endl;
      }
      else{
         outputPlayers << "false" << endl;
      }
      outputPlayers << "Indiv1" << endl;
      vector<vector<vector<double>>> indivWeightsOne = bestIndivOne.getWeights();
      vector<vector<vector<double>>> secondBestindivWeightsOne = secondBestIndivOne.getWeights();
      for(int i = 0; i < numLayers; i++){
         for(int j = 0; j < numPerceptPerLayer.at(i + 1); j++){
            for(int k = 0; k < numPerceptPerLayer.at(i) + 1; k++){
               outputPlayers << indivWeightsOne.at(i).at(j).at(k) << endl;
            }
         }
      }
      outputPlayers << "Indiv2" << endl;
      for(int i = 0; i < numLayers; i++){
         for(int j = 0; j < numPerceptPerLayer.at(i + 1); j++){
            for(int k = 0; k < numPerceptPerLayer.at(i) + 1; k++){
               outputPlayers << secondBestindivWeightsOne.at(i).at(j).at(k) << endl;
            }
         }
      }
      outputPlayers.close();
   }
   cout << "Save pop 2 players? ";
   cin >> input;
   cout << endl;
   if(input == "yes"){
      cout << "Name player save: ";
      cin >> fileName;
      cout << endl;
      vector<int> numPerceptPerLayer = pop2.getNumPerceptPerLayer();
      int activationNum = pop2.getActivationNum();
      outputPlayers.open(fileName);
      for(uint i = 0; i < numPerceptPerLayer.size(); i++){
         outputPlayers << numPerceptPerLayer.at(i);
      }
      outputPlayers << endl;
      outputPlayers << numLayers << endl;
      outputPlayers << activationNum << endl;
      outputPlayers << gameBoardSize1 << endl;
      if(bestIndivTwo.getBFS()){
         outputPlayers << "true" << endl;
      }
      else{
         outputPlayers << "false" << endl;
      }
      outputPlayers << "Indiv1" << endl;
      vector<vector<vector<double>>> indivWeightsTwo = bestIndivTwo.getWeights();
      vector<vector<vector<double>>> secondBestindivWeightsTwo = secondBestIndivTwo.getWeights();
      for(int i = 0; i < numLayers; i++){
         for(int j = 0; j < numPerceptPerLayer.at(i + 1); j++){
            for(int k = 0; k < numPerceptPerLayer.at(i) + 1; k++){
               outputPlayers << indivWeightsTwo.at(i).at(j).at(k) << endl;
            }
         }
      }
      outputPlayers << "Indiv2" << endl;
      for(int i = 0; i < numLayers; i++){
         for(int j = 0; j < numPerceptPerLayer.at(i + 1); j++){
            for(int k = 0; k < numPerceptPerLayer.at(i) + 1; k++){
               outputPlayers << secondBestindivWeightsTwo.at(i).at(j).at(k) << endl;
            }
         }
      }
      outputPlayers.close();
   }
   /*
   Things needed to save individual:
   numLayers - can get
   activationNum - can get
   gameBoardSize - can get
   numPerceptPerLayer - can give
   */
   total = pop1Wins - pop2Wins;
   return total;
}
Population loadPopulation(){
   string fileName;
   string line;
   int generation;
   string generationString;
   string stringEnd;
   vector<int> numPerceptPerLayer;
   int numLayers;
   int populationSize;
   int activationNum;
   int gameBoardSize;
   string BFSstore;
   bool BFS;
   //int fitness = 0;
   //int bestFitness = INT_MIN;
   double weight;
   //bool foundGeneration;
   cout << "Choose population to load: ";
   cin >> fileName;
   cout << endl;
   cout << "Choose generation: ";
   cin >> generation;
   generationString = "generation" + to_string(generation);
   cout << endl;
   ifstream file(fileName);
   getline(file, line);
   for(uint i = 0; i < line.size(); i++){
      numPerceptPerLayer.push_back(line.at(i) - '0');
   }
   file >> numLayers;
   file >> populationSize;
   file >> activationNum;
   file >> gameBoardSize;
   file >> BFSstore;
   if(BFSstore == "true"){
      BFS = true;
   }
   else{
      BFS = false;
   }
   Population pop(populationSize, populationSize, numPerceptPerLayer, activationNum, BFS);
   pop.setGameBoardSize(gameBoardSize);
   pop.setNumPerceptPerLayer(numPerceptPerLayer);
   pop.setActivationNum(activationNum);
   //file.get();
   //cout << "String end: " << stringEnd << endl;
   while (getline(file, line)){
      if(line == generationString){
         //cout << "Found gen string" << endl;
         for(int r = 0; r < populationSize; r++){
            for(int c = 0; c < populationSize; c++){
               //cout << "Made indiv!" << endl;
               Network individual(numPerceptPerLayer, activationNum);
               for(int i = 0; i < numLayers; i++){
                  for(int j = 0; j < numPerceptPerLayer.at(i + 1); j++){
                     for(int k = 0; k < numPerceptPerLayer.at(i) + 1; k++){
                        //cout << "Loaded weight!" << endl;
                        file >> weight;
                        individual.changeOneWeight(weight, i, j, k);
                     }
                  }
               }
               pop.setIndividual(individual, r, c);
            }
         }
         break;
      }
   }
   // Commented section proves that populations are reading in weights correctly
   /*
   for(int i = 0; i < populationSize; i++){
      for(int j = 0; j < populationSize; j++){
         Network bestPlayer = pop.getIndividual(i,j);
         vector<vector<vector<double>>> playerWeights = bestPlayer.getWeights();
         pop.outputAllWeights(playerWeights, numLayers, numPerceptPerLayer);
      }
   }
   */
   //result = playAVisualGame(&playerAgent, &agentMinimax, bestPlayer, bestPlayer, 4);
   return pop;
}
Network loadIndiv(){
   string fileName;
   string line;
   int indiv;
   string generationString;
   string stringEnd;
   vector<int> numPerceptPerLayer;
   int numLayers;
   int activationNum;
   int gameBoardSize;
   string BFSstore;
   bool BFS;
   cout << "Player to load: ";
   cin >> fileName;
   cout << endl;
   cout << "Indiv: ";
   cin >> indiv;
   generationString = "Indiv" + to_string(indiv);
   cout << endl;
   ifstream file(fileName);
   getline(file, line);
   for(uint i = 0; i < line.size(); i++){
      numPerceptPerLayer.push_back(line.at(i) - '0');
   }
   file >> numLayers;
   file >> activationNum;
   file >> gameBoardSize;
   file >> BFSstore;
   if(BFSstore == "true"){
      BFS = true;
   }
   else{
      BFS = false;
   }
   Network individual(numPerceptPerLayer, activationNum);
   individual.setBFS(BFS);
   return individual;
}
void outPutNeuronWeights(){
   string fileName;
   string line;
   int generation;
   string generationString;
   string stringEnd;
   vector<int> numPerceptPerLayer;
   int numLayers;
   int populationSize;
   int activationNum;
   int gameBoardSize;
   string BFSstore;
   bool BFS;
   //int fitness = 0;
   //int bestFitness = INT_MIN;
   double weight;
   //bool foundGeneration;
   cout << "Choose population to load: ";
   cin >> fileName;
   cout << endl;
   cout << "Choose generation: ";
   cin >> generation;
   generationString = "generation" + to_string(generation);
   cout << endl;
   ifstream file(fileName);
   getline(file, line);
   for(uint i = 0; i < line.size(); i++){
      numPerceptPerLayer.push_back(line.at(i) - '0');
   }
   file >> numLayers;
   file >> populationSize;
   file >> activationNum;
   file >> gameBoardSize;
   file >> BFSstore;
   if(BFSstore == "true"){
      BFS = true;
   }
   else{
      BFS = false;
   }
   Population pop(populationSize, populationSize, numPerceptPerLayer, activationNum, BFS);
   pop.setGameBoardSize(gameBoardSize);
   pop.setNumPerceptPerLayer(numPerceptPerLayer);
   //file.get();
   //cout << "String end: " << stringEnd << endl;
   while (getline(file, line)){
      if(line == generationString){
         //cout << "Found gen string" << endl;
         for(int r = 0; r < populationSize; r++){
            for(int c = 0; c < populationSize; c++){
               //cout << "Made indiv!" << endl;
               Network individual(numPerceptPerLayer, activationNum);
               for(int i = 0; i < numLayers; i++){
                  for(int j = 0; j < numPerceptPerLayer.at(i + 1); j++){
                     for(int k = 0; k < numPerceptPerLayer.at(i) + 1; k++){
                        //cout << "Loaded weight!" << endl;
                        file >> weight;
                        individual.changeOneWeight(weight, i, j, k);
                     }
                  }
               }
               pop.setIndividual(individual, r, c);
            }
         }
         break;
      }
   }
   double territoryWeight = 0.0;
   double squaresReachedFirstWeight = 0.0;
   double squaresReachedWeight = 0.0;
   double closerChebyWeight = 0.0;
   double closerEuclidWeight = 0.0;
   double closeMovesWeight = 0.0;
   for(int r = 0; r < populationSize; r++){
      for(int c = 0; c < populationSize; c++){
         //cout << "Made indiv!" << endl;
         for(int i = 0; i < numLayers; i++){
            for(int j = 0; j < numPerceptPerLayer.at(i + 1); j++){
               for(int k = 0; k < numPerceptPerLayer.at(i) + 1; k++){
                  //cout << "Loaded weight!" << endl;
                  switch(k){
                     case 0:
                     break;
                     case 1:
                     territoryWeight += pop.getWeight(r, c, i, j, k);
                     break;
                     case 2:
                     squaresReachedFirstWeight += pop.getWeight(r, c, i, j, k);
                     break;
                     case 3:
                     squaresReachedWeight += pop.getWeight(r, c, i, j, k);
                     break;
                     case 4:
                     closerChebyWeight += pop.getWeight(r, c, i, j, k);
                     break;
                     case 5:
                     closerEuclidWeight += pop.getWeight(r, c, i, j, k);
                     break;
                     case 6:
                     closeMovesWeight += pop.getWeight(r, c, i, j, k);
                     break;
                  }
               }
            }
         }
      }
   }
   cout << "Average territoryWeight: " << territoryWeight / 36.0 << endl;
   cout << "Average squaresReachedFirstWeight: " << squaresReachedFirstWeight / 36.0 << endl;
   cout << "Average squaresReachedWeight: " << squaresReachedWeight / 36.0 << endl;
   cout << "Average closerChebyWeight: " << closerChebyWeight / 36.0 << endl;
   cout << "Average closerEuclidWeight: " << closerEuclidWeight / 36.0 << endl;
   cout << "Average closeMovesWeight: " << closeMovesWeight / 36.0 << endl;
   
}

Network genetic(int populationHeight, int populationWidth, vector<int> numPerceptPerLayer, int activationNum, int gameBoardSize, bool BFS){
   Population pop(populationHeight, populationWidth, numPerceptPerLayer, activationNum, BFS);
   int generation = 0;
   int fitness;
   int count = 0;
   int bestFitness = INT_MIN;
   int powerOfTwo = 2;
   ofstream outputPlayers;
   string file;
   //double weightInput;
   Network bestPlayer(numPerceptPerLayer, activationNum);
   int numLayers = bestPlayer.getNumLayers();
   vector<vector<vector<double>>> indivWeights;
   
   cout << "Name storage file: " << endl;
   cin >> file;
   while(generation <= 1000){
      for (int i = 0; i < populationHeight; i++){
         for (int j = 0; j < populationWidth; j++){
            pop.playNeighbors(i, j, gameBoardSize);
            Network individual = pop.getIndividual(i, j);
            fitness = individual.getFitness();
            //cout << "Fitness of individual " << i << ", " << j << ": " << fitness << endl;
         }
      }
      /*
      // Testing output all weights
      for (int i = 0; i < populationHeight; i++){
         for (int j = 0; j < populationWidth; j++){
            Network father = pop.getIndividual(i, j);
            int numLayers = father.getNumLayers();
            fatherWeights = father.getWeights();
            cout << "Weights before children: " << endl;
            pop.outputAllWeights(fatherWeights, numLayers, numPerceptPerLayer);
         }
      }
      */
      vector<vector<Network>> childStorage = vector<vector<Network>>(populationHeight);
      // Create children from players
      
      for (int i = 0; i < populationHeight; i++){
         for (int j = 0; j < populationWidth; j++){
            Network father = pop.getIndividual(i, j);
            Network mother = pop.selectMate(i, j);
            Network child(father, mother, numPerceptPerLayer);
            childStorage.at(i).push_back(child);
            count++;
         }
      }
      // Replace population with children
      for (int i = 0; i < populationHeight; i++){
         for (int j = 0; j < populationWidth; j++){
            pop.setIndividual(childStorage.at(i).at(j), i, j);
         }
      }
      /*
      // Test to make sure child has weights
      for (int i = 0; i < populationHeight; i++){
         for (int j = 0; j < populationWidth; j++){
            father = pop.getIndividual(i, j);
            int numLayers = father.getNumLayers();
            fatherWeights = father.getWeights();
            pop.outputAllWeights(fatherWeights, numLayers, numPerceptPerLayer);
         }
      }
      */
      generation++;
      cout << generation << endl;
      
      if(generation == 1){
         outputPlayers.open(file);
         for(uint i = 0; i < numPerceptPerLayer.size(); i++){
            outputPlayers << numPerceptPerLayer.at(i);
         }
         outputPlayers << endl;
         outputPlayers << numLayers << endl;
         outputPlayers << populationHeight << endl;
         outputPlayers << activationNum << endl;
         outputPlayers << gameBoardSize << endl;
         if(BFS){
            outputPlayers << "true" << endl;
         }
         else{
            outputPlayers << "false" << endl;
         }
         
         outputPlayers << "generation" << generation << endl;
         
         for(int i = 0; i < populationHeight; i++){
            for(int j = 0; j < populationWidth; j++){
               Network outputIndiv = pop.getIndividual(i, j);
               indivWeights = outputIndiv.getWeights();
               for(int i = 0; i < numLayers; i++){
                  for(int j = 0; j < numPerceptPerLayer.at(i + 1); j++){
                     for(int k = 0; k < numPerceptPerLayer.at(i) + 1; k++){
                        outputPlayers << indivWeights.at(i).at(j).at(k) << endl;
                     }
                  }
               }
            }
         }
         
      }
      else if(generation % powerOfTwo == 0){
         outputPlayers << "generation" << generation << endl;
         for(int i = 0; i < populationHeight; i++){
            for(int j = 0; j < populationWidth; j++){
               Network outputIndiv = pop.getIndividual(i, j);
               indivWeights = outputIndiv.getWeights();
               for(int i = 0; i < numLayers; i++){
                  for(int j = 0; j < numPerceptPerLayer.at(i + 1); j++){
                     for(int k = 0; k < numPerceptPerLayer.at(i) + 1; k++){
                        outputPlayers << indivWeights.at(i).at(j).at(k) << endl;
                     }
                  }
               }
            }
         }
         if (powerOfTwo == 64){
            powerOfTwo = 100;
         }
         else if(powerOfTwo < 64){
            powerOfTwo = powerOfTwo * 2;
         }
         /*
         if(powerOfTwo == 1024){
            powerOfTwo = 1000;
         }
         else if(powerOfTwo < 1024){
            powerOfTwo = powerOfTwo * 2;
         }
         */
      }
   }
   outputPlayers << "generation" << generation << endl;
   outputPlayers.close();
   // Select and return #1 best player
   for (int i = 0; i < populationHeight; i++){
         for (int j = 0; j < populationWidth; j++){
            fitness = (pop.getIndividual(i,j)).getFitness();
            if(fitness > bestFitness){
               bestFitness = fitness;
               bestPlayer = pop.getIndividual(i,j);
            }
         }
   }
   
   return bestPlayer;
   /*
   pop.playNeighbors(0, 0, gameBoardSize);
   Network father = pop.getIndividual(0, 0);
   Network mother = pop.selectMate(0, 0);
   Network child(father, mother, numPerceptPerLayer);
   pop.setIndividual(child, 0, 0);
   */
   /*
   vector<vector<vector<double>>> fatherWeights;
   vector<vector<vector<double>>> motherWeights;
   vector<double> inputs{2.0};
   vector<double> weightsIn{.5, 1.5};
   vector<double> weightsOut{.5, 1.5, .5};
   Network father = pop.getIndividual(0, 0);
   Network mother = pop.getIndividual(0, 1);
   int numLayers = father.getNumLayers();
   int instruction;
   double networkEvaluation;
   //cout << "Number of layers: " << numLayers << endl;
   //cout << "Output of numPerceptPerLayer: " << endl;
   //cout << endl;
   fatherWeights = father.getWeights();
   motherWeights = mother.getWeights();
   cout << "All father weights: " << endl;
   pop.outputAllWeights(fatherWeights, numLayers, numPerceptPerLayer);
   cout << "All mother weights: " << endl;
   pop.outputAllWeights(motherWeights, numLayers, numPerceptPerLayer);
   Network child(father, mother, numPerceptPerLayer);
   for(int i = 0; i < numLayers; i++){
      for(int j = 0; j < numPerceptPerLayer.at(i + 1); j++){
         if(i == numLayers - 1){
            father.changeWeights(weightsOut, i, j);
         }
         else{
            father.changeWeights(weightsIn, i, j);
         }
      }    
   }
   networkEvaluation = father.evaluate(inputs);
   cout << "Evaluation of Network: " << networkEvaluation << endl;
   //Network fittest;
   //Network secondFittest;
   */
   // Board states to inputs ideas: 
}
