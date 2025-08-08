/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex_fd_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 23:31:00 by migarrid          #+#    #+#             */
/*   Updated: 2025/06/28 16:43:42 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

int	ft_print_hex_fd(unsigned int n, t_format *fmt, int uppercase, int fd)
{
	int		len;
	char	*base;
	char	*str;

	if (uppercase)
		base = "0123456789ABCDEF";
	else
		base = "0123456789abcdef";
	str = ft_itoa_base((unsigned long)n, base);
	if (!str)
		return (-1);
	ft_apply_flags(&str, fmt);
	if (!str)
		return (-1);
	len = write(fd, str, ft_strlen(str));
	free(str);
	return (len);
}
