using System;

public class hammurabi
{
  static int peasants_eat = 20;  /* amount consumed per peasant a year.   */
  static int plant_per_acre = 2; /* amount required to plant an acre.  */
  static int acres_per_peasant = 15;  /* 1 peasant can work 15 acres/year.  */
  static int land_area = 1000;
  static int population = 100;
  static int barn = 3000;
  static int planted = 0;
  static float crop_yield = 2.0f;    /* yield per acre of last years crop.  */
  static float rats = 0.0f;
  static Random rnd = new Random (1234);

  public static void simulateEconomy ()
  {
    crop_yield = (float)rnd.NextDouble() * 4.0f + 2.0f;   /* how good was the harvest? */
    rats = (float)rnd.NextDouble() * 10.0f;   /* rats eat up to 10% of the barns contents */
    int rats_ate = (int)((rats/100.0f) * (float)barn);
    barn -= rats_ate;
    barn += (int)((float)planted * crop_yield);
    Console.WriteLine ("Oh great king, the rats ate {0} bushels last year ({1}%)",
		       rats_ate, rats);
    Console.WriteLine ("The harvest per acre was {0} bushels and you planted {1} acres",
		       crop_yield, planted);
    Console.WriteLine ("After the harvest the kingdom now has a total of {0} bushels in the barns",
		       barn);
  }


  public static bool can_plant (int acres)
  {
    bool result = true;

    if (acres > land_area) {
      Console.WriteLine ("you only have {0} acres", land_area);
      result = false;
    }
    if (acres * plant_per_acre > barn) {
      Console.WriteLine ("you only have enough grain to plant {0} acres", barn/plant_per_acre);
      result = false;
    }

    if (acres_per_peasant * population < acres) {
      Console.WriteLine ("you only have enough peasants to work {0} acres",
			 acres_per_peasant * population);
      result = false;
    }
    return result;
  }


  public static bool readInput ()
  {
    int starved;
    int immigrants;
    int acres;
    int food;
    int newpop;

     /*
      *  how much seed to plant?
      */
     do {
       Console.Write ("how many acres to plant?  ");
       acres = Convert.ToInt32 (Console.ReadLine ());
       Console.WriteLine ("you wish to plant {0} acres of land", acres);
     } while (! can_plant (acres));

     planted = acres;
     barn -= planted;

     /*
      *  how much food to distribute?
      */
     do {
       starved = 0;
       immigrants = 0;

       Console.Write ("how much food to distribute as food?  ");
       food = Convert.ToInt32 (Console.ReadLine ());
       if (food >= 0) {
	 if (food >= population * peasants_eat) {
	   /*
	    *  more food than necessary
	    */
	   immigrants = (food - population * peasants_eat) / peasants_eat;
	 }
	 else {
	   /*
	    *  peasants starved
	    */
	   starved = population - (food / peasants_eat);
	 }
       }
     } while (food < 0);

     if (immigrants > 0)
       Console.WriteLine ("Your kingdom has attacted {0} new peasants", immigrants);
     if (starved > 0)
       Console.WriteLine ("Under your stewardship {0} peasants starved ({1}%)",
			  starved, starved * 100.0/population);
     newpop = population + immigrants - starved;
     Console.WriteLine ("Your kingdom has a total of {0} peasants", newpop);
     if (newpop < population * 0.55) {
       Console.WriteLine ("Oh dear, too many peasants have died due to your mismanagement and you have been assassinated");
       return false;
     }

     return true;
  }


  public static void Main ()
  {
    while (readInput ())
      simulateEconomy ();
  }
}
