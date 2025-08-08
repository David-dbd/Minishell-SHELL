/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_char_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 23:31:04 by migarrid          #+#    #+#             */
/*   Updated: 2025/06/28 16:07:03 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

int	ft_print_char_fd(char c, t_format *fmt, int fd)
{
	int		len;
	int		pad_len;
	char	pad_char;

	if (fmt->width > 1)
		len = fmt->width;
	else
		len = 1;
	pad_len = len - 1;
	pad_char = ' ';
	if (fmt->zero_pad && !fmt->left_align)
		pad_char = '0';
	if (!fmt->left_align)
	{
		while (pad_len-- > 0)
			write(fd, &pad_char, 1);
		write(fd, &c, 1);
	}
	else
	{
		write(fd, &c, 1);
		while (pad_len-- > 0)
			write(fd, " ", 1);
	}
	return (len);
}
