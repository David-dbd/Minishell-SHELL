/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmatch_cmp.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 19:54:17 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/05 22:44:22 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft_plus.h"

static void	ft_compare_and_transform(char **temp, const char *s2)
{
	int	i;

	i = 0;
	while ((*temp)[i] != '\0' && s2[i] != '\0')
	{
		if (ft_tolower((*temp)[i]) == s2[i])
			(*temp)[i] = ft_tolower((*temp)[i]);
		else if (ft_toupper((*temp)[i]) == s2[i])
			(*temp)[i] = ft_toupper((*temp)[i]);
		i++;
	}
}

int	ft_strmatch_cmp(char *s1, const char *s2, int *error)
{
	char	*temp;
	int		result;

	temp = ft_strdup(s1);
	if (!temp)
	{
		*error = -1;
		return (-1);
	}
	ft_compare_and_transform(&temp, s2);
	result = ft_strcmp(temp, s2);
	free (temp);
	return (result);
}

// static void	ft_compare_and_transform(char **temp, const char *s2)
// {
// 	int	i;

// 	i = 0;
// 	while ((*temp)[i] != '\0' && s2[i] != '\0')
// 	{
// 		if (ft_tolower((*temp)[i]) == s2[i])
// 			(*temp)[i] = ft_tolower((*temp)[i]);
// 		else if (ft_toupper((*temp)[i]) == s2[i])
// 			(*temp)[i] = ft_toupper((*temp)[i]);
// 		i++;
// 	}
// }

// int	ft_strmatch_cmp(char *s1, const char *s2)
// {
// 	char	*temp;
// 	int		result;

// 	if (!s1 || !s2)
// 		return (0);
// 	temp = ft_strdup(s1);
// 	if (!temp)
// 		return (-1);
// 	ft_compare_and_transform(&temp, s2);
// 	result = ft_strcmp(temp, s2);
// 	free (temp);
// 	return (result);
// }
