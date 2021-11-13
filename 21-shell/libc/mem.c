void memcpy(char* source, char* dest, int lenth)
{
	int i = 0;

	for(i = 0; i < lenth; i++)
		dest[i] = source[i];
}

void memset(char* dest, char value, int lenth)
{
	int i = 0;

	for(i = 0; i < lenth; i++)
		dest[i] = value;
}
