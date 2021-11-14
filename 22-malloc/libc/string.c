#include "string.h"

void int_to_ascii(int n, char* str)
{
	// 12 => "12"  -14 => "-14"
	// 1<<31 == 2147483648
	int i = 0, sign = 0;
	char tmp_str[0x10] = {0};

	sign = n;
	if(n < 0)
		n = -n;

	do {
		tmp_str[i++] = n % 10 + '0';
	} while((n /= 10) > 0);

	if(sign < 0)
		tmp_str[i++] = '-';
	tmp_str[i] = '\x00';

	int lenth = i;
	for(i = 0; i < lenth; i++)
		str[lenth-i-1] = tmp_str[i];
	str[lenth] = '\x00';
}

void hex_to_ascii(int n, char* str)
{
	char s[0x10] = "0123456789abcdef";
	char tmp_str[0x10] = {0};
	int i = 0;

	do {
		tmp_str[i++] = s[n%0x10];
	} while((n /= 0x10) > 0);

	tmp_str[i] = 'x';
	tmp_str[i+1] = '0';

	int lenth = strlen(tmp_str);
	for(i = 0; i < lenth; i++)
		str[lenth-i-1] = tmp_str[i];
	str[lenth] = '\0';
}

int strlen(char* s)
{
	int i = 0;
	
	while(s[i])
		i += 1;
	
	return i;
}

int strcmp(char* s1, char* s2)
{
	int i = 0;
	
	for(i = 0; s1[i] == s2[i]; i++)
	{
		if(s1[i] == 0)
			return 0;
	}

	return s1[i] - s2[i];
}

void backspace(char* s)
{
	while(*s)
		s += 1;
	s -= 1;
	*s = '\0';
}

void add(char* s, char ch)
{
	while(*s)
		s += 1;
	*s = ch;
	s += 1;
	*s = '\0';
}
