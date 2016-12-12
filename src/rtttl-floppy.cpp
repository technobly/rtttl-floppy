#include "rtttl-floppy.h"

// A RTTTL (RingTone Text Transfer Language) player for floppy drive music
//
// Originally written by Brett Hagman (http://roguerobotics.com),
// Reworked by Technobly (http://technobly.com).
// Converted into a class by Chee Zhiquan(http://zv.sg). Added play, pause, stop functions
// Modified to work for Floppy Drive music by Brett Walach <technobly@gmail.com>
//
// This plays RTTTL (RingTone Text Transfer Language) songs by bit-banging a selected digital output.
//
// To play the output on a Floppy Drive, hook up 5V and GND to your floppy drive power connector,
// and define the STEPOUT and DIROUT pins and connect these pins to pins 18 and 20 on the floppy drive.
//
// When the drive is powered up initially, it will drive itself all of the way to the outer floppy track (1)
// #define STEPOUT D2 // GREEN WIRE (Pin 18 on Floppy)
// #define DIROUT D3  // BLUE WIRE  (Pin 20 on Floppy)
// RTTTL_Floppy floppy(STEPOUT, DIROUT, song);
//
// You can get more RTTTL songs from
// http://code.google.com/p/rogue-code/wiki/ToneLibraryDocumentation

// Note durations in Hz (FIXME: would be more precise to store these in microseconds)
int notes[] = { 0,
/*         C    C#   D    D#   E    F    F#   G    G#   A    A#   B */
/* O1 */  31,  34,  36,  38,  41,  43,  46,  49,  51,  55,  58,  61,
/* O2 */  65,  69,  73,  77,  82,  87,  93,  98, 103, 110, 117, 123,
/* O3 */ 131, 138, 147, 155, 165, 174, 185, 196, 207, 220, 233, 247,
/* O4 */ 262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494,
/* O5 */ 523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988,
/* O6 */ 1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
/* O7 */ 2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951
};

RTTTL_Floppy::RTTTL_Floppy(int16_t stepoutPin, int16_t dirPin, char* song) {
    _stepoutPin = stepoutPin;
    _dirPin = dirPin;
    _song = song;
}

void RTTTL_Floppy::start(void) {
    pinMode(_stepoutPin, OUTPUT);
    pinMode(_dirPin, OUTPUT);
    if(!songDone) {
        songDone = true;
        RTTTL_Floppy::begin_rtttl(_song);
    }
    RTTTL_Floppy::resetDrive();
}

void RTTTL_Floppy::pause(void) {
    isSongPlaying = false;
    songDone = false;
}

void RTTTL_Floppy::stop(void) {
    isSongPlaying = false;
    songDone = true;
    RTTTL_Floppy::begin_rtttl(_song);
}

void RTTTL_Floppy::play(void) {
    isSongPlaying = true;
}

bool RTTTL_Floppy::isSongDone(void) {
    return songDone;
}

bool RTTTL_Floppy::isPaused(void) {
    return !isSongPlaying;
}

bool RTTTL_Floppy::isPlaying(void) {
    return isSongPlaying;
}

bool RTTTL_Floppy::cycle(void) {
    if(isSongPlaying) {
        if(!RTTTL_Floppy::next_rtttl()) {
            songDone = false;
            isSongPlaying = false;
            return false;
        }
        return true;
    }
    return false;
}


void RTTTL_Floppy::begin_rtttl(char *p)
{
  // Absolutely no error checking in here

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while(*p != ':') p++;    // ignore name
  p++;                     // skip ':'

  // get default duration
  if(*p == 'd')
  {
    p++; p++;              // skip "d="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    if(num > 0) default_dur = num;
    p++;                   // skip comma
  }

  // get default octave
  if(*p == 'o')
  {
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if(num >= 1 && num <=7) default_oct = num;
    p++;                   // skip comma
  }

  // get BPM
  if(*p == 'b')
  {
    p++; p++;              // skip "b="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    bpm = num;
    p++;                   // skip colon
  }

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 2;  // this is the time for whole note (in milliseconds)

  // Save current song pointer...
  songPtr = p;
}

bool RTTTL_Floppy::next_rtttl() {

  char *p = songPtr;
  // if notes remain, play next note
  if(*p)
  {
    // first, get note duration, if available
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }

    if (num) duration = wholenote / num;
    else duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

    // now get the note
    note = 0;

    switch(*p)
    {
      case 'c':
        note = 1;
        break;
      case 'd':
        note = 3;
        break;
      case 'e':
        note = 5;
        break;
      case 'f':
        note = 6;
        break;
      case 'g':
        note = 8;
        break;
      case 'a':
        note = 10;
        break;
      case 'b':
        note = 12;
        break;
      case 'p':
      default:
        note = 0;
    }
    p++;

    // now, get optional '#' sharp
    if(*p == '#')
    {
      note++;
      p++;
    }

    // now, get optional '_' flat
    // not originally part of the RTTTL spec, added for convenience
    if(*p == '_')
    {
      note--;
      p++;
    }

    // now, get optional '.' dotted note
    if(*p == '.')
    {
      duration += duration/2;
      p++;
    }

    // now, get optional '*' stared note (which adds 1/8th note)
    // not originally part of the RTTTL spec, added for convenience
    if(*p == '*')
    {
      duration += wholenote/8;
      p++;
    }

    // now, get scale
    if(isdigit(*p))
    {
      scale = *p - '0';
      p++;
    }
    else
    {
      scale = default_oct;
    }

    scale += OCTAVE_OFFSET;

    if(*p == ',')
      p++;       // skip comma for next note (or we may be at the end)

    // Save current song pointer...
    songPtr = p;

    // now play the note
    if(note)
    {
      // LOG(INFO,"next 2: num:%d dur:%d scale:%d note:%d", num, duration, scale, note);
      RTTTL_Floppy::floppy_rtttl(notes[(scale - 1) * 12 + note]);
      delay((duration * extra_pause) - duration); // just the extra pause
    }
    else
    {
      delay(duration * extra_pause);
    }
    return 1; // note played successfully.
  }
  else {

    return 0; // all done
  }
}

// Pulse the STEPOUT pin to play a tone for a particular duration
void RTTTL_Floppy::floppy_rtttl(int toneHz) {
    long elapsed_time = 0;
    int toneUs = 1000000 / toneHz; // FIXME: loosing precision here, would be better to store tone in microseconds
    while (elapsed_time < (duration * 1000)) {

        digitalWriteFast(_stepoutPin, HIGH);
        delayMicroseconds(toneUs / 2);
        digitalWriteFast(_stepoutPin, LOW);
        delayMicroseconds(toneUs / 2);

        // Flip direction of floppy head after every note,
        // Unless far away from end of travel (this prevents bumping up against
        // and edge during note duration, which makes it sound like two notes.
        // This actually helps define the boundaries of every note,
        // especially if the same note is played repeatedly.
        if (!dirState) {
            if (++trackIndex >= 79) {
                dirState = !dirState;
                digitalWriteFast(_dirPin, dirState);
            }
        } else {
            if (--trackIndex <= 2) {
                dirState = !dirState;
                digitalWriteFast(_dirPin, dirState);
            }
        }

        // Keep track of how long we pulsed
        elapsed_time += toneUs;
    }
}

// Reset drive - start on track 2 on outer edge of floppy disk
void RTTTL_Floppy::resetDrive(void) {
    digitalWriteFast(_dirPin, LOW);
    delay(1);
    digitalWriteFast(_stepoutPin, HIGH);
    delayMicroseconds(2000);
    digitalWriteFast(_stepoutPin, LOW);
    delayMicroseconds(2000);
    trackIndex = 2; // Starts on track 2
    dirState = LOW; //    |
}

