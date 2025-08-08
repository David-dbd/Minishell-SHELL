/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_float_fd_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 17:30:00 by migarrid          #+#    #+#             */
/*   Updated: 2025/06/28 17:57:19 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

static void	ft_copy_string_parts(char *result, char *int_part, char *frac_part,
		int precision)
{
	int	pos;

	pos = 0;
	while (int_part[pos])
	{
		result[pos] = int_part[pos];
		pos++;
	}
	if (precision > 0)
	{
		result[pos++] = '.';
		ft_add_zeros(result, &pos, precision - ft_strlen(frac_part));
		while (*frac_part)
			result[pos++] = *frac_part++;
	}
	result[pos] = '\0';
}

static char	*ft_build_float_str(double f, int precision, int is_neg)
{
	char	*result;
	char	*int_part;
	char	*frac_part;
	long	multiplier;
	long	temp_val;

	multiplier = ft_power_10(precision);
	f = f * multiplier + 0.5;
	temp_val = (long)f;
	int_part = ft_itoa(temp_val / multiplier);
	if (!int_part)
		return (NULL);
	frac_part = ft_itoa(temp_val % multiplier);
	if (!frac_part)
		return (free(int_part), NULL);
	result = malloc
		(ft_strlen(int_part) + is_neg + (precision > 0) * (1 + precision) + 1);
	if (!result)
		return (free(int_part), free(frac_part), NULL);
	if (is_neg)
		result[0] = '-';
	ft_copy_string_parts(result + is_neg, int_part, frac_part, precision);
	return (free(int_part), free(frac_part), result);
}

static char	*ft_float_to_str(double f, int precision)
{
	char	*result;
	int		is_negative;

	if (ft_is_special(f, &result))
		return (result);
	is_negative = (f < 0.0);
	if (is_negative)
		f = -f;
	return (ft_build_float_str(f, precision, is_negative));
}

int	ft_print_float_fd(double f, t_format *fmt, int fd)
{
	char	*str;
	int		precision;
	int		len;

	if (fmt->precision == -1)
		precision = 6;
	else
		precision = fmt->precision;
	str = ft_float_to_str(f, precision);
	if (!str)
		return (-1);
	ft_apply_flags(&str, fmt);
	len = ft_strlen(str);
	write(fd, str, len);
	free(str);
	return (len);
}
