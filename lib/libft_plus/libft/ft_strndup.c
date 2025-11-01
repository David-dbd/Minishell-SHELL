/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 13:03:16 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/31 16:44:56 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

char	*ft_strndup(const char *str, size_t n)
{
	size_t	i;
	char	*new_str;

	if (!str)
		return (NULL);
	new_str = (char *)malloc((n + 1) * sizeof(char));
	if (!new_str)
		return (NULL);
	i = 0;
	while (i < n && str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}
