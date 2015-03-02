using System;
using System.IO;
using System.Text;


public class tile
{
  string desc;
  int index;

  public void set_desc (string desc)
  {
    this.desc = desc;
    this.index = 0;
  }

  public tile (string desc)
  {
    set_desc (desc);
  }

  public tile ()
  {
    set_desc (" ");
  }

  public char get_tile ()
  {
    return desc[index];
  }

  public void hit ()
  {
    if (this.index < desc.Length)
      this.index++;
  }
}


public class breakout
{
  public const int max_row = 25;
  public const int max_col = 15;
  private tile[,] grid;

  
  public char get_tile (int row, int col)
  {
    return grid[row, col].get_tile ();
  }

  public void hit (int row, int col)
  {
    grid[row, col].hit ();
  }

  public breakout ()
  {
    System.Console.WriteLine("inside breakout constructor");
    grid = new tile[max_row,max_col];

    for (int j = 0; j<max_row; j++)
      for (int i = 0; i<max_col; i++) {
	grid[j, i] = new tile ();
      }
  }

  //
  //  process_line - 
  //

  private void process_line (string line, int row)
  {
    if (line != null) {
      int col = 0;
      int i = 0;
      string desc;

      while ((i<line.Length) && (line[i] == '[')) {
	desc = "";
	i++;
	while (line[i] != ']') {
	  desc += line[i];
	  i++;
	}
	grid[row, col] = new tile (desc);
	col++;
	i++;
      }
    }
  }


  //
  //  read_file - pre-condition : filename exists and is in the correct format.
  //              post-condition: breakout data structures are initialised and
  //                              if successful level is returned else 0 is returned.
  //
  
  private int read_file (string filename, int level)
  {
    int row = 0;
    FileInfo f = new FileInfo (filename);
    StreamReader r = f.OpenText ();
    string line;

    do {
      line = r.ReadLine ();
      process_line (line, row);
      row++;
    } while (line != null);

    while (row < max_row) {
      for (int col = 0; col < max_col; col++) {
	grid[row, col] = new tile ();
      }

      row++;
    }
    return level;
  }


  //
  //  load_level - pre-condition:  none.
  //               post-condition: loads a level, initialising the
  //                               scene and level is returned.
  //                               If level does not exist 0 is returned.

  public int load_level (int level)
  {
    StringBuilder filename = new StringBuilder ();
    string f;
    
    filename.AppendFormat ("level{0}", level);
    f = filename.ToString ();
    if (File.Exists (f))
      return read_file (f, level);
    else
      return 0;
  }
}
