#include "midi_defs_funcs.h"

#define QUANTIZATION 0 // set as 0 for none

//byte midi_start = 0xfa;
//byte midi_stop = 0xfc;
//byte midi_clock = 0xf8;
//byte midi_continue = 0xfb;
//bool play_flag = 0;

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
byte quarter_frame_time_data = 0;

static int cur_beat = 0; // This will multiply time_factor to give us when to actually play a note

static float cur_time_secs = 0.0;

// Use WITHIN_DELTA(var,mid) to tell whether var is within preset DELTA of mid (use cur_beat)

void setup() 
{
  //MIDI baud rate:
  Serial.begin(31250);
  
  pinMode(11,OUTPUT);
  pinMode(3,OUTPUT);
}
//MIDI TIMECODE TIME!!!

void loop() 
{
  if(Serial.available() > 0) 
  {
    data = Serial.read();
//    if(data == midi_start || data == midi_continue)
//    {
//      play_flag = 1;
//    }
//    else if(data == midi_stop) 
//    {
//      play_flag = 0;
//      analogWrite(11,0);
//    }
//    else if( (data == midi_clock) && (play_flag == 1) ) 
//    {
//      if(0 == (count % CLOCKS_PER_EIGHTH_NOTE))
//      {
//        Sync();
//      }
//      count = (count+1) % CLOCKS_PER_BAR; // so 95 is max value of count
//      analogWrite(11,count*2);
//    }
    if( (data == midi_quarter_frame) )
    {
      while(!Serial.available()){} // wait for next byte
      data = Serial.read();
      quarter_frame_time_data = data & 0b00001111; //zero out top half of byte
      data = data >> 4;
      switch(data) // tells which data we're getting
      {
        case 0: //lsb's of frame
          cur_time_secs += framesToSec(quarter_frame_time_data);
          break;
        case 1:
          cur_time_secs += framesToSec(quarter_frame_time_data << 4);
          break;
        case 2: //lsb's of seconds
          cur_time_secs += quarter_frame_time_data;
          break;
        case 3:
          cur_time_secs += quarter_frame_time_data << 4;
          //check_time(cur_time_secs);
          break;
        case 4: //lsb's of mins
          cur_time_secs += 60 * (quarter_frame_time_data);
          check_time(cur_time_secs);
          break;
        case 5:
          cur_time_secs += 60 * (quarter_frame_time_data << 4);
          check_time(cur_time_secs);
          break;
        // 6 and 7 are hours and are ignored for now
        default:
          check_time(cur_time_secs);
          break;
      }
    }
  }
}

/* ---------------- Local Functions: ---------------------- */

static inline float framesToSec(int frames)
{
  if(frames == 0) // Don't divide by 0
    return 0;
  else
    return ( (float)frames * secs_per_frame ); //(secs / frames) * frames = secs
}

// This will check the timecode to see if we are ready to play a note yet (close enough to a beat)
static inline void check_time(float time)
{
//  static bool toggle = HIGH;
  analogWrite(3,time);
  if(WITHIN_DELTA(time,time_factor*cur_beat))
  {
    cur_time_secs = 0;
    Sync();
//    digitalWrite(3,toggle);
//    toggle = !toggle;
    return;
  }
  float time_under_delta = time - DELTA; // this tells us when to stop incrementing cur_beat if needed
  while(time_factor*cur_beat < time_under_delta)
  {
    cur_beat++;
    analogWrite(11,cur_beat);
  }
}



/* ---------------- Local Functions using globals: ---------------------- */

static inline void noteOn(byte cmd, note_t pitch, byte velocity) 
{
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
  //could set noteIsOn for anytime velocity > 0
}

  
static inline void Sync() 
{
//  static bool noteIsOn = 0;
  noteOn(0x90, note, 0x45);
  noteOn(0x90, note, 0x00); // 0 velocity kills note
//  static bool toggle = HIGH;
//  digitalWrite(3,toggle);
//  toggle = !toggle;
//  if(noteIsOn == 0)
//  {
//    noteOn(0x90, note, 0x45);
//    noteIsOn = 1;
//  }
//  else
//  { 
//    noteOn(0x90, note, 0x00); // 0 velocity kills note
//    noteIsOn = 0;
//  }
}


  
