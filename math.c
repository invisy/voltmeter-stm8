#include "math.h"

int abs(int x)
{
	if(x < 0)
		return (-1)*x;
		
	return x;
}

int pow(int x, unsigned int y)
{
	if (y == 0)
		return 1;
	else if ((y % 2) == 0)
		return pow(x, y / 2) * pow(x, y / 2);
	else
		return x * pow(x, y / 2) * pow(x, y / 2);
}

int min(int x, int y)
{
	if(x < y)
		return x;
	else
		return y;
}

int float_to_digits(float x, unsigned char precision, char* out_decimal_part)
{
	char i=0;
	char buffer[6];
	int integer_part = (int)x;
	float decimal_part = x-integer_part;
	
	precision = min(precision, 6);
	
	for(i=0;i<precision+1;i++)
	{
		float decimal_x10 = decimal_part*10;
		int integer_x10 = decimal_x10;
		decimal_part = decimal_x10-integer_x10;
		buffer[i] = integer_x10;
	}
	
	if(buffer[precision] >= 5)
	{
		buffer[precision-1] += 1;
		
		for(i=precision-1;i>0;i--)
		{
			if(buffer[i] > 9)
			{
				buffer[i] = 0;
				buffer[i-1] += 1;
			}
		}
	}
	
	if(buffer[0] > 9)
	{
		buffer[0] = 0;
		integer_part += 1;
	}
	
	for(i=0;i<precision;i++)
		out_decimal_part[i] = buffer[i];
	
	return integer_part;
}

void insertion_sort(float arr[], int n)
{
	int i, j;
	float key;
	for (i = 1; i < n; i++) 
	{
		key = arr[i];
		j = i - 1;
  
		while (j >= 0 && arr[j] > key) 
		{
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = key;
	}
}