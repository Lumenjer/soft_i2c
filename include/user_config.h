#ifndef USER_CONFIG_H
#define USER_CONFIG_H

#define DEVICE_MAX 10
#define DEBUG

#ifdef LOG
  #undef LOG
#endif
#ifdef DEBUG
	#define LOG(func, ...) Serial.func(__VA_ARGS__)
#else
	#define LOG(func, ...) ;
#endif

#ifdef ASSERT
  #undef ASSERT
#endif
#define ASSERT(x) \
  if (!(x)){ \
    LOG(printf_P, PSTR("ASSERT: %s:%d\n"), __FILE__, __LINE__); \
    while(1){}; \
  }

#endif // USER_CONFIG_H
