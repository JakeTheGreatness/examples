using System;
using System.Threading;
using System.IO;
using System.Text;


public struct ghost
{
  public char name;
  public int initx, inity;
  public int curx, cury;
}

public class game
{
  const int initial_delay = 400;
  const int minimum_delay = 100;
  const int nGhosts = 4;
  static int level = 1;
  static int score = 0;
  static char [,] grid = new char [80, 40];
  static int current_delay = initial_delay;
  static int px, py, food, maxx, maxy;
  static ghost [] ghosts = new ghost [nGhosts];

  public static void delay ()
  {
    Thread.Sleep (current_delay);
    if (current_delay > minimum_delay)
      current_delay -= 1;
  }

  public static bool GetChar (ref char ch)
  {
    if (Console.KeyAvailable)
      {
        ConsoleKeyInfo k = Console.ReadKey ();

	if (k.Key == ConsoleKey.Escape)
	  Environment.Exit(0);

        ch = k.KeyChar;
        return true;
      }
    return false;
  }

  public static bool GetCharTimeout (ref char ch)
  {
    for (int i = current_delay; i > 0; i -= 40)
      {
	if (GetChar (ref ch))
	  return true;
	else
	  Thread.Sleep (40);
      }
    return false;
  }

  public static int max (int a, int b)
  {
    if (a > b)
      return a;
    return b;
  }

  public static void blank_grid ()
  {
    for (int col = 0; col < grid.GetLength (0); col++)
      for (int row = 0; row < grid.GetLength (1); row++)
	grid [col, row] = ' ';
    for (int g = 0; g < nGhosts; g++)
      {
	ghosts[g].name = ' ';
	ghosts[g].initx = -1;
	ghosts[g].inity = -1;
	ghosts[g].curx = -1;
	ghosts[g].cury = -1;
      }
    food = 0;
    maxx = -1;
    maxx = -1;
    current_delay = initial_delay;
  }

  public static void process_line (string line, int row)
  {
    int no;

    Console.WriteLine (line);
    maxx = max (line.Length, maxx);
    maxy = max (row, maxy);
    for (int col = 0; col < line.Length; col++) {
      grid[col, row] = line[col];
      switch (line[col]) {
      case 'p':
	px = col;
	py = row;
	break;
      case '1':
      case '2':
      case '3':
      case '4':
	no = Convert.ToInt32 (line[col]) - Convert.ToInt32 ('1');
	ghosts[no].name = line[col];
	ghosts[no].initx = col;
	ghosts[no].inity = row;
	ghosts[no].curx = col;
	ghosts[no].cury = row;
	break;
      case '#':
	break;
      case 'w':
	break;
      case '.':
	food += 1;
	break;
      case ' ':
	break;
      default:
	grid[col, row] = ' ';
	break;
      }
    }
  }

  public static void writePos (int x, int y)
  {
    ConsoleColor old;

    switch (grid[x, y]) {
    case ' ':
      Console.Write (' ');
      break;
    case '1':
      old = Console.ForegroundColor;
      Console.ForegroundColor = ConsoleColor.Cyan;
      Console.Write ('o');
      Console.ForegroundColor = old;
      break;
    case '2':
      old = Console.ForegroundColor;
      Console.ForegroundColor = ConsoleColor.Blue;
      Console.Write ('o');
      Console.ForegroundColor = old;
      break;
    case '3':
      old = Console.ForegroundColor;
      Console.ForegroundColor = ConsoleColor.Green;
      Console.Write ('o');
      Console.ForegroundColor = old;
      break;
    case '4':
      old = Console.ForegroundColor;
      Console.ForegroundColor = ConsoleColor.Magenta;
      Console.Write ('o');
      Console.ForegroundColor = old;
      break;
    case 'p':
      old = Console.ForegroundColor;
      Console.ForegroundColor = ConsoleColor.Yellow;
      Console.Write ('o');
      Console.ForegroundColor = old;
      break;
    case '.':
      old = Console.ForegroundColor;
      Console.ForegroundColor = ConsoleColor.Blue;
      Console.Write ('.');
      Console.ForegroundColor = old;
      break;
    case 'w':
      old = Console.ForegroundColor;
      Console.ForegroundColor = ConsoleColor.Red;
      Console.Write ('o');
      Console.ForegroundColor = old;
      break;
    case '#':
      old = Console.BackgroundColor;
      Console.BackgroundColor = ConsoleColor.White;
      Console.Write (' ');
      Console.BackgroundColor = old;
      break;
    default:
      Console.Write (' ');
      break;
    }
  }

  public static void redraw ()
  {
    Console.SetCursorPosition (0, 0);
    Console.WriteLine ("Score: {0}    Level: {1}    Delay: {2}",
		       score, level, current_delay);
    for (int y = 0; y <= maxy; y++)
      {
	for (int x = 0; x < maxx; x++)
	  writePos (x, y);
	Console.WriteLine ("");
      }
  }

  public static bool in_bound_dim (int d, int r)
  {
    return (r >= 0) && (r < grid.GetLength (d));
  }

  public static bool in_bounds (int x, int y)
  {
    return in_bound_dim (0, x) && in_bound_dim (1, y);
  }

  public static void move (int x, int y)
  {
    if (in_bounds (px+x, py+y))
      {
	grid[px, py] = ' ';
	x = px+x;
	y = py+y;
	switch (grid[x, y]) {
	case '#':
	  // cannot move onto a wall
	  break;
	case '1':
	case '2':
	case '3':
	case '4':
	  // moved into monster, suicide
	  px = x;
	  py = y;
	  break;
	case ' ':
	  // moved into space
	  px = x;
	  py = y;
	  break;
	case '.':
	  // gathered food
	  food -= 1;
	  grid[x, y] = ' ';
	  px = x;
	  py = y;
	  break;
	case 'w':
	  // cannot move onto well
	  break;
	default:
	  break;
	}
	grid[px, py] = 'p';
      }
  }

  public static void process_input (char ch)
  {
    switch (ch) {
    case 'a':
      move (-1, 0);  // left
      break;
    case 'd':
      move (1, 0);   // right
      break;
    case 'w':
      move (0, -1);  // up
      break;
    case 's':
      move (0, 1);   // down
      break;
    case 'x':
      move (0, 1);   // down
      break;
    }
  }

  public static bool gathered_food ()
  {
    return food == 0;
  }

  public static bool monster_captured_player ()
  {
    for (int g = 0; g < nGhosts; g++)
      if (ghosts[g].curx == px && ghosts[g].cury == py)
	return true;
    return false;
  }

  public static bool level_finished ()
  {
    return gathered_food () || monster_captured_player ();
  }

  public static bool can_move_onto (int x, int y)
  {
    return (grid[x, y] == ' ') || (grid[x, y] == '.') || (grid[x, y] == 'w') || (grid[x, y] == 'p');
  }

  public static bool move_onto (int g, int x, int y)
  {
    Console.WriteLine ("g = {0}, x = {1}, y = {2}", g, x, y);
    char old = grid[x, y];

    if (old == 'p')
      return false;   // caught player, game over.

    grid[x, y] = grid[ghosts[g].curx, ghosts[g].cury]; // update grid
    grid[ghosts[g].curx, ghosts[g].cury] = old;
    ghosts[g].curx = x;
    ghosts[g].cury = y;

    return true;
  }

  public static void pause ()
  {
    Console.ReadKey ();
  }

  public static bool should_move_x (int g)
  {
    Console.WriteLine ("should_move_x  g = {0}, x = {1}, y = {2}",
		       g, ghosts[g].curx, ghosts[g].cury);
    if (ghosts[g].curx < px && can_move_onto (ghosts[g].curx+1, ghosts[g].cury))
      return move_onto (g, ghosts[g].curx+1, ghosts[g].cury);
    if (ghosts[g].curx > px && can_move_onto (ghosts[g].curx-1, ghosts[g].cury))
      return move_onto (g, ghosts[g].curx-1, ghosts[g].cury);

    return false;
  }

  public static bool should_move_y (int g)
  {
    Console.WriteLine ("should_move_y  g = {0}, x = {1}, y = {2}",
		       g, ghosts[g].curx, ghosts[g].cury);
    if (ghosts[g].cury < py && can_move_onto (ghosts[g].curx, ghosts[g].cury+1))
      return move_onto (g, ghosts[g].curx, ghosts[g].cury+1);
    if (ghosts[g].cury > py && can_move_onto (ghosts[g].curx, ghosts[g].cury-1))
      return move_onto (g, ghosts[g].curx, ghosts[g].cury-1);

    return false;
  }

  /*
   *  move_ghost_1 - prioritise x movement.
   */

  public static bool move_ghost_1 (int g)
  {
    if (should_move_x (g))
      return true;
    if (should_move_y (g))
      return true;
    return false;
  }

  /*
   *  move_ghost_2 - prioritise x movement.
   */

  public static bool move_ghost_2 (int g)
  {
    if (should_move_y (g))
      return true;
    if (should_move_x (g))
      return true;
    return false;
  }

  /*
   *  move_ghost - move ghost, g.
   *               returns true if ghost eats player.
   */

  public static bool move_ghost (int g)
  {
    switch (ghosts[g].name) {
    case '1':
      return move_ghost_1 (g);
    case '2':
      return move_ghost_2 (g);
    case '3':
      return move_ghost_1 (g);
    case '4':
      return move_ghost_2 (g);
    }
    return false;
  }

  /*
   *  move_ghosts - attempts to move all ghosts.
   */

  public static void move_ghosts ()
  {
    for (int g = 0; g < nGhosts; g++)
      move_ghost (g);
  }

  public static bool completed_level ()
  {
    Console.Clear ();
    while (! level_finished ())
      {
	char ch = 'z';

	redraw ();
	if (GetCharTimeout (ref ch))
	  process_input (ch);
	else
	  move_ghosts ();
      }
    return gathered_food ();
  }

  public static bool read_file (string filename, int no)
  {
    int row = 0;
    FileInfo f = new FileInfo (filename);
    StreamReader r = f.OpenText ();
    string line;

    blank_grid ();

    line = r.ReadLine ();
    while (line != null)
      {
	process_line (line, row);
	row++;
	line = r.ReadLine ();
      }
    Console.WriteLine ("finished reading file");

    return true;
  }

  public static bool load_level (int level)
  {
    StringBuilder filename = new StringBuilder ();
    string f;

    filename.AppendFormat ("level{0}", level);
    f = filename.ToString ();
    if (File.Exists (f))
      return read_file (f, level);
    else
      return false;
  }

  public static bool completed_all ()
  {
    while (load_level (level))
      {
	if (! completed_level ())
	  return false;
	level += 1;
      }
    return true;
  }

  public static void Main ()
  {
    if (completed_all ())
      Console.WriteLine ("well done, you have completed all levels and your score was {0}", score);
    else
      Console.WriteLine ("you died with a score of {0}", score);
  }
}
