#pragma once
#ifndef __MySort_H__
#define __MySort_H__

#include<stdio.h>
#include<stdlib.h>

typedef struct user_score {
	int score;
	char name[20];
}U_Score;

typedef U_Score DataType2;

void Mysort(DataType2 *array,int size);

#endif //sort 
