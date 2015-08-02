//#define ONE_OVER_EIGHT (float)0.125
//#define ONE_OVER_4 (float)0.25
//#define ONE_OVER_16(float)0.0625
//#define ONE_OVER_32 (float)0.03125

#define BPM 130
#define DEC_FACTOR 30 // 30 gives 8th notes (2*BPM is 8th notes, so 60/(2*BPM) is time per 8th note)
#define FPS 25 // frames per second; for all the Studio One stuff I do
//#define BPSec (float)BPM/60 //avoid using if possible (float division)\
#define FRAMES_PER_BEAT (FPS/BPM)*60 // beats = quarter notes
#define BEATS_PER_FRAME (BPM/(60*FPS)) // so now we have how many quarter frames it takes for a quarter note!
static const float time_factor = (float)DEC_FACTOR/(float)BPM;
static const float secs_per_frame = 1.0/(float)FPS;

#define CLOCKS_PER_QUARTER_NOTE 24
#define CLOCKS_PER_EIGHTH_NOTE 12
#define CLOCKS_PER_32ND_NOTE 6

#define CLOCKS_PER_BAR 96

#define DELTA 0.18*time_factor

#define WITHIN_DELTA(var,mid) ((var > (mid-DELTA)) && (var < (mid+DELTA))) ? 1 : 0

//  static const uint8_t
//    A0_n = 21, AS0_n = 22, B0_n = 23, C1_n = 24, CS1_n = 25, D1_n = 26,
//    DS1_n = 27, E1_n = 28, F1_n = 29, FS1_n = 30, G1_n = 31, GS1_n = 32,
//    
//    A1_n = 33, AS1_n = 34, B1_n = 35, C2_n = 36, CS2_n = 37, D2_n = 38,
//    DS2_n = 39, E2_n = 40, F2_n = 41, FS2_n = 42, G2_n = 43, GS2_n = 44,
//    
//    A2_n = 45, AS2v = 46, B2_n = 47, C3_n = 48, CS3_n = 49, D3_n = 50,
//    DS3_n = 51, E3_n = 52, F3_n = 53, FS3_n = 54, G3_n = 55, GS3_n = 56,
//    
//    A3_n = 57, AS3_n = 58, B3_n = 59, C4_n = 60, CS4_n = 61, D4_n = 62,
//    DS4_n = 63, E4_n = 64, F4_n = 65, FS4_n = 66, G4_n = 67, GS4_n = 68,
//    
//    A4_n = 69, AS4_n = 70, B4_n = 71, C5_n = 72, CS5_n = 73, D5_n = 74,
//    DS5_n = 75, E5_n = 76, F5_n = 77, FS5_n = 78, G5_n = 79, GS5_n = 80,
//    
//    A5_n = 81, AS5_n = 82, B5_n = 83, C6_n = 84, CS6_n = 85, D6_n = 86,
//    DS6_n = 87, E6_n = 88, F6_n = 89, FS6_n = 90, G6_n = 91, GS6_n = 92;

//  enum class note_t{
//    public:
//    A0_n = 21, AS0_n = 22, B0_n = 23, C1_n = 24, CS1_n = 25, D1_n = 26,
//    DS1_n = 27, E1_n = 28, F1_n = 29, FS1_n = 30, G1_n = 31, GS1_n = 32,
//    
//    A1_n = 33, AS1_n = 34, B1_n = 35, C2_n = 36, CS2_n = 37, D2_n = 38,
//    DS2_n = 39, E2_n = 40, F2_n = 41, FS2_n = 42, G2_n = 43, GS2_n = 44,
//    
//    A2_n = 45, AS2v = 46, B2_n = 47, C3_n = 48, CS3_n = 49, D3_n = 50,
//    DS3_n = 51, E3_n = 52, F3_n = 53, FS3_n = 54, G3_n = 55, GS3_n = 56,
//    
//    A3_n = 57, AS3_n = 58, B3_n = 59, C4_n = 60, CS4_n = 61, D4_n = 62,
//    DS4_n = 63, E4_n = 64, F4_n = 65, FS4_n = 66, G4_n = 67, GS4_n = 68,
//    
//    A4_n = 69, AS4_n = 70, B4_n = 71, C5_n = 72, CS5_n = 73, D5_n = 74,
//    DS5_n = 75, E5_n = 76, F5_n = 77, FS5_n = 78, G5_n = 79, GS5_n = 80,
//    
//    A5_n = 81, AS5_n = 82, B5_n = 83, C6_n = 84, CS6_n = 85, D6_n = 86,
//    DS6_n = 87, E6_n = 88, F6_n = 89, FS6_n = 90, G6_n = 91, GS6_n = 92,
//    
//    EOList // should be 93 (for now, at least)
//    
//  };
//  
//  note_t& operator++( note_t &n )
//  {
//    n = (note_t)( (int)(n) + 1 );
//    if( n == note_t::EOList )
//      n = note_t::A0_n;
//    return n;
//  }



/* This is an array to map the timecode time to actual 8th notes
   so we know when to send an actual MIDI signal */
static const float timeToBeats[] = 
{
  0, time_factor, 2*time_factor, 3*time_factor, 4*time_factor, 5*time_factor,
  6*time_factor, 7*time_factor, 8*time_factor, 9*time_factor, 10*time_factor
  
};
  
  
  enum note_t
  {
    A1_n = 21, AS1_n = 22, B1_n = 23, C2_n = 24, CS2_n = 25, D2_n = 26,
    DS2_n = 27, E2_n = 28, F2_n = 29, FS2_n = 30, G2_n = 31, GS2_n = 32,
    
    A2_n = 33, AS2_n = 34, B2_n = 35, C3_n = 36, CS3_n = 37, D3_n = 38,
    DS3_n = 39, E3_n = 40, F3_n = 41, FS3_n = 42, G3_n = 43, GS3_n = 44,
    
    A3_n = 45, AS3_n = 46, B3_n = 47, C4_n = 48, CS4_n = 49, D4_n = 50,
    DS4_n = 51, E4_n = 52, F4_n = 53, FS4_n = 54, G4_n = 55, GS4_n = 56,
    
    A4_n = 57, AS4_n = 58, B4_n = 59, C5_n = 60, CS5_n = 61, D5_n = 62,
    DS5_n = 63, E5_n = 64, F5_n = 65, FS5_n = 66, G5_n = 67, GS5_n = 68,
    
    A5_n = 69, AS5_n = 70, B5_n = 71, C6_n = 72, CS6_n = 73, D6_n = 74,
    DS6_n = 75, E6_n = 76, F6_n = 77, FS6_n = 78, G6_n = 79, GS6_n = 80,
    
    A6_n = 81, AS6_n = 82, B6_n = 83, C7_n = 84, CS7_n = 85, D7_n = 86,
    DS7_n = 87, E7_n = 88, F7_n = 89, FS7_n = 90, G7_n = 91, GS7_n = 92,
    
    EOList // should be 93 (for now, at least)
    
  };
  
  struct Ranq 
  {
    unsigned long long v;
    Ranq(unsigned long long j) : v(4101842887655102017LL)
    {
      v ^= j;
      v = int64();
    }
    inline unsigned long long int64()
    {
      v ^= v >> 21;
      v ^= v << 35;
      v ^= v >> 4;
      return v * 2685821657736338717LL;
    }
    inline unsigned long int32() {return (unsigned long) int64(); }
  };
  

//use to increment note:
static inline note_t inc_note(note_t n)
{
  n = (note_t)((int)n + 1);
  if(n == FS5_n)
    n = A1_n;
  return n;
}

  
  
