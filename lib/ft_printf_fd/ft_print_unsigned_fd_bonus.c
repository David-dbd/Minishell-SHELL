/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned_fd_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 22:10:56 by migarrid          #+#    #+#             */
/*   Updated: 2025/06/28 16:44:31 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

int	ft_print_unsigned_fd(unsigned int n, t_format *fmt, int fd)
{
	int		len;
	char	*str;

	str = ft_itoa_base((long)n, "0123456789");
	if (!str)
		return (-1);
	ft_apply_flags(&str, fmt);
	if (!str)
		return (-1);
	len = write(fd, str, ft_strlen(str));
	free(str);
	return (len);
}
