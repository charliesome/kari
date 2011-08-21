#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <gc.h>
#include "kari.h"
#include "kari_stdlib.h"

static char* readings[] = {    
    "Be joyful always; pray continually; give thanks in all circumstances, for this is God's will for"
    " you in Christ Jesus.",
    "Thessalonians 5:16-18",
    
    "\"For I know the plans I have for you,\" says the Lord. \"They are plans for good and not for"
    " disaster, to give you a future and a hope.\"",
    "Jeremiah 29:11",
    
    "One thing I ask of the LORD, this is what I seek: that I may dwell in the house of the LORD all"
    " the days of my life, to gaze upon the beauty of the LORD and to seek him in his temple.",
    "Psalm 27:4",
    
    "Taste and see that the LORD is good; blessed is the man who takes refuge in him.",
    "Psalm 34:8",
    
    "A friend loves at all times, and a brother is born for adversity.",
    "Proverbs 17:17",
    
    "The name of the Lord is a strong fortress; the godly run to him and are safe.",
    "Proberbs 18:10",
    
    "But they that wait upon the LORD shall renew their strength; they shall mount up with wings as"
    " eagles; they shall run, and not be weary; and they shall walk, and not faint.",
    "Isaiah 40:31",
    
    "Greater love has no one than this, that he lay down his life for his friends.",
    "John 15:13",
    
    "And we know that God causes everything to work together for the good of those who love God and"
    " are called according to his purpose for them.",
    "Romans 8:28"
};

K_FN(reading)
{
    size_t idx;
    char* buff;
    srand(time(NULL));
    idx = (rand() % (sizeof(readings) / sizeof(char*))) & ~1;
    buff = (char*)GC_MALLOC(strlen(readings[idx]) + strlen(readings[idx + 1]) + 20);
    sprintf(buff, "%s\n  - %s", readings[idx], readings[idx + 1]);
    return (kari_value_t*)kari_create_string(buff);
}