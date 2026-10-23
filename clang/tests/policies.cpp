namespace user {
char *gets(char *p) { return p; }
int longjmp(int x) { return x; }
}
#define ONCE(x) ((x)+1)
template<class T> T identity(T x) { return x; }
int safe() { return identity(ONCE(user::longjmp(1))); }
