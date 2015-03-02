using System;
using Gdk;
using Gtk;
using Cairo;
using System.Collections.Generic;


class invaders: DrawingArea
{
  private static Gtk.Window win = null;
  /* we redraw when this differs to the current second */
  private static int last_second = -1;
  private static int window_size = 512;
  private static double gun_size = 1.0/15.0;
  private static double gun_xvalue = 0.5 - gun_size/2;
  private static bool going_left = false;
  private static bool going_right = false;
  Context context;
  DrawingArea area;
  private static uint mytimer;

  /*
   *  invaders constructor, creates a 512x512 game window and calls the
   *  update handler every 1/10 of a second.
   */

  invaders ()
  {
    /* 100/1000 is the 1/10 second time interval.  */
    mytimer = GLib.Timeout.Add(100, new GLib.TimeoutHandler (update_handler) );

    win = new Gtk.Window ("Space Invaders");
    win.SetDefaultSize (window_size, window_size);
    win.SetPosition (WindowPosition.Center);
    /* add what to do when a delete event is processed.  */
    win.DeleteEvent += new DeleteEventHandler (handle_quit);

    area = new DrawingArea ();
    area.AddEvents ((int) 
		    (EventMask.ButtonPressMask    
		     |EventMask.ButtonReleaseMask    
		     |EventMask.KeyPressMask    
		     |EventMask.PointerMotionMask));
    area.AddEvents ((int) EventMask.ButtonPressMask);
    area.ButtonPressEvent += new ButtonPressEventHandler (MyHandler);

    System.Console.WriteLine ("before win.Add");
    win.Add (area);
    System.Console.WriteLine ("before win.ShowAll");

    win.ShowAll ();
    System.Console.WriteLine ("before context");
    context = Gdk.CairoHelper.Create (area.GdkWindow);
    System.Console.WriteLine ("finish gamewindow");
  }

  void do_run (object sender, EventArgs args)
  {
  }

  private void MyHandler (object obj, ButtonPressEventArgs args)
  {
    if (args.Event.Type == EventType.ButtonPress) {
      // the left button was used
      if (args.Event.Button == 1) {
	Console.WriteLine ("left");
	going_left = true;
	going_right = false;
      }
      // the right button is down
      else if (args.Event.Button == 3) {
	Console.WriteLine ("right");
	going_left = false;
	going_right = true;
      }
      // the middle button is down
      else if (args.Event.Button == 2) {
	Console.WriteLine ("middle");
      }
    }
  }

  Context draw (Context cr, int width, int height)
  {
    int size = Math.Min (width, height);

    cr.Save ();
    // clear the canvas, cr.
    cr.Color = new Cairo.Color (0, 0, 0);  /* window is all white */
    cr.Rectangle (0, 0, width, height);
    cr.Fill ();

    /*
     *  origin at the bottom left of the window.
     */

    cr.Scale (size/1.0, size/1.0);
    cr.Restore ();

    /*
     *  we draw our gun
     */
    // cr = drawGun (cr);
    Console.WriteLine ("draw called");

    return cr;
  }


  Context circle (Context cr, double x, double y, double r,
		  double t, bool filled)
  {
    cr.Save ();
    if (! filled)
      cr.LineWidth = t;
    cr.Arc (x, y, r, 0.0, 2.0 * Math.PI);
    if (filled)
      cr.Fill ();
    else
      cr.Stroke ();
    cr.Restore ();
    return cr;
  }


  Context drawGun (Context cr)
  {
    cr.Save ();
    cr.Color = new Cairo.Color (0, 0, 0);  /* black */
    cr.Rectangle (gun_xvalue, gun_size, gun_size, gun_size);
    cr.Fill ();
    cr.Restore ();
    return cr;
  }


  /*
   *  update_handler - returns true to indicate it needs to be called again.
   *                   This function is automatically called by the event
   *                   subsystem every 1/10th of a second.  We check to
   *                   see if the second has changed and if so we redraw
   *                   the clock by queuing a draw event.
   */

  private static bool update_handler ()
  {
    Console.WriteLine ("update!");

    if (going_right && gun_xvalue < 1.0)
      gun_xvalue += 0.1;
    if (going_left && gun_xvalue > 0.0)
      gun_xvalue -= 0.1;

    win.QueueDraw ();
    return true;
  }

  /*
   *  OnExposeEvent - obtains the context, window size and then
   *                  calls draw.  Notice we cannot choose the
   *                  name of this method.
   */

  protected override bool OnExposeEvent (Gdk.EventExpose e)
  {
    using (Context cr = Gdk.CairoHelper.Create (e.Window))
    {
      int w, h;
      e.Window.GetSize (out w, out h);
      draw (cr, w, h);
    }
    return true;
  }

  /*
   *  handle_quit - this method is called when the user presses the quit button.
   */

  void handle_quit (object sender, DeleteEventArgs e)
  {
    GLib.Source.Remove (mytimer);
    Application.Quit ();
  }

  void do_exit (object sender, EventArgs args)
  {
    GLib.Source.Remove (mytimer);
    Application.Quit();
  }

  /*
   *  Main - the initial method which creates the invaders object and enters the event loop.
   */

  static void Main ()
  {
    Application.Init ();
    new invaders ();
    Application.Run ();
  }
}
