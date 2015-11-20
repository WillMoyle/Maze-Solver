// MAZE SOLVER
// Author: Will Moyle
// Last modified: 09/01/15
// Function header file

////////////////////////////////////////////////////////////////////////////////////
// PRE-SUPPLIED FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////


/* You are pre-supplied with the functions below. Add your own
   function prototypes to the end of this file. */

/* helper functions to allocate and deallocate dynamic 2D arrays */
char **allocate_2D_array(int rows, int columns);
void deallocate_2D_array(char **m, int rows);

/* pre-supplied function to load a maze from a file */
char **load_maze(const char *filename, int &height, int &width);

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width);

////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS WRITTEN BY WILL MOYLE
////////////////////////////////////////////////////////////////////////////////////

/* returns true if the character ch is found in maze and sets row 
   and column to the appropriate coordiates (or -1 if not found) */
bool find_marker(char ch, char **maze, int height, int width, int &row, int &column);


/* returns true if the string directions is a valid sequence of N, S,
   E or W */
bool valid_string(char *directions);

/* converts a direction NSEW into an integer change for x and y */
void direction_translation(char compass, int &x, int &y);

/* returns true if the space is available to move into and sets end 
   to true if the space is the 'X' */
bool space_available(char **maze, int row, int column, bool &end, char endchar);

/* returns true if path is a valid solution to maze */
bool valid_solution(char *path, char **maze, int height, int width);

/* generates a valid next move given previously attempted moves in list
   'moves' returns false if no moves are available*/
bool generate_next_move(char &next_move, char *moves, char **maze, int height, int width, 
			int row, int column, bool &end, char endchar);


/* generates a path from a given starting position */
bool generate_path(char *path, char **maze, int height, int width,
		   int &row, int &column, char end);

/* finds a path through the maze, returning the directions as a string */
char *find_path(char **maze, int height, int width, char start, char end);

/* changes all occurences of character in maze to ' ' */
void reset_map(char **maze, int height, int width, char character);
