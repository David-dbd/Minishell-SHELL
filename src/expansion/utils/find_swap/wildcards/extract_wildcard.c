/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_wildcard.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:15:42 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/13 01:03:22 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../../inc/minishell.h"

int	extract_wildcard(t_shell *data, char *str, char **ptr, int *wildcard_type)
{
	int	i;
	int	start;
	int	len;

	i = 0;
	len = 0;
	start = start;
	while (str[i] != '\0')
	{
		if (str[i] == '*')
		{
			/* Casito 1: '*' solo o '* ' */
			if ((str[i + 1] == '\0' || ft_isspace(str[i + 1])) //se puede eliminar el ft_Strdup aqui porque no hace nada y solo afecta en eficiencia
				&& (i == 0 || !ft_isalnum(str[i - 1])))// pero si se quita hay que hacer el if (key existe se libera) en lugar de liberarla sin revisar
			{
				// printf("CASE1\n\n");
				*ptr = ft_strdup("");
				if (!*ptr)
					exit_error(data, ERR_MALLOC, EXIT_FAILURE);
				*wildcard_type = ALL;
				return (SUCCESS);
			}
			/* Case 2: *palabra o *palabra* */
			if (str[i + 1] != '\0' && !ft_isspace(str[i + 1]))
			{
				// printf("CASE2\n\n");
				start = i;
				start++;
				while (str[start] != '\0' && !ft_isspace(str[start]) && str[start] != '*')
					start++;
				if (str[start] != '*')
					*wildcard_type = BEGINING;
				else
					*wildcard_type = MIDDLE;
				*ptr = ft_strndup(str + i + 1, start - 1);
				if (!*ptr)
					exit_error(data, ERR_MALLOC, EXIT_FAILURE);
				return (SUCCESS);
			}
			/* Caase 3: palabra* */
			if (i > 0 && ft_isalnum(str[i - 1]))
			{
				// printf("CASE3\n\n");
				start = 0;
				while (start < i && !ft_isalnum(str[start]))
					start++;
				*wildcard_type = END;
				*ptr = ft_strndup(str + start, i - start);
				if (!*ptr)
					exit_error(data, ERR_MALLOC, EXIT_FAILURE);
				return (SUCCESS);
			}
		}
		i++;
	}
	return (FAILURE);
}

