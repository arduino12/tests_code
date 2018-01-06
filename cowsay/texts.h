#ifndef _TEXTS_H_
#define _TEXTS_H_

#include <Arduino.h>
#include "globals.h"
#include "screen.h"
//A1 DE
PCHARS(COW_SAY_P,		" ___   ^__^"				"\n"
						"/Moo\\_ (oo)\\______/`"	"\n"
						"\\___/ `(__)\\______)"		"\n"
						"        U  /|    /|"		"\n");
PCHARS(DOTS_P,			LEFT_DOT, RIGHT_DOT, '\n', LEFT_DOT, RIGHT_DOT, '\0');
PCHARS(DOT_P,			LEFT_DOT, RIGHT_DOT, '\0');
PCHARS(CELSIUS_P,		'O', FUL, TOP, '\n', ' ', FUL, BOT, '\0');
//PCHARS(CELSIUS_P,		CELSIUS, FUL, TOP, '\n', ' ', FUL, BOT, '\0');
//PCHARS(CELSIUS_P,		LEFT_DOT, RIGHT_DOT, FUL, TOP, '\n', ' ', ' ', FUL, BOT, '\0');
PCHARS(FORTHION_0001_P, "The early bird might get the worm, but the second mouse gets the cheese.");
PCHARS(FORTHION_0002_P, "Men are the best cooks, because with two eggs, one sausage, and a little bit of milk, he can fill a girl's tummy for nine months.");
PCHARS(FORTHION_0003_P, "I am on a seafood diet. I see food, and I eat it.");
PCHARS(FORTHION_0004_P, "When I think of books, I touch my shelf.");
PCHARS(FORTHION_0005_P, "Never trust an atom. They make up everything.");
PCHARS(FORTHION_0006_P, "I am a nobody. Nobody is perfect. Therefore, I am perfect.");
PCHARS(FORTHION_0007_P, "Future depends on your dreams. So go to sleep.");
PCHARS(FORTHION_0008_P, "A balanced diet is a chocolate chip cookie in each hand.");
PCHARS(FORTHION_0009_P, "If everything seems to be going well, you have obviously overlooked something.");
PCHARS(FORTHION_0010_P, "Everyone has a photographic memory, some just don't have film.");
PCHARS(FORTHION_0011_P, "Hard work never killed any body. But why take the risk?");
PCHARS(FORTHION_0012_P, "Deja moo! I swear that's the exact same cow we passed about six miles ago.");
PCHARS(FORTHION_0013_P, "Do files get embarrassed when they get unzipped?");
PCHARS(FORTHION_0014_P, "A chicken is an egg's way of producing more eggs.");
PCHARS(FORTHION_0015_P, "A bus station is where a bus stops. A train station is where a train stops. On my desk I have a work station.");
PCHARS(FORTHION_0016_P, "When Alexander Bell invented the telephone he had 3 missed calls from Chuck Norris");
PCHARS(FORTHION_0017_P, "Some magicans can walk on water, Chuck Norris can swim through land.");
PCHARS(FORTHION_0018_P, "Death once had a near-Chuck Norris experience");
PCHARS(FORTHION_0019_P, "Chuck Norris can slam a revolving door.");
PCHARS(FORTHION_0020_P, "Chuck Norris can light a fire by rubbing two ice-cubes together.");
PCHARS(FORTHION_0021_P, "Chuck Norris made a Happy Meal cry.");
PCHARS(FORTHION_0022_P, "Chuck Norris reads Hebrew from left to right.");
PCHARS(FORTHION_0023_P, "Chuck Norris was born on February 30th.");
PCHARS(FORTHION_0024_P, "Chuck Norris' house has a ceiling, but no roof");
PCHARS(FORTHION_0025_P, "Q: Why do programmers always mix up Halloween and Christmas?\nA: Because Oct(31) == Dec(25)!");
PCHARS(FORTHION_0026_P, "Its not a bug, its a feature.");
PCHARS(FORTHION_0027_P, "There are 10 types of people in the world. Those who understand binary and those who get laid.");
PCHARS(FORTHION_0028_P, "who && gawk && uname && talk && date && wine && touch && unzip && strip && touch && finger && mount && fsck && more && yes; yes; more; yes; umount && make clean && sleep");

PGM_P PARRAY(FORTHIONS_PP,
	FORTHION_0001_P, FORTHION_0002_P, FORTHION_0003_P, FORTHION_0004_P, FORTHION_0005_P,
	FORTHION_0006_P, FORTHION_0007_P, FORTHION_0008_P, FORTHION_0009_P, FORTHION_0010_P,
	FORTHION_0011_P, FORTHION_0012_P, FORTHION_0013_P, FORTHION_0014_P, FORTHION_0015_P,
	FORTHION_0016_P, FORTHION_0017_P, FORTHION_0018_P, FORTHION_0019_P, FORTHION_0020_P,
	FORTHION_0021_P, FORTHION_0022_P, FORTHION_0023_P, FORTHION_0024_P, FORTHION_0025_P,
	FORTHION_0026_P, FORTHION_0027_P, FORTHION_0028_P);

#endif
