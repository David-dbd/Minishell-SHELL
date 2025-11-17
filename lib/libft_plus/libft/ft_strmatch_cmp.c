/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmatch_cmp.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 19:54:17 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/17 16:39:02 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

int	ft_strmatch_cmp(const char *s1, const char *s2)
{
	size_t	i;

	if (!s1 || !s2)
		return (0);
	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0'
		&& ft_tolower(s1[i]) == ft_tolower(s2[i]))
	{
		i++;
	}
	return (ft_tolower(s1[i]) - ft_tolower(s2[i]));
}
