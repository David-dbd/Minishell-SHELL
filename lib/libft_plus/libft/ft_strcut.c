/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcut.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 16:58:49 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/04 17:09:52 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

char	*ft_strcut(char *str, int c)
{
	char	*chr;

	if (!str || !*str)
		return (NULL);
	chr = ft_strchr(str, c);
	if (chr)
		*chr = '\0';
	return (str);
}
