#ifndef LOG_H
#define LOG_H

#ifdef NDEBUG
#define DebugPrint(...) (void)0
#else

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define DEBUG_FILE_NAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define DebugPrint(...) do{ \
    printf("<%s::%s> ", DEBUG_FILE_NAME, __func__); \
    printf(__VA_ARGS__); \
    printf("\n");} while(0)

/*
    //http://stackoverflow.com/questions/15549893/modify-printfs-via-macro-to-include-file-and-line-number-information
   //#define DebugPrint(...) printf(__VA_ARGS__)


    #define FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

    #define DebugPrint(...) do{ \
    printf("<%s::%s> ", FILE, __func__); \
    printf(__VA_ARGS__); \
    printf("\n");} while(0)
*/

#endif

extern bool GLOBAL_LOGGING_ENABLED;

//#define LOG(...) (void)0;

/*
#define LOG(...) \
printf(__VA_ARGS__); \
printf("\n");
*/

#define LOG(...) do{ \
    printf(__VA_ARGS__); \
    printf("\n");} while(0);


#endif // LOG_H
