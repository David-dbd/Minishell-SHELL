/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_key.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:23:28 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/07 19:18:59 by migarrid         ###   ########.fr       */
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

int	copy_key(char *str, char **key_to_find)
{
	int	i;
	int	j;
	int	start;
	int	flag;

	i = 0;
	j = 0;
	flag = FALSE;
	while (str[i] != '\0')
	{
		if (str[i] == '$')
		{
			i++;
			if (!ft_isalpha(str[i]) && !is_symbol(str[i]))
				continue ;
			start = i;
			while ((ft_isalnum(str[start]) || is_symbol(str[start]))
				&& flag == FALSE)
			{
				if (str[start] == '$')
				{
					flag = TRUE;
					break ;
				}
				(*key_to_find)[j++] = str[start];
				start++;
			}
			(*key_to_find)[j] = '\0';
			return (SUCCESS);
		}
		i++;
	}
	return (FAILURE);
}
