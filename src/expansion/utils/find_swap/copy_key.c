/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_key.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:23:28 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/10 21:24:03 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

int	copy_key(char *buffer, char **key_to_find)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	j = 0;
	while (buffer[i] != '\0')
	{
		if (buffer[i] == '$')
		{
			i++;
			if (!ft_isalpha(buffer[i]))
				continue ;
			start = i;
			while (ft_isalpha(buffer[start]) || (*key_to_find)[j] != '\0')
			{
				(*key_to_find)[j++] = buffer[start];
				start++;
			}
			(*key_to_find)[j] = '\0';
			return (SUCCESS);
		}
		i++;
	}
	return (FAILURE);
}
