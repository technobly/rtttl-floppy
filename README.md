# rtttl-floppy

A RTTTL (RingTone Text Transfer Language) player for floppy drive music

[![Youtube](http://i.imgur.com/mQbRVRH.png)](https://www.youtube.com/watch?v=XMm_lL8oMrE)

## Usage

```c++
// see library and examples for full wiring description

#include "rtttl-floppy.h"

char *song = (char *)"The Simpsons:d=4,o=2,b=168:c.3,e3,f#3,8a3,g.3,e3,c3,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c3,8c3,8c3,c3";

RTTTL_Floppy floppy(D2, D3, song);

void setup() {
    floppy.start();
    floppy.play();
}

void loop() {
    floppy.cycle(); // this needs to be called regularly or the song will stall
    Particle.process();
}
```

See a [complete example](examples/music-box/music-box.ino) in the examples directory.

## License
Copyright 2016 Brett Walach
Released under the MIT license
