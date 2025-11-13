/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol_range.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 15:06:26 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/08 16:05:55 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

bool	ft_atol_range(const char *nptr)
{
	int			i;
	int			sign;
	__int128	result;

	if (!nptr)
		return (0);
	i = 0;
	sign = 1;
	result = 0;
	while (nptr[i] == ' ' || (nptr[i] >= '\t' && nptr[i] <= '\r'))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(nptr[i]))
	{
		result = (result * 10) + (nptr[i] - '0');
		if ((sign * result) > LONG_MAX || (sign * result) < LONG_MIN)
			return (false);
		i++;
	}
	return (true);
}
/*#include <stdio.h>
int	main(int ac, char **av)
{
	if (ac == 2)
	{
		printf("Number: %d\n", ft_atoi_range(av[1]));
		return (0);
	}
	return (0);
}*/
