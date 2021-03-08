#ifndef PARSE_ARG_H
#define PARSE_ARG_H
typedef int (*pCallback) (char *, char*, void *);
int parseCmdLine(int argc, char * argv[], pCallback p, void * userData);
#endif