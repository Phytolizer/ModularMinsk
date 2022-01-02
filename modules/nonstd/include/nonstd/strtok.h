#ifndef NONSTD_STRTOK_H_
#define NONSTD_STRTOK_H_

char* NonstdStrtok(char* str, const char* delim);
char* NonstdStringTokenizeReentrant(char* str, const char* delim, char** savep);

#endif  // NONSTD_STRTOK_H_
