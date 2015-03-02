using Gtk;
using Gdk;
using System;
using Cairo;
using System.Collections.Generic;

enum moving_types { none_t, ball_t, paddle_t, paddle2_t };
enum game_state { start_t, new_level_t, play_t, finish_t };


class breakout_definitions
{
  public const int x_boarder       = 10;
  public const int y_boarder       = 10;
  public const int x_res           = 640;
  public const int y_res           = 480;
  public const int x_win           = x_res + x_boarder;
  public const int y_win           = y_res + y_boarder;
  public const int paddle_width    = x_res / 20;
  public const int paddle2_width   = paddle_width * 3/2;
  public const int paddle2_timeout = 100;
  public const int y_paddle        = y_res-30;
  public const int paddle_height   = 10;

  public static Cairo.Color get_color (string colorDesc)
  {
    switch (colorDesc) {

    case "red"      :  return new Cairo.Color (1, 0, 0);
    case "yellow"   :  return new Cairo.Color (0, 1, 1);
    case "green"    :  return new Cairo.Color (0, 1, 0);
    case "blue"     :  return new Cairo.Color (0, 0, 1);
    case "darkgrey" :  return new Cairo.Color (0.8, 0.8, 0.8);
    case "lightgrey":  return new Cairo.Color (0.95, 0.95, 0.95);
    case "darkred"  :  return new Cairo.Color (0.65, 0.1, 0.2);
    case "darkgreen":  return new Cairo.Color (0.1, 0.4, 0.2);
    case "darkblue" :  return new Cairo.Color (0.1, 0.2, 0.6);
    case "white"    :  return new Cairo.Color (1, 1, 1);
    case "black"    :  return new Cairo.Color (0, 0, 0);

    default:
      return new Cairo.Color (0, 0, 0);
    }
  }
}


class moving
{
  public int x;
  public int y;
  public int vx;
  public int vy;
  public int radius;
  private moving_types moving_type;
  private int timeout;


  public void ball (int initx, int inity, int velx, int vely, int rad)
  {
    moving_type = moving_types.ball_t;
    x = initx;
    y = inity;
    vx = velx;
    vy = vely;
    radius = rad;
    timeout = 0;
  }


  public void draw_ball (Context context)
  {
    context.Save ();
    context.Arc (x, y, radius, 0, 2*Math.PI);
    context.Color = breakout_definitions.get_color ("blue");
    context.StrokePreserve ();
    context.Fill ();
    context.SetSourceRGB (1, 1, 1);
    context.Restore ();
  }


  public void draw_paddle (Context context)
  {
    context.Save ();
    context.Color = breakout_definitions.get_color ("darkred");
    context.Rectangle (x, y,
		       breakout_definitions.paddle_width,
		       breakout_definitions.paddle_height);
    context.StrokePreserve ();
    context.Fill ();
    context.SetSourceRGB (1, 1, 1);
    context.Restore ();
  }


  public void draw_paddle2 (Context context)
  {
  }


  public void draw (Context context)
  {
    switch (moving_type) {

    case moving_types.none_t   : break;
    case moving_types.ball_t   : draw_ball (context); break;
    case moving_types.paddle_t : draw_paddle (context); break;
    case moving_types.paddle2_t: draw_paddle2 (context); break;

    }
  }


  public void erase_ball (Context context)
  {
    context.Save ();
    context.Color = breakout_definitions.get_color ("white");
    context.Rectangle (x-radius, y-radius, radius*2, radius*2);
    context.StrokePreserve ();
    context.Fill ();
    context.SetSourceRGB (1, 1, 1);
    context.Restore ();
  }


  public void erase_paddle (Context context)
  {
    context.Save ();
    context.Color = breakout_definitions.get_color ("white");
    context.Rectangle (x, y,
		       breakout_definitions.paddle_width,
		       breakout_definitions.paddle_height);
    context.StrokePreserve ();
    context.Fill ();
    context.SetSourceRGB (1, 1, 1);
    context.Restore ();
  }


  public void erase_paddle2 (Context context)
  {
  }


  public void erase (Context context)
  {
    switch (moving_type) {

    case moving_types.none_t   : break;
    case moving_types.ball_t   : erase_ball (context); break;
    case moving_types.paddle_t : erase_paddle (context); break;
    case moving_types.paddle2_t: erase_paddle2 (context); break;

    }
  }

  private void draw_if_moved (Context context, int nx, int ny)
  {
    if ((nx != x) || (ny != y)) {
      erase (context);
      x = nx;
      y = ny;
      draw (context);
    }
  }


  private bool hits_paddle0 (moving paddle, int nx, int ny, ref int vx, ref int vy)
  {
    int paddle_boarder = 2;

    // hits top left corner
    if ((nx == paddle.x) && (ny == paddle.y+breakout_definitions.paddle_height)) {
      vx = -vx;
      vy = -vy;
      return true;
    }

    // hits top right corner
    if ((nx == paddle.x+breakout_definitions.paddle_width) && (ny == paddle.y+breakout_definitions.paddle_height)) {
      vx = -vx;
      vy = -vy;
      return true;
    }

    // hits top
    if ((ny == paddle.y-paddle_boarder) &&
	((nx >= paddle.x) && (nx <= paddle.x+breakout_definitions.paddle_width))) {
      vy = -vy;
      return true;
    }

    // hits left or right
    if ((in_range (ny, paddle.y, paddle.y+breakout_definitions.paddle_height)) &&
	((nx == paddle.x) || (nx == paddle.x+breakout_definitions.paddle_width))) {
      vx = -vx;
      vy = -vy;
      return true;
    }
    return false;
  }


  private bool hits_paddle (moving paddle, int nx, int ny, ref int vx, ref int vy)
  {
    return (hits_paddle0 (paddle, nx-radius, ny-radius, ref vx, ref vy) ||
	    hits_paddle0 (paddle, nx+radius, ny-radius, ref vx, ref vy) ||
	    hits_paddle0 (paddle, nx-radius, ny+radius, ref vx, ref vy) ||
	    hits_paddle0 (paddle, nx+radius, ny+radius, ref vx, ref vy));
  }


  private bool hits_wall (int nx, int ny, ref int vx, ref int vy)
  {
    if ((nx == radius) || (nx == breakout_definitions.x_res - radius)) {
      vx = -vx;
      return true;
    }
    else if (ny == radius) {
      vy = -vy;
      return true;
    }
    return false;
  }


  private bool in_range (int a, int b, int c)
  {
    return ((a >= b) && (a <= c));
  }


  private bool hits_brick0 (Context context, int nx, int ny,
			    ref int vx, ref int vy, breakout b, gamewindow g)
  {
    int r = (ny * breakout.max_row) / breakout_definitions.y_res;
    int c = (nx * breakout.max_col) / breakout_definitions.x_res;

    if ((r < 0) || (c < 0) || (r >= breakout.max_row) || (c >= breakout.max_col))
      return false;

    if (b.get_tile (r, c) == ' ')
      return false;  // no brick present
    
    int bx0   = (breakout_definitions.x_res * c)/breakout.max_col;
    int by0   = (breakout_definitions.y_res * r)/breakout.max_row;
    int width = (breakout_definitions.x_res/breakout.max_col);
    int height= (breakout_definitions.y_res/breakout.max_row);

    
    if (in_range (nx, bx0, bx0+width) && ((ny == by0) || (ny == by0+height))) {
      // hit top of brick or bottom of brick
      vy = -vy;
      g.hit_brick (r, c);
      return true;
    }
    if (in_range (ny, by0, by0+height) && ((nx == bx0) || (nx == bx0+width))) {
      // hit left or right of brick
      vx = -vx;
      g.hit_brick (r, c);
      return true;
    }

    return false;
  }
  


  private bool hits_brick (Context context, int nx, int ny,
			   ref int vx, ref int vy, breakout b, gamewindow g)
  {
    return (hits_brick0 (context, nx-radius, ny-radius, ref vx, ref vy, b, g) ||
	    hits_brick0 (context, nx-radius, ny+radius, ref vx, ref vy, b, g) ||
	    hits_brick0 (context, nx+radius, ny-radius, ref vx, ref vy, b, g) ||
	    hits_brick0 (context, nx+radius, ny+radius, ref vx, ref vy, b, g));
  }


  private bool falls_below (Context context, int ny)
  {
    if (ny == breakout_definitions.y_res - 3*radius) {
      erase (context);
      moving_type = moving_types.none_t;
      return true;
    }
    return false;
  }


  public void move (Context context, moving paddle, breakout b, gamewindow g)
  {
    if ((vx != 0) || (vy != 0)) {
      int nx = Math.Min (Math.Max (x + vx, 0), breakout_definitions.x_res);
      int ny = Math.Min (Math.Max (y + vy, 0), breakout_definitions.y_res);
      
      if (moving_type == moving_types.ball_t) {
	if (hits_paddle (paddle, nx, ny, ref vx, ref vy))
	  ;
	else if (hits_brick (context, nx, ny, ref vx, ref vy, b, g))
	  ;
	else if (hits_wall (nx, ny, ref vx, ref vy))
	  ;
	else if (falls_below (context, ny))
	  ;
	else
	  draw_if_moved (context, nx, ny);
      }
      else
	draw_if_moved (context, nx, ny);
    }
  }

  public void to_paddle2 ()
  {
    if (moving_type == moving_types.paddle_t) {
      moving_type = moving_types.paddle2_t;
      timeout = breakout_definitions.paddle2_timeout;
      
    }
  }

  public void paddle (int initx, int inity)
  {
    moving_type = moving_types.paddle_t;
    x = initx;
    y = inity;
    vx = 0;
    vy = 0;
    radius = 0;
    timeout = 0;
  }

  public moving ()
  {
    moving_type = moving_types.none_t;
  }

  public bool is_none_t ()
  {
    return moving_type == moving_types.none_t;
  }

  public void default_paddle ()
  {
    paddle (breakout_definitions.paddle_width, breakout_definitions.y_paddle-breakout_definitions.paddle_height);
  }

  public void default_ball ()
  {
    ball (breakout_definitions.paddle_width, breakout_definitions.y_paddle/2, -1, -1, 8);
  }

  public void paddle_velocity (int v)
  {
    if (moving_type == moving_types.paddle_t || moving_type == moving_types.paddle2_t)
      vx = v;
  }
}

class gamewindow : DrawingArea
{
  private Gtk.Window win = null;
  private breakout b;
  Context context;
  DrawingArea area;
  List<moving> move_list = new List<moving>();
  game_state current_state = game_state.start_t ;
  int current_level = 1;


  public gamewindow ()
  {
    System.Console.WriteLine("inside gamewindow");
    win = new Gtk.Window ("Breakout");

    win.SetDefaultSize (breakout_definitions.x_win, breakout_definitions.y_win);
    win.SetPosition (WindowPosition.Center);
    DeleteEvent += delegate { Application.Quit(); };
    area = new DrawingArea ();
    area.AddEvents ((int) (EventMask.ButtonPressMask
			   |EventMask.ButtonReleaseMask));
    area.ExposeEvent += OnDrawingAreaExposed;
    area.KeyPressEvent += new KeyPressEventHandler (key_handler);
    area.ButtonPressEvent += new ButtonPressEventHandler (ButtonPressHandler);
    area.ButtonReleaseEvent += new ButtonReleaseEventHandler (ButtonReleaseHandler);

    MenuBar mb = new MenuBar ();
    Menu file_menu = new Menu ();
    MenuItem file = new MenuItem ("File");
    file.Submenu = file_menu;
    
    MenuItem run = new MenuItem ("Run");
    run.Activated += do_run;
    file_menu.Append (run);
    
    MenuItem exit = new MenuItem ("Exit");
    exit.Activated += do_exit;
    file_menu.Append (exit);
    mb.Append (file);

    VBox vbox = new VBox (false, 3);
    vbox.PackStart (mb, false, false, 0);

    HBox hbox = new HBox();
    hbox.Add (area);
    vbox.Add (hbox);

    System.Console.WriteLine("before win.Add");
    win.Add (vbox);
    System.Console.WriteLine("before win.ShowAll");

    win.ShowAll ();
    System.Console.WriteLine("before context");
    context = Gdk.CairoHelper.Create(area.GdkWindow);

    System.Console.WriteLine("finish gamewindow");
  }


  private void erase_brick (int row, int col)
  {
    int x0 = (breakout_definitions.x_res * col)/breakout.max_col;
    int y0 = (breakout_definitions.y_res * row)/breakout.max_row;
    int xd = (breakout_definitions.x_res/breakout.max_col);
    int yd = (breakout_definitions.y_res/breakout.max_row);

    context.Save ();
    context.Color = breakout_definitions.get_color ("white");
    context.Rectangle (x0, y0, xd, yd);
    context.StrokePreserve ();
    context.Fill ();
    context.SetSourceRGB (1, 1, 1);
    context.Restore ();
  }


  private void draw_colour (int row, int col, string colorDesc)
  {
    int x0 = (breakout_definitions.x_res * col)/breakout.max_col+2;
    int y0 = (breakout_definitions.y_res * row)/breakout.max_row+2;
    int xd = (breakout_definitions.x_res/breakout.max_col)-4;
    int yd = (breakout_definitions.y_res/breakout.max_row)-4;

    context.Save ();
    context.Color = breakout_definitions.get_color (colorDesc);
    context.Rectangle (x0, y0, xd, yd);
    context.StrokePreserve ();
    context.Fill ();
    context.SetSourceRGB (1, 1, 1);
    context.Restore ();
  }


  private void draw_brick (int row, int col)
  {
    switch (b.get_tile (row, col)) {

    case ' ':  erase_brick (row, col); break;
    case '#':  draw_colour (row, col, "red");  break;
    case '=':  draw_colour (row, col, "yellow");  break;
    case '-':  draw_colour (row, col, "green");  break;
    case '<':  draw_colour (row, col, "darkgrey");  break;
    case '>':  draw_colour (row, col, "lightgrey");  break;

    }
  }


  public void check_new_ball (int row, int col)
  {
    int n = 1;
    char ch = b.get_tile (row, col);

    if ((ch >= '0') && (ch <= '9')) {
      n = (int) (ch-'0');
      b.hit (row, col);
      ch = b.get_tile (row, col);
    }
    if (ch == 'o') {
      int x = (breakout_definitions.x_res * col) / breakout.max_col;
      int y = (breakout_definitions.y_res * row) / breakout.max_row;

      while (n > 0) {
	System.Console.WriteLine("new ball {0}, x = {1}, y = {2}", n, x, y);
	if (n > 0) {
	  moving m = new moving ();

	  m.ball (x, y, -1, -1, 8);
	  move_list.Add (m);
	}
	if (n > 1) {
	  moving m = new moving ();

	  m.ball (x+10, y, 1, -1, 8);
	  move_list.Add (m);
	}	  
	if (n > 2) {
	  moving m = new moving ();

	  m.ball (x, y+10, -1, 1, 8);
	  move_list.Add (m);
	}	  
	if (n > 3) {
	  moving m = new moving ();

	  m.ball (x+10, y+10, 1, 1, 8);
	  move_list.Add (m);
	}	  
	n -= 4;
      }
      b.hit (row, col);
    }
  }

  public void hit_brick (int row, int col)
  {
    erase_brick (row, col);
    b.hit (row, col);
    check_new_ball (row, col);
    draw_brick (row, col);
  }


  private void redraw_screen ()
  {
    for (int row = 0; row < breakout.max_row; row++)
      for (int col = 0; col < breakout.max_col; col++)
	draw_brick (row, col);
  }

  void OnDrawingAreaExposed(object source, ExposeEventArgs args)
  {
    // context = Gdk.CairoHelper.Create(area.GdkWindow);
    // redraw_screen ();
  }


  void start_timer ()
  {
    GLib.Timeout.Add (8, new GLib.TimeoutHandler (handle_timer));
  }


  void init_board ()
  {
    moving p = new moving ();
    moving b = new moving ();

    p.default_paddle ();
    b.default_ball ();
    
    move_list.Clear ();
    move_list.Add (p); // first item in the list is always the paddle
    move_list.Add (b);

    redraw_screen ();
    p.draw (context);
  }
  

  void do_start ()
  {
    current_level = 3;
    b = new breakout ();
    System.Console.WriteLine("inside main");
    current_level = b.load_level (current_level);
    if (current_level == 0)
      current_state = game_state.finish_t;
    else {
      current_state = game_state.play_t;
      init_board ();
    }
  }


  private void check_game_over ()
  {
    int lost;
    int i;

    do {
      i = 0;
      lost = 0;
      foreach (moving m in move_list)
	if (m.is_none_t ())
	  lost = i;
      i++;
      if (lost != 0)
	move_list.RemoveAt (lost);
    } while (lost != 0);
    if (move_list.Count == 1)
      current_state = game_state.finish_t;
  }


  void do_play ()
  {
    foreach (moving m in move_list)
      m.move (context, move_list[0], b, this);

    check_game_over ();
  }
  

  // handle_timer - returns true to indicate that we want the timeout to occur again
 
  bool handle_timer ()
  {
    switch (current_state) {

    case game_state.start_t:
      do_start ();
      return true;
    case game_state.new_level_t:
      // do_next_level ();
      return true;
    case game_state.play_t:
      do_play ();
      return true;
    case game_state.finish_t:
      return false;
    default:
      return true;
    }
  }

  public void key_handler (object source, KeyPressEventArgs args)
  {
    // handle keyboard press
    System.Console.WriteLine("key pressed");
  }


  private void ButtonPressHandler (object obj, ButtonPressEventArgs args)
  {
    if (args.Event.Type == EventType.ButtonPress) {
      // the left button was used
      if (args.Event.Button == 1)
	move_list[0].paddle_velocity (-2);
      // the right button was used
      else if (args.Event.Button == 3)
	move_list[0].paddle_velocity (2);
    }
  }


  private void ButtonReleaseHandler (object obj, ButtonReleaseEventArgs args)
  {
    if (args.Event.Type == EventType.ButtonRelease) {
      // the left button was used
      if (args.Event.Button == 1)
	move_list[0].paddle_velocity (0);
      // the right button was used
      else if (args.Event.Button == 3)
	move_list[0].paddle_velocity (0);
    }
  }

  protected virtual void OnTableKeyPress (object o, Gtk.KeyPressEventArgs args)
  {
    System.Console.WriteLine("key pressed");
    if ( args.Event.Key == Gdk.Key.Alt_L ) {
      System.Console.WriteLine("key pressed");
    }
  }

  void do_run (object sender, EventArgs args)
  {
    current_state = game_state.start_t;
    start_timer ();
  }

  void do_exit (object sender, EventArgs args)
  {
    Application.Quit();
  }

  //
  //  Main - pre-condition : none.
  //         post-condition: plays the game and tidies up before finishing.
  //
  
  public static void Main ()
  {
    Application.Init();
    new gamewindow ();
    Application.Run();
  }
}
