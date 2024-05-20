#include "PuzzleGenerator.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

const int MAX_ITERATIONS_WITHOUT_IMPROVEMENT = 1000; // Adjust as needed


PuzzleGenerator::PuzzleGenerator(int _nRows, int _nColumns, int _minVal, int _maxVal)
    : nRows(_nRows), nColumns(_nColumns), minVal(_minVal), maxVal(_maxVal)
{}

Puzzle PuzzleGenerator::GeneratePuzzleUsingRandomWalk() {
	Puzzle bestPuzzleFound = RandomWalk(4);

	while (timer.GetElapsedTime() + 4 < maxtime) {
		Puzzle p = RandomWalk(4);
		if (p.GetValue() > bestPuzzleFound.GetValue()) {
			bestPuzzleFound = p;
		}
	}

	return bestPuzzleFound;
}

Puzzle PuzzleGenerator::GeneratePuzzleUsingHillClimb() {
    Puzzle bestPuzzleFound = RandomWalk(4);
    int bestValueFound = bestPuzzleFound.GetValue();
    int iterationsSinceImprovement = 0; // Initialize count

    while (timer.GetElapsedTime() < maxtime) {
        Puzzle currentPuzzle = HillClimb();
        int currentValue = currentPuzzle.GetValue();
        if (currentValue > bestValueFound) {
            bestPuzzleFound = currentPuzzle;
            bestValueFound = currentValue;
            iterationsSinceImprovement = 0; // Reset count
        } else {
            iterationsSinceImprovement++; // Increment count
            if (iterationsSinceImprovement >= MAX_ITERATIONS_WITHOUT_IMPROVEMENT) {
                currentPuzzle = RandomWalk(4); // Trigger restart
                bestValueFound = currentPuzzle.GetValue();
                iterationsSinceImprovement = 0; // Reset count
            }
        }
    }

    return bestPuzzleFound;
}

Puzzle PuzzleGenerator::GeneratePuzzle() {
    timer.StartTimer();
    maxtime = 59.9; // To make sure we don't exceed a minute

    if(min(nRows, nColumns) > 20) {
        return GeneratePuzzleUsingRandomWalk();
    }
    return GeneratePuzzleUsingHillClimb();
}

Puzzle PuzzleGenerator::RandomWalk(double timelimit) {
    Puzzle p(nRows, nColumns, minVal, maxVal);

    Puzzle bestPuzzle = p;
    int bestValue = p.GetValue();

    Timer t;
    t.StartTimer();

    // Loop until we hit the time limit.
    while (t.GetElapsedTime() < timelimit - 0.1) { // To make sure that we don't exceed the time limit, we stop just before we hit the time limit.
        // Generate a successor of p by randomly changing the value of a random cell
        // (since we are doing a random walk, we just replace p with its successor).
        p = p.GetRandomSuccessor();

        // Update the current best solution.
        if (p.GetValue() > bestValue) { // Calling GetValue() for the first time is costly
                                        // since the puzzle has to be evaluated first.
            bestValue = p.GetValue(); // Calling it a second time simply returns the value that was computed before.
            bestPuzzle = p;
        }
    }

    return bestPuzzle;
}

Puzzle PuzzleGenerator::HillClimb() {
    Puzzle bestPuzzle(nRows, nColumns, minVal, maxVal);
    int bestValue = 0;

    // Set initial temperature and cooling rate
    double initialTemperature = 500.0;  // Decreased initial temperature
    double coolingRate = 0.995;         // Adjusted cooling rate

    while (timer.GetElapsedTime() < maxtime) {
        Puzzle currentPuzzle(nRows, nColumns, minVal, maxVal);

        Puzzle currentBestPuzzle = currentPuzzle;
        int currentBestValue = currentPuzzle.GetValue();

        double temperature = initialTemperature;

        // Hill-climbing with simulated annealing loop
        while (temperature > 1e-3 && timer.GetElapsedTime() < maxtime) {  // Added time limit check
            bool improvement = false;
            vector<Puzzle> successors;
            currentPuzzle.GetAllSuccessors(successors);

            // Filtered sampling strategy
            size_t sampleSize = min(successors.size(), static_cast<size_t>(sqrt(successors.size())));
            vector<Puzzle> sampledSuccessors;
            for (size_t i = 0; i < sampleSize; ++i) {
                int randIndex = rand() % successors.size();
                sampledSuccessors.push_back(successors[randIndex]);
            }

            // Evaluate each sampled successor
            for (auto &successor : sampledSuccessors) {
                int successorValue = successor.GetValue();
                int delta = successorValue - currentBestValue;

                if (delta > 0 || exp(delta / temperature) > ((double)rand() / RAND_MAX)) {
                    currentBestValue = successorValue;
                    currentBestPuzzle = successor;
                    improvement = true;
                }
            }

            // Update current puzzle if improvement is found
            if (improvement) {
                currentPuzzle = currentBestPuzzle;
            }

            temperature *= coolingRate;
        }

        if (currentBestValue > bestValue) {
            bestValue = currentBestValue;
            bestPuzzle = currentBestPuzzle;
        }
    }

    return bestPuzzle;
}
