# sudoku

Solves a sudoku puzzle.

Usage:

  ./a.out [args]
  
  Arguments consist in numbers separated by a single whitespace. First number is the sudoku size. Sudokus are normally of size **9** (because they are 9 by 9).
  All other numbers are sudoku's numbers (0 for unknown).
  
  Example (for a sudoku of size 4 (4x4)):
  
-------------
| 4 0 | 0 0 |
| 3 0 | 2 0 |
-------------
| 0 0 | 0 2 |
| 0 0 | 0 0 |
-------------

./a.out 4 4 0 0 0 3 0 2 0 0 0 0 2 0 0 0 0
        ^
        |
        |____ sudoku size
       
       
Output:

-------------
| 4 2 | 1 3 |
| 3 1 | 2 4 |
-------------
| 1 3 | 4 2 |
| 2 4 | 3 1 |
-------------
