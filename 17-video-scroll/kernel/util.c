void memory_copy(char* source, char* dest, int nbytes)
{
	int i = 0;
	for(i = 0; i < nbytes; i++)
		dest[i] = source[i];
}

void int_to_ascii(int n, char str[])
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
}
