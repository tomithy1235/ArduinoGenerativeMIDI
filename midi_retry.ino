#include "midi_defs_funcs.h"

#define QUANTIZATION 0 // set as 0 for none

byte midi_start = 0xfa;
byte midi_stop = 0xfc;
byte midi_clock = 0xf8;
byte midi_continue = 0xfb;

bool play_flag = 0;
byte data;

byte midi_quarter_frame = 0xF1;
/* A quarter-frame messages consists of a status byte of 0xF1, 
followed by a single 7-bit data value: 3 bits to identify
the piece, and 4 bits of partial time code. (Wikipedia) */

/* Next byte is broken into two halves (taken from: http://www.recordingblogs.com/sa/tabid/88/Default.aspx?topic=MIDI+Quarter+Frame+message )
Consider: 0xZX. This is two parts: Z is from 0-7. 
0,1 mean X is lsb's,msb's (respectively) of the frame,
2,3 mean X is lsb's,msb's of the seconds
4,5 mean X is lsb's,msb's of the minutes
6,7 mean X is lsb's,msb's of the hours (fuck that)

I can have a macro that I put Tempo into, and then other macros/consts to make
the values that I need in order to ACTUALLY hit the quarter notes or whatever.
TODO: make it into a pot that I can adjust and get values on or whatever.

So what if I just look at the evens (0,2,4,6) and then use the lsb's of whichever
unit I want (probably just seconds? maybe frames if beats are too off of seconds.
I really am mostly wanting just quarter notes possible at this point.
*/



// So quarter notes will be every... 

Ranq ran(analogRead(A0) + 1); // put a pot on A0 to adjust seed as I wish
/* 
use ran.int64() for a random num in range of:
0 to 18,446,744,073,709,551,615

use ran.int32() for random num in range of:
0 to 4,294,967,295
*/

note_t note = D5_n;
int count = 0;

static int cur_beat = 0; // This will multiply time_factor to give us when to actually play a note

// Use WITHIN_DELTA(var,mid) to tell whether var is within preset DELTA of mid (use cur_beat)

void setup() 
{
  //MIDI baud rate:
  Serial.begin(31250);
  
  pinMode(11,OUTPUT);
}
//MIDI TIMECODE TIME!!!

void loop() 
{
  static float cur_time_secs = 0.0;
  //TODO: change to a switch-case
  if(Serial.available() > 0) 
  {
    data = Serial.read();
    if(data == midi_start || data == midi_continue)
    {
      play_flag = 1;
//      digitalWrite(11,HIGH);
    }
    else if(data == midi_stop) 
    {
      play_flag = 0;
      analogWrite(11,0);
    }
    else if( (data == midi_clock) && (play_flag == 1) ) 
    {
      if(0 == (count % CLOCKS_PER_EIGHTH_NOTE))
      {
        Sync();
      }
      count = (count+1) % CLOCKS_PER_BAR; // so 95 is max value of count
      analogWrite(11,count*2);
    }
    else if( (data == midi_quarter_frame) )
    {
      while(!Serial.available()){} // wait for next byte
      data = Serial.read();
      switch(data >> 4) // the 
    }
  }
}




static inline float framesToSec(int frames)
{
  if(frames == 0) // Don't divide by 0
    return 0;
  else
    return 1.0/frames;
}



/* ---------------- Functions using globals: ---------------------- */

static inline void noteOn(byte cmd, note_t pitch, byte velocity) 
{
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
  //could set noteIsOn for anytime velocity > 0
}

  
static inline void Sync() 
{
  static bool noteIsOn = 0;
// do something for every MIDI Clock pulse when the sequencer is running
  //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
  if(noteIsOn == 0)
  {
    noteOn(0x90, note, 0x45);
    noteIsOn = 1;
  }
  //Note on channel 1 (0x90), some note value (note), silent velocity (0x00):
  else
  {
    noteOn(0x90, note, 0x00);
    noteIsOn = 0;
  }
}


  
