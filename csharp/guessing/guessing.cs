using System;

public class guessing
{
  public static void Main ()
  {
    Random rnd = new Random (1234);
    int secret = rnd.Next (100)+1;
    int your_guess = -1;
    string line;
    int count = 1;

    Console.WriteLine ("I have thought of a number between 1 and 100 see if you can guess it");
    do { 
      Console.Write ("what is your guess? ");
      line = Console.ReadLine ();
      if (line != null) {
	your_guess = Convert.ToInt32 (line);
	if (your_guess < secret)
	  Console.WriteLine ("{0} is too low, try a higher number", your_guess);
	else if (your_guess > secret)
	  Console.WriteLine ("{0} is too high, try a lower number", your_guess);
	else {
	  Console.WriteLine ("well done, {0} was correct, you succeeded in {1} guesses",
			     your_guess, count);
	}
	count++;
      }
    } while (your_guess != secret);
    Console.WriteLine ("goodbye");
  }
}
