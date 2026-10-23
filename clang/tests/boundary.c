/* A label alone is permitted. A forward transfer is detected. */
int boundary(int x) { again: if (x) { --x; goto again; } return x; }
