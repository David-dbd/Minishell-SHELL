/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asignation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:35:11 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/09/10 21:57:01 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Propósito:
	*Realiza la asignación de una variable, extrayendo
	clave y valor del token.

	Lógica:
	*Reserva memoria para key y value.
	*Extrae la clave hasta el '=' usando aux_key_asig().
	*Extrae el valor ignorando comillas y punto y coma con aux_value_asig().
	*Si el valor está vacío, se asigna una cadena vacía.
	*Añade la variable al entorno con add_var().
*/

static int	aux_mem_alloc(char **key, char **value, int len)
{
	*key = (char *)malloc((len + 1) * sizeof(char));
	*value = (char *)malloc((len + 1) * sizeof(char));
	if (!*key || !*value)
	{
		free (*key);
		free (*value);
		return (ERROR);
	}
	return (SUCCESS);
}

static void	aux_key_asig(t_token *token, char **key, int *i)
{
	while (token->value[*i] != '=')
	{
		(*key)[*i] = token->value[*i];
		(*i)++;
	}
	(*key)[*i] = '\0';
	(*i)++;
}

static void	aux_value_asig(t_token *token, char **value, int *i)
{
	int	j;

	j = 0;
	while (token->value[*i] != '\0')
	{
		if (token->value[*i] != '\"' && token->value[*i] != '\''
			&& token->value[*i] != ';')
		{
			(*value)[j++] = token->value[*i];
		}
		(*i)++;
	}
	(*value)[j] = '\0';
}

int	asignation(t_shell *data, t_token *token, int type)
{
	char	*key;
	char	*value;
	int		len;
	int		i;

	len = ft_strlen(token->value);
	i = 0;
	if (aux_mem_alloc(&key, &value, len) == ERROR)
	{
		exit_error(data, ERR_MALLOC, EXIT_USE);
		return (ERROR);
	}
	aux_key_asig(token, &key, &i);
	aux_value_asig(token, &value, &i);
	add_var(data, key, value, type);
	return (SUCCESS);
}
