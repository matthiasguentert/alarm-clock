#ifndef DEBUG_HPP
#define DEBUG_HPP

#if DEBUG
#define D_SerialBegin(...) Serial.begin(__VA_ARGS__)
#define D_SerialBeginAndWait(...) \
  Serial.begin(__VA_ARGS__);      \
  while (!Serial)                 \
  {                               \
    delay(1);                     \
  }
#define D_print(...) Serial.print(__VA_ARGS__)
#define D_println(...) Serial.println(__VA_ARGS__)
#define D_write(...) Serial.write(__VA_ARGS__)
#define D_SerialWait \
  while (!Serial)    \
  {                  \
    delay(1);        \
  }
#else
#define D_SerialBegin(...)
#define D_SerialBeginAndWait(...)
#define D_print(...)
#define D_println(...)
#define D_write(...)
#define D_SerialWait
#endif

#endif
