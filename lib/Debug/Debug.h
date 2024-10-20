#ifndef DEBUG_H
#define DEBUG_H

// Set this to 1 to enable debug output, 0 to disable
#define DEBUG_ENABLE 1

#if DEBUG_ENABLE
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

#endif // DEBUG_H