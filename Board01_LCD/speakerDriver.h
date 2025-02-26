#ifdef __cplusplus
extern "C"
//From Lab Exercise 4-2
//Template by Dr. Jianjian Song
// Vance Steele and John Webb
{
#endif

#include "msp.h"

/* Note defines */
#define NOTECNT 3
// Note A4 - 440 Hz, B4 - 493.88 Hz, C5 - 523.26 Hz
#define NOTEA4  27273
#define NOTEB4  24297
#define NOTEC5  22933
//const uint16_t notePeriods[3] = { NOTEA4, NOTEB4, NOTEC5 };
//#define notes[] = {NOTEA4, NOTEB4, NOTEC5}

extern void configSpeaker(void);

extern void speakerBlare(void);

extern void speakerOff(void);
