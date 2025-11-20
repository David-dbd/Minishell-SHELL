/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_key.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:23:28 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/20 23:56:15 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	str es token->val. Se hace un recorrido de str, ya que si asumimos
	que siempre el '$' estará al inicio, no podriamos validar casos como:
	Hello $USER. Es por esto que hay que recorrer el str, de modo que podamos
	ignorar todos aquellos caracteres que estan antes del '$' y poder copiar
	aquellos que estan depués del mismo (siemore y cuando sea una sintaxis
	válida).
*/

static int	if_find_dollar(char *str, char **key_to_find, int i)
{
	int	j;

	j = 0;
	if (str[i] == '$')
	{
		i++;
		if (is_symbol(str[i])) //tratar los simbolos de manera independiente. $$ $! $? y ademas el export "$$USER" ahora si funcionaria
		{
			(*key_to_find)[j++] = str[i];
			(*key_to_find)[j] = '\0';
			return (TRUE);
		}
		if (!ft_isalpha(str[i]) && str[i] != '_')
			return (FALSE);
		while (ft_isalnum(str[i]) || str[i] == '_')
		{
			(*key_to_find)[j++] = str[i];
			i++;
		}
		(*key_to_find)[j] = '\0';
		return (TRUE);
	}
	return (FALSE);
}

static void	aux_copy_tilde(char *str, char **key_to_find, int i)
{
	int	j;
	int	flag;

	j = 0;
	flag = FALSE;
	(*key_to_find)[j++] = '~';
	while ((str[i] == '+' || str[i] == '-' || str[i] == '/') && flag == FALSE)
	{
		if (str[i] == '$' || str[i] == '~')
		{
			flag = TRUE;
			break ;
		}
		(*key_to_find)[j++] = str[i];
		i++;
	}
	(*key_to_find)[j] = '\0';
}

static int	if_find_tilde(char *str, char **key_to_find, int i)
{
	int	j;

	j = 0;
	if (str[i] == '~')
	{
		if (str[i + 1] == '\0' || str[i + 1] == ' ')
		{
			(*key_to_find)[j++] = str[i];
			(*key_to_find)[j] = '\0';
			return (TRUE);
		}
		i++;
		if (str[i] != '+' && str[i] != '/' && str[i] != '-')
			return (FALSE);
		if (str[i] == '+' && str[i + 1]
			&& ((str[i + 1] == '+') || (str[i + 1] != '/'))
			|| (str[i] == '-' && str[i + 1]
				&& ((str[i + 1] == '-') || (str[i + 1] != '/'))))
			return (FALSE);
		aux_copy_tilde(str, key_to_find, i);
		return (TRUE);
	}
	return (FALSE);
}

int	copy_key(char *str, char **key_to_find, int *type)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (if_find_dollar(str, key_to_find, i))
		{
			*type = DOLLAR;
			return (SUCCESS);
		}
		if (if_find_tilde(str, key_to_find, i))
		{
			*type = TILDE;
			return (SUCCESS);
		}
		i++;
	}
	return (FAILURE);
}
