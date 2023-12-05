#include "Utils.hpp"
#include <rlgl.h>
#include <raymath.h>


void Log(int severity, const char *fmt, ...)
{
    /* Determine strings for the type and colour */
    const char *type;
    const char *color;
    switch (severity)
    {
    case LOG_INFO:
        type = "info";
        color = CONSOLE_COLOR_GREEN;
        break;
    case LOG_ERROR:
        type = "error";
        color = CONSOLE_COLOR_RED;
        break;
    case LOG_WARNING:
        type = "warning";
        color = CONSOLE_COLOR_PURPLE;
        break;
    default:
        break; /* Unreachable */
    }

    /* Obtain the current date and time */
    time_t rawTime;
    struct tm *timeInfo;
    char timeBuffer[80];

    time(&rawTime);
    timeInfo = localtime(&rawTime);

    strftime(timeBuffer, sizeof(timeBuffer), "[%H:%M:%S]", timeInfo);

    /* Format for printing to the console (with colours) */
    char consoleFormat[1024];
    snprintf(consoleFormat, 1024, "%s%s %s%s%s: %s\n", CONSOLE_COLOR_CYAN,
             timeBuffer, color, type, CONSOLE_COLOR_RESET, fmt);

    va_list argptr;

    /* Print to the console */
    va_start(argptr, fmt);
    vprintf(consoleFormat, argptr);
    va_end(argptr);
}


