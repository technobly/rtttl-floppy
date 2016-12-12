#pragma once

// A RTTTL (RingTone Text Transfer Language) player for floppy drive music

#include "Particle.h"

#define OCTAVE_OFFSET 0

class RTTTL_Floppy {
	private:
        void begin_rtttl(char *p);
        bool next_rtttl();
        void floppy_rtttl(int toneHz);

        int16_t _stepoutPin;
        int16_t _dirPin;
        char* _song;
        byte default_dur = 4;
        byte default_oct = 2;
        int bpm = 60;
        float extra_pause = 1.05; // to distinguish the notes better by increasing the pause
        int num;
        long wholenote;
        long duration;
        byte note;
        byte scale;
        char *songPtr;
        int trackIndex = 0;  // 80 is the total travel, stay +/-39 from center of travel to avoid ends, drive will power up on track 1 (outer edge)
        int dirState = LOW; // LOW moves towards track 80, center of floppy disk, HIGH moves towards track 1 outer edge of floppy disk.

    public:
        RTTTL_Floppy(int16_t stepoutPin, int16_t dirPin, char* song);
        void pause(void);
        void start(void);
        void stop(void);
        void play(void);
        bool cycle(void);
        bool isPlaying(void);
        bool isPaused(void);
        bool isSongDone(void);
        void resetDrive(void);

        bool isSongPlaying = false;
        bool songDone = false;
};

