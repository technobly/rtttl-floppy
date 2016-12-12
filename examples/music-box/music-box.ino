// Music Box Example for rtttl-floppy library by Brett Walach.

#include "rtttl-floppy.h"

SYSTEM_MODE(MANUAL);

/* When the drive is powered up initially, it will drive itself all of the way to the outer floppy track (1)
 *
 * Wiring
 * -------
 * Music box should have a lid mounted cherry switch with the Normally Closed set of contacts in series
 * with the power connection to the Photon, either on the USB cable, or directly wired to VIN.  The drive
 * should be powered with 5V at 1A.
 * VIN (5V on Floppy power connector)
 * GND (GND on Floppy power connector) */
#define STEPOUT D2 // GREEN WIRE (Pin 18 on Floppy)
#define DIROUT D3  // BLUE WIRE  (Pin 20 on Floppy)

// Note: songs should be converted to Octave 1-3 to sound good on the floppy drive.
char *song1 = (char *)"The Simpsons:d=4,o=2,b=168:c.3,e3,f#3,8a3,g.3,e3,c3,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c3,8c3,8c3,c3";
char *song2 = (char *)"Indiana:d=4,o=2,b=150:e,8p,8f,8g,8p,1c3,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f3,p,a,8p,8b,2c3,2d3,2e3,e,8p,8f,8g,8p,1c3,p,d3,8p,8e3,1f.3,g,8p,8g,e.3,8p,d3,8p,8g,e.3,8p,d3,8p,8g,f.3,8p,e3,8p,8d3,2c3";
char *song3 = (char *)"TakeOnMe:d=4,o=2,b=100:8f#3,8f#3,8f#3,8d3,8p,8b,8p,8e3,8p,8e3,8p,8e3,8g#3,8g#3,8a3,8b3,8a3,8a3,8a3,8e3,8p,8d3,8p,8f#3,8p,8f#3,8p,8f#3,8e3,8e3,8f#3,8e3,8f#3,8f#3,8f#3,8d3,8p,8b,8p,8e3,8p,8e3,8p,8e3,8g#3,8g#3,8a3,8b3,8a3,8a3,8a3,8e3,8p,8d3,8p,8f#3,8p,8f#3,8p,8f#3,8e3,8e3";
char *song4 = (char *)"Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6";
char *song5 = (char *)"Muppets:d=4,o=5,b=250:c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,8a,8p,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,8e,8p,8e,g,2p,c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,a,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,d,8d,c";
char *song6 = (char *)"Xfiles:d=4,o=5,b=125:e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,g6,f#6,e6,d6,e6,2b.,1p,g6,f#6,e6,d6,f#6,2b.,1p,e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,e6,2b.";
char *song7 = (char *)"Looney:d=4,o=5,b=140:32p,c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f";
char *song8 = (char *)"20thCenFox:d=16,o=5,b=140:b,8p,b,b,2b,p,c6,32p,b,32p,c6,32p,b,32p,c6,32p,b,8p,b,b,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,g#,32p,a,32p,b,8p,b,b,2b,4p,8e,8g#,8b,1c#6,8f#,8a,8c#6,1e6,8a,8c#6,8e6,1e6,8b,8g#,8a,2b";
char *song9 = (char *)"Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6";
char *song10 = (char *)"MASH:d=8,o=5,b=140:4a,4g,f#,g,p,f#,p,g,p,f#,p,2e.,p,f#,e,4f#,e,f#,p,e,p,4d.,p,f#,4e,d,e,p,d,p,e,p,d,p,2c#.,p,d,c#,4d,c#,d,p,e,p,4f#,p,a,p,4b,a,b,p,a,p,b,p,2a.,4p,a,b,a,4b,a,b,p,2a.,a,4f#,a,b,p,d6,p,4e.6,d6,b,p,a,p,2b";
char *song11 = (char *)"StarWars:d=4,o=2,b=45:32p,32f#,32f#,32f#,8b.,8f#.3,32e3,32d#3,32c#3,8b.3,16f#.3,32e3,32d#3,32c#3,,8b.3,,16f#.3,,32e6,32d#3,,32e6,8c#.3,,32f#,32f#,32f#,8b.,8f#.3,,32e6,32d#3,,32c#3,,8b.3,,16f#.3,,32e6,32d#3,,32c#3,,8b.3,,16f#.3,,32e6,32d#3,,32e6,8c#3";
char *song12 = (char *)"GoodBad:d=4,o=5,b=56:32p,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,d#,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,c#6,32a#,32d#6,32a#,32d#6,8a#.,16f#.,32f.,32d#.,c#,32a#,32d#6,32a#,32d#6,8a#.,16g#.,d#";
char *song13 = (char *)"TopGun:d=4,o=4,b=31:32p,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,16f,d#,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,g#";
char *song14 = (char *)"A-Team:d=8,o=2,b=100:4d#3,a#,2d#3,16p,g#,4a#,4d#.,p,16g,16a#,d#3,a#,f3,2d#3,16p,c#.3,16c3,16a#,g#.,2a#";
char *song15 = (char *)"Flinstones:d=4,o=5,b=40:32p,16f6,16a#,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,d6,16f6,16a#.,16a#6,32g6,16f6,16a#.,32f6,32f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,a#,16a6,16d.6,16a#6,32a6,32a6,32g6,32f#6,32a6,8g6,16g6,16c.6,32a6,32a6,32g6,32g6,32f6,32e6,32g6,8f6,16f6,16a#.,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#6,16c7,8a#.6";
char *song16 = (char *)"Jeopardy:d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5,p,d#,g#,d#,g#5,d#,g#,2d#,d#,g#,d#,g#,c.7,8a#,8g#,8g,8f,8e,d#,g#,d#,g#5,d#,g#,2d#,g#.,8f,d#,c#,c,p,a#5,p,g#.5,d#,g#";
char *song17 = (char *)"Gadget:d=16,o=5,b=50:32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,32d#,32f,32f#,32g#,a#,d#6,4d6,32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,8d#";
char *song18 = (char *)"Smurfs:d=32,o=2,b=100:4c#3,16p,4f#3,p,16c#3,p,8d#3,p,8b,p,4g#,16p,4c#3,p,16a#,p,8f#,p,8a#,p,4g#,4p,g#,p,a#,p,b,p,c3,p,4c#3,16p,4f#3,p,16c#3,p,8d#3,p,8b,p,4g#,16p,4c#3,p,16a#,p,8b,p,8f,p,4f#";
char *song19 = (char *)"MahnaMahna:d=16,o=6,b=125:c#,c.,b5,8a#.5,8f.,4g#,a#,g.,4d#,8p,c#,c.,b5,8a#.5,8f.,g#.,8a#.,4g,8p,c#,c.,b5,8a#.5,8f.,4g#,f,g.,8d#.,f,g.,8d#.,f,8g,8d#.,f,8g,d#,8c,a#5,8d#.,8d#.,4d#,8d#.";
char *song20 = (char *)"LeisureSuit:d=16,o=6,b=56:f.5,f#.5,g.5,g#5,32a#5,f5,g#.5,a#.5,32f5,g#5,32a#5,g#5,8c#.,a#5,32c#,a5,a#.5,c#.,32a5,a#5,32c#,d#,8e,c#.,f.,f.,f.,f.,f,32e,d#,8d,a#.5,e,32f,e,32f,c#,d#.,c#";
char *song21 = (char *)"MissionImp:d=16,o=2,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d";
/* MACGYVER THEME SONG ... TRANSCRIBED BY BRETT WALACH - DEC 7th 2016 */
char *song22 = (char *)"MacGyver:d=8,o=2,b=100:32p,\
b1,e,a,b,4a,e,4b1,e,a,b,a,e,b1,e,b1,e,a,b,4a,e,4b1,e,d3,c3,d3,c3,b,a,\
b1,e,a,b,4a,e,4b1,e,a,b,a,e,b1,e,b1,e,a,b,4a,e,4b1,e,d3,c3,d3,c3,b,a,\
4b,8p,2b,8p,4b,8p,4a,4d.3,4b,8p,2b,8p,4b,1a,c3,c3,c3,c3,c3,c3,\
c3,c3,2b,f,4a,2g,c3,4c3,4b,a,b,a,4g,4e3,2a,\
c3,4c3,2b,f,4a,2g,c3,4c3,4b,a,b,a,4g,4e3,2a,4b.,\
c3,b,a,4c3,b,4a,d3,c3,b,4d3,c3,4b,e3,d3,e3,4f#3,4b,4g3,4f#3,4e3,\
g3,e3,b,f#3,d3,a,e3,c3,g,d3,b,g,c3,f,b,e,c3,b,a,g,4b_,4a,4g,c3,c3,c3,c3,\
c3,c3,2b,g,4a,2g,c3,4c3,4b,a,b,a,4g,4e3,2a,\
c3,4c3,2b,g,4a,2g,c3,4c3,4b,a,b,a,4g,4e3,\
2a,2b,2c3,2d3,2e3,2g3,\
g4,g3,d4,g3,e_4,e_3,b_3,a3,g3,g,d3,g,e_3,g,b_,a,g,g,g,g,g,g,g";
char *song24 = (char *)"SMBtheme:d=4,o=2,b=60:16e3,16e3,32p,8e3,16c3,8e3,8g3,8p,8g,8p,8c3,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e3,16g3,8a3,16f3,8g3,8e3,16c3,16d3,8b,16p,8c3,16p,8g,16p,8e,16p,8a,8b,16a#,8a,16g.,16e3,16g3,8a3,16f3,8g3,8e3,16c3,16d3,8b,8p,16g3,16f#3,16f3,16d#3,16p,16e3,16p,16g#,16a,16c3,16p,16a,16c3,16d3,8p,16g3,16f#3,16f3,16d#3,16p,16e3,16p,16c3,16p,16c7,16c7,p,16g3,16f#3,16f3,16d#3,16p,16e3,16p,16g#,16a,16c3,16p,16a,16c3,16d3,8p,16d#3,8p,16d3,8p,16c3";
char *song25 = (char *)"SMBunderground:d=16,o=2,b=60:c,c1,a1,a,a#1,a#,2p,8p,c,c1,a1,a,a#1,a#,2p,8p,f1,f,d1,d,d#1,d#,2p,8p,f1,f,d1,d,d#1,d#,2p,32d#,d,32c#,c,p,d#,p,d,p,g#1,p,g1,p,c#,p,32c,f#,32f,32e,a#,32a,g#,32p,d#,b1,32p,a#1,32p,a1,g#1";
char *song26 = (char *)"SMBwater:d=8,o=2,b=200:4d1,4e1,4f#1,4g1,4a1,4a#1,b1,b1,b1,p,b1,p,2b1,p,g1,2e.,2d#.,2e.,p,g1,a1,b1,c,d,2e.,2d#,4f,2e.,2p,p,g1,2d.,2c#.,2d.,p,g1,a1,b1,c,c#,2d.,2g1,4f,2e.,2p,p,g5,2g.,2g.,2g.,4g,4a,p,g,2f.,2f.,2f.,4f,4g,p,f,2e.,4a5,4b5,4f,e,e,4e.,b5,2c.";
char *song27 = (char *)"SMBdeath:d=4,o=2,b=90:32c3,32c3,32c3,8p,16b,16f3,16p,16f3,16f.3,16e.3,16d3,16c3,16p,16e,16p,16c";
char *song28 = (char *)"RickRoll:d=4,o=2,b=160:8g,8a,8c3,8a,e3,8p,e3,8p,d3.,p,8p,8g,8a,8c3,8a,d3,8p,d3,8p,c3,8b,a.,8g,8a,8c3,8a,2c3,d3,b,a,g.,8p,g,2d3,2c3.,p,8g,8a,8c3,8a,e3,8p,e3,8p,d3.,p,8p,8g,8a,8c3,8a,2g3,b,c3.,8b,a,8g,8a,8c3,8a,2c3,d3,b,a,g.,8p,g,2d3,2c3.";

#define SONGS_MAX 9
char * song_array[] = {song24,song25,song28,song22,song1,song3,song18,song14,song2};

void cycleRGBLED() {
    static int count = 0;
    static const int c[8][3] = {255,0,0, 255,150,0, 255,255,0, 0,255,0, 0,0,255, 0,255,255, 255,0,255};
    RGB.control(true);
    RGB.color(c[count][0],c[count][1],c[count][2]);
    if (++count > 6) count = 0;
}

void setup(void)
{
    // Every time we power up, fetch the songIdx from the EEPROM, use it, then advance it and store it again.
    uint16_t songIdx;
    uint16_t magic_num = 0x1234;
    uint16_t temp = 0;
    EEPROM.get(0, temp);
    if (temp != magic_num) { // If this is our first time writing to this Photon, initialize the EEPROM
        EEPROM.put(0, 0x1234);
        songIdx = 0;
    }
    else {
        EEPROM.get(2, songIdx);
    }

    RTTTL_Floppy floppy(STEPOUT, DIROUT, song_array[songIdx]);
    if (++songIdx >= SONGS_MAX) songIdx = 0;
    EEPROM.put(2, songIdx);

    delay(600); // wait for drive to power up and align with track 1 (it may have to travel 80 tracks)
    floppy.start();
    floppy.play();

    // Using this while(1) instead of loop() to keep floppy object in scope
    while(1) {
        if (floppy.cycle()) { // this needs to be called regularly or the song will stall
            cycleRGBLED(); // if we are playing a note, cycle the colors on the RGB LED
        }
        Particle.process();
    }
}
