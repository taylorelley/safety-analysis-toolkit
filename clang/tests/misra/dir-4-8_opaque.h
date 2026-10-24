#ifndef DIR_4_8_OPAQUE_H
#define DIR_4_8_OPAQUE_H
struct hidden { int value; }; /* NONCOMPLIANT */
struct used { int value; };
struct unused_here { int value; };
#endif
