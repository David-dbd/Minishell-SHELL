/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_wildcard.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:15:42 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/31 22:28:14 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../../inc/minishell.h"

int	extract_wildcard(t_shell *data, char *str, char **ptr)
{
	int	i;
	int	start;
	int	len;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '*')
		{
			i++;
			start = i;
			len = 0;
			while (str[i] != '\0' && str[i] != ' ' && str[i] != '\t' && str[i] != '*')
			{
				len++;
				i++;
			}
			if (len > 0)
			{
				*ptr = ft_strndup(str + start, len);
				if (!*ptr)
					exit_error(data, ERR_MALLOC, EXIT_FAILURE);
				return (SUCCESS);
			}
			// No pattern after *, continue searching
		}
		else
			i++;
	}
	return (FAILURE);
}
