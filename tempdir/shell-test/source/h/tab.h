#ifndef _TAB_H_
#define _TAB_H_
/** tab commands **/
extern char *autoCommand(const char *text, int state);
extern char **autoCommand(const char* text, int start, int end);
#endif