// CHECKS: misra-c2012-rule-4-2
// ARGS: -std=c99 -Wno-trigraphs
const char *date = "(Date should be in the form ??-??-??)"; // NONCOMPLIANT
const char *question = "Really? ?";
