/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_float_fd_utils_bonus.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:30:00 by migarrid          #+#    #+#             */
/*   Updated: 2025/06/28 17:30:00 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

long	ft_power_10(int exp)
{
	long	result;

	result = 1;
	while (exp > 0)
	{
		result *= 10;
		exp--;
	}
	return (result);
}

int	ft_is_special(double f, char **result)
{
	if (f != f)
	{
		*result = ft_strdup("nan");
		return (1);
	}
	if (f == 1.0 / 0.0)
	{
		*result = ft_strdup("inf");
		return (1);
	}
	if (f == -1.0 / 0.0)
	{
		*result = ft_strdup("-inf");
		return (1);
	}
	return (0);
}

void	ft_add_zeros(char *result, int *pos, int count)
{
	while (count > 0)
	{
		result[(*pos)++] = '0';
		count--;
	}
}
