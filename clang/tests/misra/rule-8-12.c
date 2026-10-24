// CHECKS: misra-c2012-rule-8-12
enum colour { red = 3, blue, green, yellow = 5 }; // NONCOMPLIANT
enum shade { dark = 3, mid, light = 5, bright = 5 };
