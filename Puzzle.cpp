#include "Puzzle.h"

Puzzle::Puzzle(int _nRows, int _nColumns, int _minVal, int _maxVal)
	:nRows(_nRows), nColumns(_nColumns), minVal(_minVal), maxVal(_maxVal), evaluated(false)
{
	pSize = nRows*nColumns;
	cells.resize(pSize);
	Randomize();
}

int Puzzle::GetValue()
{
	if (!evaluated)
		Evaluate();
	
	return value;
}

bool Puzzle::HasSolution()
{
	if (!evaluated)
		Evaluate();
	
	return hasSolution;
}

Puzzle Puzzle::GetRandomSuccessor()
{
	Puzzle p(*this);
	
	if (minVal == maxVal)	// Can't change anything
		return p;
		
	// Pick a random cell
	int i = rand()%(pSize-1);	// Don't modify the goal
	
	// Randomly change its value
	int newVal = cells[i].val;
	while (newVal == cells[i].val)
		newVal = rand()%(maxVal-minVal+1)+minVal;
	
	p.SetCellValue(i, newVal);
	
	return p;
}

void Puzzle::GetAllSuccessors(std::vector<Puzzle> & successors)
{
	successors.clear();
	Puzzle p(*this);
	for (int i = 0; i < pSize - 1; i++)	// Do not modify the goal!
	{
		for (int v = minVal; v <= maxVal; v++)
		{
			if (cells[i].val != v)
			{
				successors.push_back(Puzzle(*this));
				successors.back().SetCellValue(i, v);
			}
		}
	}
}

void Puzzle::Randomize()
{
	// Randomly initialize the puzzle
	for (int i = 0; i < pSize; i++)
	{
		cells[i].val = rand()%(maxVal-minVal+1)+minVal;
	}
	
	// Set the goal to be 0
	cells[pSize-1].val = 0;	
	
	evaluated = false;
}

void Puzzle::SetCellValue(int cell, int val)
{
	cells[cell].val = val;
	evaluated = false;	// Puzzle has changed, it needs to be evaluated again.
}

void Puzzle::Print(bool printStatistics)
{
	if (!evaluated)
		Evaluate();

	printf("\nPuzzle:\n");
	int i = 0;
	for (int r = 0; r < nRows; r++)
	{
		for (int c = 0; c < nColumns; c++)
		{
			printf("%u ", cells[i].val);
			i++;
		}
		printf("\n");
	}
	
	printf("\n");
	
	if(printStatistics)
	{
		if (hasSolution)
			printf("Solution: Yes\n");
		else
			printf("Solution: No\n");
			
		if (hasUniqueSolution)
			printf("Unique: Yes\n");
		else
			printf("Unique: No\n");
			
		printf("Solution length: %u\n", solutionLength);			
		printf("# of black holes: %u\n", nBlackHoles);
		printf("# of white holes: %u\n", nWhiteHoles);
		printf("# of forced forward moves: %u\n", nForcedForwardMoves);
		printf("# of forced backward moves: %u\n", nForcedBackwardMoves);
		printf("Puzzle value: %d\n\n", value);
	}
}

// Puzzle evaluation functions.
void Puzzle::Evaluate()
{
	//assert(!evaluated);	// This function is private, and should only be called by the GetValue() function if the puzzle has not been evaluated before.

	GenerateEdges();	// The searches will need those edges.
	
	nBlackHoles = 0;
	nWhiteHoles = 0;
	nForcedForwardMoves = 0;
	nForcedBackwardMoves = 0;
	nReachableCells = 0;
	nReachingCells = 0;

	for (int i = 0; i < pSize; i++){
		cells[i].reachable = 0;	// Assume that it is not reachable
		cells[i].reaching = 0;	// Assume that it is not reaching
		cells[i].uniquePath = 1;	// Assume that it has a unique path from the start
		cells[i].gVal = pSize;	// Initialize the gVals to be infinite. pSize is sufficient, since gVal can't be larger than the number of cells
	}

	// Perform a forward search to figure out the 'reachable' and 'uniquePath' flags for cells, and the solution cost
	ForwardSearch();
	
	hasSolution = cells[pSize-1].reachable;
	solutionLength = cells[pSize-1].gVal;
	hasUniqueSolution = (hasSolution && cells[pSize-1].uniquePath);
	
	// Perform a backward search to figure out the 'reaching' flags for cells
	BackwardSearch();
	
	for (int i = 1; i < pSize-1; i++){
		if (cells[i].reachable)
		{
			nReachableCells++;
			if (!cells[i].reaching)
				nBlackHoles++;
				
			if (cells[i].succ.size() == 1)
				nForcedForwardMoves++;
		}
		
		if (cells[i].reaching)
		{
			nReachingCells++;
			if (!cells[i].reachable)
				nWhiteHoles++;
				
			if (cells[i].pred.size() == 1)
				nForcedBackwardMoves++;
		}
	}
	
	if (cells[0].succ.size() == 1)
		nForcedForwardMoves++;
	if (cells[pSize-1].pred.size() == 1)
		nForcedBackwardMoves++;
		
	ComputeValue();
	
	evaluated = true;
}

void Puzzle::ComputeValue()
{
	value = 0;
	if (!hasSolution)
		value -= pSize*100;
		
	if (hasUniqueSolution)
		value += pSize;
		
	value += solutionLength*5;
	value -= 2*(nBlackHoles + nWhiteHoles + nForcedForwardMoves + nForcedBackwardMoves);
}

void Puzzle::GenerateEdges()
{
	for (int i = 0; i < pSize; i++)
	{
		cells[i].succ.clear();
		cells[i].pred.clear();
	}
	
	for (int i = 0; i < pSize-1; i++)	// Exclude the goal
	{
		int r = i/nColumns;	
		int c = i%nColumns;
		int x = cells[i].val;
		
		if (r + x < nRows)	// Downward move
			AddEdge(r, c, r+x, c);
			
		if (r >= x)	// Upward move
			AddEdge(r, c, r-x, c);

		if (c + x < nColumns)	// Right move
			AddEdge(r, c, r, c+x);
			
		if (c >= x)	// Left move
			AddEdge(r, c, r, c-x);
	}
}

void Puzzle::AddEdge(int r1, int c1, int r2, int c2)
{	
	int from = r1*nColumns + c1;
	int to = r2*nColumns + c2;
	
	cells[from].succ.push_back(to);
	cells[to].pred.push_back(from);
}

void Puzzle::ForwardSearch()
{
	std::queue<int> open;
	cells[0].gVal = 0;
	open.push(0);
	cells[0].reachable = 1;
	
	while(!open.empty())
	{
		int s = open.front();
		open.pop();
				
		// Expand cell s
		for (int i = 0; i < cells[s].succ.size(); i++)
		{
			int t = cells[s].succ[i];
			
			// Search is reaching this cell for the first time, since this is BFS with unit-cost edges, it is guaranteed to be the shortest path
			if (!cells[t].reachable)
			{
				open.push(t);
				cells[t].reachable = 1;
				cells[t].gVal = cells[s].gVal + 1;
				
				// If s has a unique shortest path from the start, as far as we know, t also has a unique shortest path from the start
				// Otherwise, t definitely does not have a unique shortest path from the start
				cells[t].uniquePath = cells[s].uniquePath;	
			}
			
			// If t is already reached by another path, and the path to reach it via s is the same, then t does not have a unique shortest path from the start
			else if (cells[t].gVal == cells[s].gVal + 1)
				cells[t].uniquePath = 0;
		}
	}
}

void Puzzle::BackwardSearch()
{
	std::queue<int> open;
	cells[pSize-1].gVal = 0;
	open.push(pSize-1);
	cells[pSize-1].reaching = 1;
	
	while(!open.empty())
	{
		int s = open.front();
		open.pop();
				
		// Expand cell s
		for (int i = 0; i < cells[s].pred.size(); i++)
		{
			int t = cells[s].pred[i];
			
			// Search is reaching this cell for the first time, since this is BFS with unit-cost edges, it is guaranteed to be the shortest path
			if (!cells[t].reaching)
			{
				open.push(t);
				cells[t].reaching = 1;
			}
		}
	}
}
