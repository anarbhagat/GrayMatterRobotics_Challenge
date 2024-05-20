# Local Search

## Overview

You will use local search methods (such as hill-climbing and simulated annealing) to generate puzzles that are optimized to be challenging and entertaining. We provide you with code that generates and evaluates the puzzles (with respect to some value function that we describe below), but you will have to  implement the local search part that finds puzzles with high values. Your program needs to terminate within 1 minute of wall clock time and report the best puzzle it has found.

## The Puzzle

The puzzle consists of a grid with *r* rows and *c* columns of cells. Each cell contains exactly one integer in the range from *i* to *j* (inclusive), where *i* and *j* are positive integers. The play of the puzzle has to start in the upper-left cell (= the start cell) and move with the smallest number of actions to the lower-right cell (= the goal cell).

If the player is in a cell that contains integer *x*, then they can perform oine of at most four actions, namely either move *x* cells to the left (L), *x* cells to the right (R), *x* cells up (U), or *x* cells down (D), provided that they do not leave the grid. An example puzzle of size 5x5 is given below:

3 2 1 4 1 <br />
3 2 1 3 3 <br />
3 3 2 1 4 <br />
3 1 2 3 3 <br />
1 4 4 3 G <br />

The shortest solution for the instance above is 19 moves: R D L R U D R L R L U D L D R R U D D.

## The Value Function for the Puzzle

Below are some of the features that are expected from a 'good' puzzle that is both challenging and entertaining. We list these features just to give you an idea of the kind of puzzles that we are trying to generate. You are not expected to program anything related to these features. The code that we provide takes care of that.
- The puzzle has a solution
- The puzzle has a unique shortest solution
- The puzzle contains as few *black holes* (dead ends) as possible. Define a *reachable cell* as a cell that can be reached from the start with a sequence of actions. Define a *reaching cell* as a cell from which the goal can be reached with a sequence of actions. A cell is a black hole if and only if it is a reachable, non-reaching cell.
- The puzzle contains as few *white holes* as possible. A cell is a white hole if and only if it is a reaching, non-reachable cell.
- The puzzle contains as few *forced forward moves* as possible. A forced forward move occurs when there is only one action that leaves a reachable cell.
- The puzzle contains as few *forced backward moves* as possible. A forced backward move occurs when there is only one action that reaches a reaching cell.

Using these features, we develop the following value function to evaluate a given puzzle:
- We multiply the length of a shortest solution by 5.
- We add *r* x *c* (*r* = number of rows, *c* = number of columns) points if there is a unique shortest solution.
- We subtract 2 points for each white or black hole.
- We subtract 2 poiunts for each forced forward or backward move.
- We subtract *r* x *c* x 100 points if the puzzle does not have a solution

## Generating Good Puzzles

We provide you with code that generates puzzles (but not necessarily good ones). The program reads the parameters *r*, *c*, *i*, and *j* from the command line and outputs a single puzzle of size *r* x *c* where each cell contains an integer between *i* and *j* (inclusive). Running 'make' compiles and runs the code using the parameters `5 5 1 4`, which should generate an output similar to the one give below. To run the program with different parameters, you can modify the makefile to change the default parameters, or execute the program directly from the command line. 

`./PuzzleGenerator 5 5 1 4` <br />
`Generating a 5x5 puzzle with values in range [1-4]` <br />

`Puzzle:` <br />
`3 2 1 4 2` <br />
`3 4 1 2 1` <br />
`4 3 2 2 1` <br />
`3 2 1 3 4` <br />
`1 4 2 3 0` <br />
<br />
`Solution: Yes` <br />
`Unique: Yes` <br />
`Solution length: 14` <br />
`# of black holes: 1` <br />
`# of white holes: 3` <br />
`# of forced forward moves: 5` <br />
`# of forced backward moves: 9` <br />
`Puzzle value: 59` <br />
<br />
`Total time: 4.901224 seconds` <br />
<br />
The provided code has three classes. The `Timer` class is used to measure the wall clock time in seconds. The `Puzzle` class has a simple interface for generating random puzzles, generating a random successor or all possible successors of a puzzle (by changing the value of a single cell), and calculating the value of a puzzle. The `PuzzleGenerator` class utilizes the `Puzzle` class to generate 'good' puzzles. You will be modifying the `GeneratePuzzle()` member function of the `PuzzleGenerator` class to generate a 'good' puzzle using local search. We have already implemented a member function for `PuzzleGenerator`, called `RandomWalk`, for generating better than average puzzles. You should look at the source code of `RandomWalk()` to understand how to use the `Timer` and `Puzzle` classes. You are free to add new member functions and variables to the `PuzzleGenerator` class, but you should not modify any other source files.

## Hints
- You are free to implement whichever local search algorithm you like. Note that a crossover function is not provided in the puzzle, so genetic algorithms might not be a good idea.
- Implementing a simple hill climbing algorithm would be pretty easy and it would get you some points (and generate decent puzzles when the puzzle size is small) but it is likely that it won't find very high quality puzzles (especially for larger puzzles, where the number of successors at teach iteration can be very large). Nevertheless, it might be a good starting point.
- Your program has plenty of time to generate a good puzzle. Try including random restarts (and storing the best solution you have found so far), to try and find higher quality puzzles. If your current search for a good puzzle does not seem promising, you might want to consider terminating it early to save some time.
- You might not want to evaluate all the successors of a state. Hill climbing considers all the successors (for a 10x10 puzzle with cell values between 1-9, there are around 800 successors), which takes a lot of time to process. Simulated annealing randomly picks one and decides whether to move to that one. You can also 'sample' your successors by randomly generating a small number of them and picking the best one. It is also possible to change your successor selection strategy dynamically as the search progresses.
- A randomly generated puzzle might not be solvable. In this case, its score will be extremely low (you can also check if a puzzle is solvable using the `HasSolution` member of the `Puzzle` class). You can either discard it and try to generate a puzzle with a solution, or you can start searching. Even if the puzzle is unsolvable, its score can get higher with respect to the other quality metrics, and, once you reach a configuratoin that is solvable, its score will jump up.
- Most of the hints that we have listed above are about reminding you of the different things that you can do. It is up to you to mix and match the different techniques and test them out to create a powerful local search algorithm for generating high-quality puzzles.

## Evaluation
- You are not allowed to use any external libraries for your code
- You may develop your code on any platform that you choose, although we have not tested if our code works on non-Linux operating systems
- We will test your code on a Linux machine that supports C++11
- We will test your program with several benchmarks using different parameters. You can assume that *r* and *c* will be between 5 and 100 (inclusive), and *i* and *j* will be between 1 and 100 (inclusive). 
- Puzzles that do not have a solution or that exceed the time limit (1 minute) or that generate a negative puzzle value will not be considered.
