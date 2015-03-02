using System;
using Gtk;
using Gdk;
using Cairo;

namespace lunar
{
	class MainClass: DrawingArea
	{
		// local fields for this class
		private static Gtk.Button start_game;
		private static Gtk.DrawingArea area;
		private static bool started = false;
		private static double height;
		private static double velocity;
		private static double accel;
		private static double fuel;
		private static double seconds;
		private static Gtk.Window w;
		private static double gravity = -1.6;   // lunar gravity ms^2
		private static bool rocket_on = false;
		private static Context context;


  		static void ButtonPressHandler (object obj, ButtonPressEventArgs args)
  		{
    		if (args.Event.Type == EventType.ButtonPress) {
 				// the left button was used
				if (args.Event.Button == 1) {
					rocket_on = true;
				}	
    		}
  		}


  		static void ButtonReleaseHandler (object obj, ButtonReleaseEventArgs args)
		{
			if (args.Event.Type == EventType.ButtonRelease) {
				// the left button was released
				if (args.Event.Button == 1)
					rocket_on = false;		
    		}
  		}


		static bool safe_velocity (double v)
		{
			if (v > 0.0) {
				return v < 10.0;
			} else {
				return (-v) < 10.0;
			}
		}

		static void update_physics (double time)
		{
			if (height <= 0.0) {
				started = false;
				if (safe_velocity (velocity)) {
					Console.WriteLine ("Huston the eagle has landed!");
				} else if (velocity < -30) {
					Console.WriteLine ("you definitely need to play again");
				} else {
					Console.WriteLine ("crashed ...");
				}
			} else {
				if (rocket_on) {
					if (fuel >= time) {
						fuel = fuel - time;
						accel = accel + time;
					}
				} else if (accel > time)
					accel = accel - time;
				else
					accel = 0.0;

				height = height + velocity * time;
				velocity = velocity + gravity + accel;
			}
		}
		
		static void Expose_Event (object obj, ExposeEventArgs args)
		{
			// Console.WriteLine ("Expose_Event");
			Gdk.EventExpose e = args.Event;
			using (Context cr = Gdk.CairoHelper.Create (e.Window)) {
				int w, h;
				e.Window.GetSize (out w, out h);
				draw (cr, w, h);
			}
		}

		static bool update_handler ()
		{
			if (started) {
				seconds = seconds + 1.0;
				update_physics (1.0);
				Console.WriteLine ("{0}: velocity {1}, accel {2}, height {3}, fuel {4}, engine on {5}",
				                   seconds, velocity, accel, height, fuel, rocket_on);
			}
            w.QueueDraw ();  // post a draw event which is processed by the Expose_Event handler below
			return true;
		}


		static void do_start (object obj, EventArgs args)
		{
			if (! started) {
				started = true;
				height = 5000.0;    // 5K above surface
				velocity = -100.0;  // 100 ms downwards
				accel = 0.0;
				fuel = 100.0;
				seconds = 0.0;
				rocket_on = false;
				Console.WriteLine ("game started");
			}
		}

		static void Window_Delete (object obj, DeleteEventArgs args)
		{
			Application.Quit ();
			args.RetVal = true;
		}

 		/*                                                                                                                                           
   		 *  draw - load and draw the lunar lander into the Window.                                                                                   
   		 */

  		static void draw (Context cr, int win_width, int win_height)
		{
			// Console.WriteLine ("drawing lunar lander {0}, {1}, {2}", cr, win_width, win_height);

			/*                                                                                                                                         
     		 *  load the lunar lander image                                                                                                            
     		 */
			ImageSurface eagle = new ImageSurface ("eagle.png");

			// Console.WriteLine ("eagle = {0}", eagle);

			/*                                                                                                                                         
     		 *  the background needs to be transparent                                                                                                 
     		 */
			cr.SetSourceRGBA (0, 0, 0, 0);
			cr.Paint ();

			/*                                                                                                                                         
     		 *  copy our lander onto the surface                                                                                                       
     		 */
			int p = 0;

			if (height < 5000.0) {
				double f = 5000.0 - height;   // distance from the top
				p = (int)((f/7000.0) * (double)win_height);
				// Console.WriteLine ("position {0}", p);
			}
    		cr.SetSourceSurface (eagle, 25, p);
    		cr.Paint();
  		}


		static void initialise_lunar ()
		{
			w = new Gtk.Window ("Lunar Lander");
			VBox v = new VBox ();

			// w.SetDefaultSize (200, 500);
			v.Spacing = 12;
			w.Add (v);

			area = new DrawingArea ();
			area.SetSizeRequest (200, 500);
			area.ExposeEvent += Expose_Event;
			area.ButtonPressEvent += new ButtonPressEventHandler (ButtonPressHandler);
    		area.ButtonReleaseEvent += new ButtonReleaseEventHandler (ButtonReleaseHandler);

  			area.AddEvents ((int) (EventMask.ButtonPressMask | EventMask.ButtonReleaseMask));

			// v.Add (area);
			v.PackStart (area);

			HBox h = new HBox ();
			start_game = new Button ("Start Game");
			start_game.Clicked += do_start;

			h.PackStart (start_game, false, false, 0);
			v.PackEnd (h, false, false, 0);

			w.ShowAll ();
			w.Resizable = false;
			w.DeleteEvent += Window_Delete;

			context = Gdk.CairoHelper.Create(area.GdkWindow);

  			GLib.Timeout.Add (1000/10, update_handler);
		}

		/*
		 *  the main method
		 */

		public static void Main (string[] args)
		{		
			Application.Init ();
			initialise_lunar ();
			Application.Run ();
		}
	}
}
