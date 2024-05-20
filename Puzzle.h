#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <climits>
#include <iostream>
#include <cassert>

class pCell {
public:
	pCell(int _val = 1)
		:val (_val)
	{}

	int val;
	
	std::vector<int> succ;
	std::vector<int> pred;
	
	int gVal;
	
	bool uniquePath;
	bool reachable;
	bool reaching;
};

class Puzzle{
public:
	Puzzle(int nRows, int nColumns, int minVal, int maxVal);
	
	int GetValue();
	bool HasSolution();
	
	Puzzle GetRandomSuccessor();
	void GetAllSuccessors(std::vector<Puzzle> & successors);
	
	void Print(bool printStatistics = true);

private:
	void Randomize();
	void GenerateEdges();
	void AddEdge(int r1, int c1, int r2, int c2);
	
	void SetCellValue(int cell, int value);
	
	void Evaluate();
	void ComputeValue();
	void ForwardSearch();
	void BackwardSearch();
	
	std::vector<pCell> cells;
	int nRows, nColumns, minVal, maxVal, pSize;
	
	bool hasSolution, hasUniqueSolution;
	bool evaluated;
	int solutionLength, nWhiteHoles, nBlackHoles, nForcedForwardMoves, nForcedBackwardMoves, nReachableCells, nReachingCells;
	int value;
};
