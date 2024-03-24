
#define defaultOffset (0xEF000)

// efficient memory usage fr
#define timerMilliseconds (*((u64*)defaultOffset + 0x10))
#define timerStarted (*((int*)defaultOffset + 0x20))
#define sugmaBalls (*((int*)defaultOffset + 0x30))
#define formatted_time_string ((char*)defaultOffset + 0x40)
#define textDrawingPtr ((char*)defaultOffset + 0x50)
