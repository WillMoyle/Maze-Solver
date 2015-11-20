#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a maze */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a maze from a file*/
char **load_maze(const char *filename, int &height, int &width) {

  bool success = get_maze_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }
  
  return m;
}

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

/* returns true if the character ch is found in maze and sets row 
   and column to the appropriate coordiates (or -1 if not found) */
bool find_marker(char ch, char **maze, int height, int width, int &row, int &column)
{
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      {
	if (maze[i][j] == ch)
	  {
	    row = i;
	    column = j;
	    return true;
	  }
      }

  row = -1;
  column = -1;
  return false;
}


/* returns true if the string directions is a valid sequence of N, S,
   E or W */
bool valid_string(char *directions)
{
  int length = strlen(directions);

  for (int i = 0; i < length; i++)
    {
      if (directions[i] != 'N'
	  && directions[i] != 'S'
	  && directions[i] != 'E'
	  && directions[i] != 'W')
	{
	  return false;
	}
    }

  return true;
}


/* converts a direction NSEW into an integer change for x and y */
void direction_translation(char compass, int &x, int &y)
{
  switch (compass) {
  case 'N':
    x = 0;
    y = -1;
    break;
  case 'S':
    x = 0;
    y = 1;
    break;
  case 'E':
    x = 1;
    y = 0;
    break;
  case 'W':
    x = -1;
    y = 0;
    break;
  default:
    break;
  }
}

/* returns true if the space is available to move into and sets end 
   to true if the space is the 'X' */
bool space_available(char **maze, int row, int column, bool &end, char endchar)
{
  char destination = maze[row][column];

  if (destination == endchar)
    {
      end = true;
      return true;
    }
  else if (destination == ' ' || destination == '>')
    {
      end = false;
      return true;
    }
  else
    {
      end = false;
      return false;
    }
}


/* returns true if path is a valid solution to maze */
bool valid_solution(char *path, char **maze, int height, int width)
{
  if (!valid_string(path))
    {
      //cout << "\nERROR: Invalid string\n";
      return false;
    }

  int current_row, current_column, x_move(0), y_move(0), next_row, next_column;
  int length = strlen(path);
  bool end(false);

  if (!find_marker('>', maze, height, width, current_row, current_column))
    {
      //cout << "\nERROR: Unable to locate start position\n";
      return false;
    }
  
  for (int i = 0; i < length; i++)
    {
      //cout << "\nCurrent position: row " << current_row << " column " << current_column << ".";

      next_row = current_row;
      next_column = current_column;
      direction_translation(path[i], x_move, y_move);
      next_row += y_move;
      next_column += x_move;

      if(next_row > height || next_row < 0
	 || next_column > width || next_column < 0)
	{
	  //cout << "\nERROR: Out of bounds\n";
	  return false;
	}

      if(!space_available(maze, next_row, next_column, end, 'X'))
	{
	  //cout << "\nERROR: The route is blocked by (";
	  //cout << next_column << "," << next_row << ")\n";
	  return false;
	}
      
      current_row = next_row;
      current_column = next_column;


      if (end && i == (length-1))
	{
	  //cout << "\nEND REACHED - CONGRATULATIONS\n";
	  return true;
	}
    }

  cout << "\nERROR: Path doesn't terminate at the end\n";
  return false;
}


/* generates a valid next move given previously attempted moves in list
   'moves' returns false if no moves are available*/
bool generate_next_move(char &next_move, char *moves, char **maze, int height, int width, 
			int row, int column, bool &end, char endchar)
{
  end = false;
  int next_row(row), next_column(column);

  if(!strcmp(moves, ""))
    {
      next_move = 'N';
      next_row -= 1;
      if (!space_available(maze, next_row, next_column, end, endchar))
	{
	  strcpy(moves, "N");
	  if(!generate_next_move(next_move, moves, maze, height, width, row, column, end, endchar))
	    return false;
	  else
	    return true;
	}
      else
	{
	  return true;
	}
    }
  if(!strcmp(moves, "N"))
    {
      next_move = 'S';
      next_row += 1;
      if (!space_available(maze, next_row, next_column, end, endchar))
	{
	  strcpy(moves, "NS");
	  if(!generate_next_move(next_move, moves, maze, height, width, row, column, end, endchar))
	    return false;
	  else
	    return true;
	}
      else
	{
	  return true;
	}
    }
  if(!strcmp(moves, "NS"))
    {
      next_move = 'E';
      next_column += 1;
      if (!space_available(maze, next_row, next_column, end, endchar))
	{
	  strcpy(moves, "NSE");
	  if(!generate_next_move(next_move, moves, maze, height, width, row, column, end, endchar))
	    return false;
	  else
	    return true;
	}
      else
	{
	  return true;
	}
    }
  if(!strcmp(moves, "NSE"))
    {
      next_move = 'W';
      next_column -= 1;
      if (!space_available(maze, next_row, next_column, end, endchar))
	{
	  return false;
	}
      else
	{
	  return true;
	}
    }

  return true;
}


/* generates a path from a given starting position */
bool generate_path(char *path, char **maze, int height, int width, int &row, int &column, char end)
{
  int current_row(row), current_column(column);
  int row_move(0), column_move(0);

  bool possible_move, ended(false), next_step;
  char next_move;
  char moves[5];
  int moves_taken = strlen(path);

  strcpy(moves, "");

  possible_move = generate_next_move(next_move, moves, maze, 
				     height, width, current_row, current_column, ended, end);
    
  if (possible_move)
    {
      path[moves_taken] = next_move;
      path[moves_taken+1] = '\0';
      direction_translation(next_move, column_move, row_move);
      row = current_row + row_move;
      column = current_column + column_move;

      if (ended)
	{
	  return true;
	}
      else
	{
	  maze[row][column] = '#';
	  next_step = generate_path(path, maze, height, width, row, column, end);
	}
    }
    
  else
    {
      if (moves_taken == 0)
	return false;

      char previous_move = path[moves_taken-1];
	  
      switch (previous_move)
	{
	case 'N':
	  column += 1;
	  break;
	case 'S':
	  column -= 1;
	  break;
	case 'E':
	  row -= 1;
	  break;
	case 'W':
	  row += 1;
	  break;
	}
      path[moves_taken - 1] = '\0';

      next_step = generate_path(path, maze, height, width, row, column, end);
    }
  
  return next_step;
}

/* changes all occurences of character in maze to ' ' */
void reset_map(char **maze, int height, int width, char character)
{
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      {
	if (maze[i][j] == character)
	  maze[i][j] = ' ';
      }
}

/* finds a path through the maze, returning the directions as a string */
char *find_path(char **maze, int height, int width, char start, char end)
{
  int start_row, start_column;
  char *path = new char[30];
  path[0] = '\0';

  if(!find_marker(start, maze, height, width, start_row, start_column))
    strcpy(path,"no solution");

  if(!generate_path(path, maze, height, width, start_row, start_column, end))
    {
      reset_map(maze, height, width, '#');
      strcpy(path,"no solution");
    }
  
  else
    reset_map(maze, height, width, '#');
  
  return path;
    
      
}



