#ifndef USERFUNCTIONS_DEF
#define USERFUNCTIONS_DEF

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

extern int meni();
extern int meniTopic();
extern int meniCity();

extern void inputToString(int input, char* dst);
extern void inputTopicToString(int input, char* dst);
extern void inputCityToString(int input, char* dst);

#endif