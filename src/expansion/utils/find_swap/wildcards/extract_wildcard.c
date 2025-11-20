/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_wildcard.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 16:15:42 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/20 23:20:50 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../../inc/minishell.h"

/*
	Caso 1 es cuando el WILDCARD es solo *
*/

static int	handle_case1(char *str, char **ptr, int *wildcard_type)
{
	*ptr = ft_strdup("");
	if (!*ptr)
		return (ERROR);
	*wildcard_type = ALL;
	return (SUCCESS);
}

/*
	Caso 2 es cuando el WILDCARD es:

	*file -> Al inicio/BEGINNIG
	*file* -> Encierra toda la palabra/MIDDLE
	*file*abc -> Casos complejos/COMPLEX
*/

static int	handle_case2(char *str, char **ptr, int *wildcard_type, int i)
{
	int	start;

	start = i + 1;
	while (str[start] != '\0' && !ft_isspace(str[start]) && str[start] != '*')
		start++;
	if (str[start] == '*' && (str[start + 1] != '\0' || (str[start + 1] == '\0'
			|| ft_isspace(str[start + 1]))))
	{
		*wildcard_type = COMPLEX;
		*ptr = ft_strdup(str);
		if (!*ptr)
			return (ERROR);
		return (SUCCESS);
	}
	if (str[start] != '*')
		*wildcard_type = BEGINING;
	*ptr = ft_strndup(str + i + 1, start - (i + 1));
	if (!*ptr)
		return (ERROR);
	return (SUCCESS);
}

/*
	Caso 3 es cuando el WILDCARD esta al final: file*
*/

static int	handle_case3(char *str, char **ptr, int *wildcard_type, int i)
{
	int	start;

	start = 0;
	while (start < i && !ft_isalnum(str[start]))
		start++;
	*wildcard_type = END;
	*ptr = ft_strndup(str + start, i - start);
	if (!*ptr)
		return (ERROR);
	return (SUCCESS);
}

/*
	Función para poder extraer y clasificar el tipo de WILDCARD.
	Puede ser de tipo BEGGINIG/MIDDLE/END/ALL/COMPLEX
*/

int extract_wildcard(t_shell *data, char *str, char **ptr, int *wildcard_type)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '*')
		{
			if ((str[i + 1] == '\0' || ft_isspace(str[i + 1])) //se puede eliminar el ft_Strdup aqui porque no hace nada y solo afecta en eficiencia
				&& (i == 0 || !ft_isalnum(str[i - 1])))
				result = handle_case1(str, ptr, wildcard_type);
			else if (str[i + 1] != '\0' && !ft_isspace(str[i + 1]))
				result = handle_case2(str, ptr, wildcard_type, i);
			else if (i > 0 && ft_isalnum(str[i - 1]))
				result = handle_case3(str, ptr, wildcard_type, i);
			if (result == ERROR)
				exit_error(data, ERR_MALLOC, EXIT_FAILURE);
			if (result != 0)
				return (result);
		}
		i++;
	}
	return (result);
}
