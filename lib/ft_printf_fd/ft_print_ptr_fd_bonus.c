/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ptr_fd_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 23:30:53 by migarrid          #+#    #+#             */
/*   Updated: 2025/06/28 16:49:25 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

static int	ptr_nill_fd(t_format *fmt, int fd)
{
	int		len;
	char	*str;

	str = ft_strdup("(nil)");
	if (!str)
		return (-1);
	ft_apply_flags(&str, fmt);
	if (!str)
		return (-1);
	len = write(fd, str, ft_strlen(str));
	free(str);
	return (len);
}

int	ft_print_ptr_fd(void *ptr, t_format *fmt, int fd)
{
	int		len;
	char	*str;
	char	*tmp;

	if (!ptr)
		return (ptr_nill_fd(fmt, fd));
	tmp = ft_utoa_base((uintptr_t)ptr);
	if (!tmp)
		return (-1);
	str = ft_strjoin("0x", tmp);
	free(tmp);
	if (!str)
		return (-1);
	ft_apply_flags(&str, fmt);
	if (!str)
		return (-1);
	len = write(fd, str, ft_strlen(str));
	free(str);
	return (len);
}
