int	isnumeric(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] < '0'|| str[i] > '9')
		return (1);
        i++;
    }
	return (0);
}
