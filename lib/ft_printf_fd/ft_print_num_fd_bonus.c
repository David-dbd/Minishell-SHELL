/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_num_fd_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 23:30:57 by migarrid          #+#    #+#             */
/*   Updated: 2025/06/28 16:43:52 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

int	ft_print_num_fd(int n, t_format *fmt, int fd)
{
	int		len;
	char	*str;

	str = ft_itoa(n);
	if (!str)
		return (-1);
	ft_apply_flags(&str, fmt);
	if (!str)
		return (-1);
	len = write(fd, str, ft_strlen(str));
	free(str);
	return (len);
}
