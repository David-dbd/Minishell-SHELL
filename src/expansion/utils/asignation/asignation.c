/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asignation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:35:11 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/09/17 19:17:56 by migarrid         ###   ########.fr       */
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
	*key = ft_calloc(len + 1, sizeof(char));
	*value = ft_calloc(len + 1, sizeof(char));
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
	while (token->value[*i] != '=' && token->value[*i] != '\0')
	{
		(*key)[*i] = token->value[*i];
		(*i)++;
	}
}

static void	aux_value_asig(t_token *token, char **value, int *i)
{
	int	j;

	j = 0;
	if (token->value[*i] == '=')
		(*i)++;
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

static int	verify_if_already_set(t_shell *data, char *key, char **value, int t)
{
	t_var	*var;

	var = data->env.vars;
	while (var)
	{
		if (ft_strcmp(var->key, key) == 0)
		{
			free (var->value);
			var->value = NULL;
			var->value = *value;
			if (var->type == LOCAL && t == ENV)
				var->type = ENV;
			return (TRUE);
		}
		var = var->next;
	}
	return (FALSE);
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
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	aux_key_asig(token, &key, &i);
	aux_value_asig(token, &value, &i);
	if (verify_if_already_set(data, key, &value, type) == TRUE)
	{
		free (key);
		return (SUCCESS);
	}
	if (type == ENV)
		data->env.size++;
	add_var(data, key, value, type);
	return (SUCCESS);
}
