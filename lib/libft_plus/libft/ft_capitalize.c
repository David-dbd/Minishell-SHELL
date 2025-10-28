/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_capitalize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:47:15 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/28 17:11:55 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

char *ft_capitalize(char *str)
{
	size_t	i;
	int	 	start;

	if (!str)
		return (NULL);
	i = 0;
	start = true;
	while (str[i] != '\0')
	{
		if (ft_isalnum(str[i]))
		{
			if (start && ft_isalpha(str[i]))
					str[i] = ft_toupper(str[i]);
			if (!start && ft_isalpha(str[i]))
					str[i] = ft_tolower(str[i]);
			start = false;
		}
		else
			start = true;
		i++;
	}
	return (str);
}
