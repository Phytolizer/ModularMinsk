#ifndef ANSI_ESC_ANSI_ESC_H_
#define ANSI_ESC_ANSI_ESC_H_

#define ANSI_ESC_RESET "\x1b[0m"
#define ANSI_ESC_BOLD "\x1b[1m"
#define ANSI_ESC_DIM "\x1b[2m"
#define ANSI_ESC_UNDERLINE "\x1b[4m"
#define ANSI_ESC_BLINK "\x1b[5m"
#define ANSI_ESC_REVERSE "\x1b[7m"
#define ANSI_ESC_HIDDEN "\x1b[8m"

#define ANSI_ESC_FG_BLACK "\x1b[30m"
#define ANSI_ESC_FG_RED "\x1b[31m"
#define ANSI_ESC_FG_GREEN "\x1b[32m"
#define ANSI_ESC_FG_YELLOW "\x1b[33m"
#define ANSI_ESC_FG_BLUE "\x1b[34m"
#define ANSI_ESC_FG_MAGENTA "\x1b[35m"
#define ANSI_ESC_FG_CYAN "\x1b[36m"
#define ANSI_ESC_FG_WHITE "\x1b[37m"

#define ANSI_ESC_BG_BLACK "\x1b[40m"
#define ANSI_ESC_BG_RED "\x1b[41m"
#define ANSI_ESC_BG_GREEN "\x1b[42m"
#define ANSI_ESC_BG_YELLOW "\x1b[43m"
#define ANSI_ESC_BG_BLUE "\x1b[44m"
#define ANSI_ESC_BG_MAGENTA "\x1b[45m"
#define ANSI_ESC_BG_CYAN "\x1b[46m"
#define ANSI_ESC_BG_WHITE "\x1b[47m"

#define ANSI_ESC_FG_BRIGHT_BLACK "\x1b[90m"
#define ANSI_ESC_FG_BRIGHT_RED "\x1b[91m"
#define ANSI_ESC_FG_BRIGHT_GREEN "\x1b[92m"
#define ANSI_ESC_FG_BRIGHT_YELLOW "\x1b[93m"
#define ANSI_ESC_FG_BRIGHT_BLUE "\x1b[94m"
#define ANSI_ESC_FG_BRIGHT_MAGENTA "\x1b[95m"
#define ANSI_ESC_FG_BRIGHT_CYAN "\x1b[96m"
#define ANSI_ESC_FG_BRIGHT_WHITE "\x1b[97m"

#define ANSI_ESC_KILL_SCREEN "\x1b[2J"
#define ANSI_ESC_KILL_LINE "\x1b[2K"

#define ANSI_ESC_CURSOR_RESET "\x1b[H"

#endif  // ANSI_ESC_ANSI_ESC_H_
