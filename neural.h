// Nathan Karr
// Honors Research
// Game of Amazons

#ifndef NEURAL_H
#define NEURAL_H

#include <iostream>
#include <array>
#include <algorithm>
#include <stack>
#include <vector>
#include <cmath>
#include <string>
#include <random>
#include "amazon.h"

using namespace std;
//https://natureofcode.com/book/chapter-10-neural-networks/
class Perceptron{
   private:
   int numInput;
   int activationNum;
   bool isInput;
   //static default_random_engine generator;
   //static normal_distribution<double> PerceptronDistribution;
   static constexpr int MIN = -1;
   static constexpr int MAX = 1;
   double c = 0.01;
   vector<double> weights;
   public:
   // could make this only constructor and generate random weights for newWeights
   Perceptron(int numWeights, int activationNum);
   Perceptron(vector<double> newWeights, int activationNum);
   void changeWeights(vector<double> newWeights);
   void changeOneWeight(double wieght, int k);
   vector<double> getWeights();
   double getWeight(int k){return weights.at(k);}
   double evaluate(vector<double> inputs);
   double sigmoid(double sum);
   double tanh(double sum);
   double ReLu(double sum);
   double softPlus(double sum);
   /*
   void train(vector<double> inputs, int desired){
      int guess = feedForward(inputs);
      double error = desired - guess;
      for(int i = 0; i < numInput; i++){
         weights.at(i) += c * error * inputs[i];
      }
   }
   */
};
class Layer{
   private:
   int numPercept;
   vector<Perceptron> perceptrons;
   public:
   Layer(int numPercep, int activationNum, int numWeights);
   void changeWeights(vector<double> weights, int instruction);
   void changeOneWeight(double wieght, int j, int k);
   double getWeight(int j, int k){return perceptrons.at(j).getWeight(k);}
   vector<vector<double>> getWeights();
   vector<double> evaluate(vector<double> input);
};
class Network{
   private:
   vector<Layer> neuralNet;
   int fitness = 0;
   int wins = 0;
   int numLayers;
   bool BFS = true;
   public:
   Network(vector<int> numPerceptPerLayer, int activationNum);
   Network(Network father, Network mother, vector<int> numPerceptPerLayer);
   vector<vector<vector<double>>> getWeights();
   void changeWeights(vector<double> weights, int layer, int instruction);
   void changeOneWeight(double wieght, int i, int j, int k);
   int getNumLayers(){return neuralNet.size();}
   double evaluate(vector<double> input);
   void setWin(){wins += 1;}
   int getWins(){return wins;}
   void setFitness(int givenFitness){fitness += givenFitness;}
   int getFitness(){return fitness;}
   double getWeight(int i, int j, int k){return neuralNet.at(i).getWeight(j, k);}
   void setBFS(bool use){BFS = use;}
   bool getBFS(){return BFS;}
   vector<Layer> getNeuralNet(){return neuralNet;}
   //int getFitness(return fitness);
};
class Population{
   private:
   vector<Network> fittestIndividuals;
   vector<vector<Network>> population;
   vector<int> numPerceptPerLayer;
   int populationHeight;
   int populationWidth;
   int gameBoardSize;
   int activationNum;
   //Network fittest;
   int generation = 0;
   public:
   Population(int populationHeight, int populationWidth, vector<int> numPerceptPerLayer, int activationNum, bool BFS);
   Network getIndividual(int row, int col);
   Network selectMate(int row, int col);
   void outputAllWeights(vector<vector<vector<double>>> individualWeights, int numLayers, vector<int> numPerceptPerLayer);
   void playNeighbors(int row, int col, int boardSize);
   void setFitness(int givenFitness, int row, int col){population.at(row).at(col).setFitness(givenFitness);}
   void setWin(int row, int col){population.at(row).at(col).setWin();}
   int getIndivFitness(int row, int col){return population.at(row).at(col).getFitness();}
   int getWins(int row, int col){return population.at(row).at(col).getWins();}
   void handleFitness(int givenFitness, Network &blackPlayer, Network &redPlayer);
   void setIndividual(Network child, int row, int col);
   void setNumPerceptPerLayer(vector<int> given){numPerceptPerLayer = given;}
   vector<int> getNumPerceptPerLayer(){return numPerceptPerLayer;}
   int getSize(){return populationHeight;}
   int getGameBoardSize(){return gameBoardSize;}
   double getWeight(int r, int c, int i, int j, int k){return population.at(r).at(c).getWeight(i, j, k);}
   void setActivationNum(int given){activationNum = given;}
   int getActivationNum(){return activationNum;}
   void setGameBoardSize(int size){gameBoardSize = size;}
   void selection();
   void mutation();
   //Network getFittest(return fittest);
};
class Genetic{ //Class to handle board checks by the genetic algorithm
   private:
   vector<vector<int>> blackBFS;
   vector<vector<int>> redBFS;
   public:
   void breadthFirstSearch(Game board, bool blackPlayerSearch);
   double squaresReachedFirst(Game board);
   double squaresReached(Game board);
   double territoryOwnership(Game board);
   double closeMoves(Game board);
   double closerToPieceChebyshev(Game board);
   double closerToPieceEuclidian(Game board);
};
class Game;
Population loadPopulation();
int playPopulations(Population pop1, Population pop2);
double buildInputAndEvaluate(Game game, Network neuralNet);
Network genetic(int populationHeight, int populationWidth, vector<int> numPerceptPerLayer, int activationNum, int gameBoardSize, bool BFS);
void outPutNeuronWeights();
Network loadIndiv();

/*
class Trainer{
   public:
   vector<double> inputs;
   int answer;
   
   Trainer(double x, double y, int a){
      inputs.push_back(x);
      inputs.push_back(y);
      inputs.push_back(1);
      answer = a;
   }
};
*/
#endif