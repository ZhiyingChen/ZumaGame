#include "MySort.h"
void Mysort(DataType2 *array,int size)//ascending sort
{
	int i,j,min;
	DataType2 temp;
	for (i = 0; i < size; i++) {
		min = i;
		for (j = i+1; j < size; j++) {
			if (array[j].score < array[min].score) min = j;			
		}
		if (min != i) {
			temp = array[min];
			array[min] = array[i];
			array[i] = temp;	
		}
	}

}