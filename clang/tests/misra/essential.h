/* Shared declarations for the essential type fixtures. */
#ifndef ESSENTIAL_H
#define ESSENTIAL_H
#include <stdbool.h>
#include <stdint.h>
typedef float float32_t;
typedef double float64_t;
typedef _Bool bool_t;
enum enuma { a1, a2, a3 };
enum enumb { B1, B2, B3 };
enum { K1 = 1, K2 = 128 };
extern enum enuma ena;
extern enum enumb enb;
extern bool_t bla, blb;
extern char cha, chb;
extern int8_t s8a, s8b;
extern int16_t s16a, s16b;
extern int32_t s32a, s32b;
extern uint8_t u8a, u8b, u8c, u8d;
extern uint16_t u16a, u16b, u16c;
extern uint32_t u32a, u32b, u32c;
extern float32_t f32a, f32b;
extern float64_t f64a;
extern void use_uint16(uint16_t x);
extern void use_uint32(uint32_t x);
#endif
