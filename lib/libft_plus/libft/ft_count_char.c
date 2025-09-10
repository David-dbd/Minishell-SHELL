#include "../libft_plus.h"

int ft_count_char(char *buffer, char c)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (buffer[i] != '\0')
	{
		if (buffer[i] == c)
			counter++;
		i++;
	}
	return (counter);
}
