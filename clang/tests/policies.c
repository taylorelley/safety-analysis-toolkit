#include <setjmp.h>
extern char *gets(char *);
#define REPEAT(x) ((x) + (x))
#define ONCE(x) ((x) + 1)
#define TEXT(x) "x x"
#define OBJECT goto
void review(char *buffer, jmp_buf env) { gets(buffer); longjmp(env, 1); }
int macros(int x) { return REPEAT(x) + ONCE(x); }
