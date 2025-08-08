/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 00:32:45 by migarrid          #+#    #+#             */
/*   Updated: 2025/06/28 16:10:48 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

static int	ft_atoi_advance(const char **str)
{
	int			num;
	const char	*start;

	start = *str;
	num = ft_atoi(start);
	while (ft_isdigit(**str))
		(*str)++;
	return (num);
}

static int	ft_specifiers_fd(char specify, va_list args, t_format *fmt, int fd)
{
	if (specify == 'c')
		return (ft_print_char_fd(va_arg(args, int), fmt, fd));
	else if (specify == 's')
		return (ft_print_str_fd(va_arg(args, char *), fmt, fd));
	else if (specify == 'p')
		return (ft_print_ptr_fd(va_arg(args, void *), fmt, fd));
	else if (specify == 'd' || specify == 'i')
		return (ft_print_num_fd(va_arg(args, int), fmt, fd));
	else if (specify == 'u')
		return (ft_print_unsigned_fd(va_arg(args, unsigned int), fmt, fd));
	else if (specify == 'x' || specify == 'X')
		return (ft_print_hex_fd
			(va_arg(args, unsigned int), fmt, specify == 'X', fd));
	else if (specify == 'f')
		return (ft_print_float_fd(va_arg(args, double), fmt, fd));
	else if (specify == '%')
		return (write(fd, "%", 1));
	return (0);
}

static void	process_flags(const char **str, t_format *fmt)
{
	if (**str == '-')
		fmt->left_align = 1;
	else if (**str == '0')
		fmt->zero_pad = 1;
	else if (**str == '#')
		fmt->hash = 1;
	else if (**str == ' ')
		fmt->space = 1;
	else if (**str == '+')
		fmt->plus = 1;
	else if (**str == '.')
	{
		(*str)++;
		fmt->precision = 0;
		if (ft_isdigit(**str))
			fmt->precision = ft_atoi_advance(str);
		return ;
	}
	else if (ft_isdigit(**str))
	{
		fmt->width = ft_atoi_advance(str);
		return ;
	}
	(*str)++;
}

static int	ft_flags_fd(char const **str, va_list args, int fd)
{
	t_format	fmt;

	fmt = (t_format){0, -1, 0, 0, 0, 0, 0, 0};
	while (**str && !ft_strchr("cspdiuxXf%", **str))
		process_flags(str, &fmt);
	fmt.specifier = **str;
	return (ft_specifiers_fd(fmt.specifier, args, &fmt, fd));
}

int	ft_printf_fd(int fd, char const *str, ...)
{
	va_list	args;
	int		count;
	int		result;

	count = 0;
	va_start(args, str);
	while (*str)
	{
		if (*str == '%')
		{
			str++;
			result = ft_flags_fd(&str, args, fd);
			if (result == -1)
			{
				va_end(args);
				return (-1);
			}
			count = count + result;
		}
		else
			count = count + write(fd, str, 1);
		str++;
	}
	va_end(args);
	return (count);
}
