/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asignation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:35:11 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/28 13:39:11 by davdiaz-         ###   ########.fr       */
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
	int	j;

	j = 0;
	while (token->value[*i] != '=' && token->value[j] != '\0')
	{
		if (token->value[*i] == '+')
		{
			(*i)++;
			continue ;
		}
		(*key)[j] = token->value[*i];
		(*i)++;
		j++;
	}
	(*key)[j] = '\0';
	//printf("key: %s\n\n", *key);
}

static void	aux_value_asig(t_token *token, char **value, int *i, int type)
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
	//printf("value: %s\n\n", *value);
}

/*
	Si el type de la asignación es de PLUS_ASIGNATION y no existe aún en nuestra
	lista enlazada, entonces hay que averiguar su naturaleza, ya que de lo
	contrario se guardaria como PLUS_ASIGNATION dentro de la lista, lo cual
	sería erroneo y daría problemas.

	Por esta razón, is_it_env_or_local averigua cual es su naturaleza/tipo
	para poderla guardar como tal.
	Su funcionamiento es sencillo. Simplemente recorre hacia atras desde el
	token en cuestion hasta encontrar un "export", de modo que es de tipo ENV
	o de tipo LOCAL si no llega a encontrar un export
*/

static void	is_it_env_or_local(t_shell *data, int *type, int i)
{
	while (i > 0)
	{
		if (data->prompt.tokens[i - 1].type
			&& (data->prompt.tokens[i - 1].type == BUILT_IN
				&& ft_strcmp(data->prompt.tokens[i - 1].value,
					BUILTIN_EXPORT) == 0))
		{
			*type = ENV;
			return ;
		}
		else if (data->prompt.tokens[i - 1].type
			&& is_delimiter_type(data->prompt.tokens[i - 1].type))
		{
			*type = LOCAL;
			return ;
		}
		i--;
	}
	if (i == 0)
		*type = LOCAL;
}

int	asignation(t_shell *data, t_token *token, int type)
{
	char	*key;
	char	*value;
	int		result;
	int		i;

	result = FALSE;
	key = NULL;
	value = NULL;
	i = 0;
	if (aux_mem_alloc(&key, &value, ft_strlen(token->value)) == ERROR)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	aux_key_asig(token, &key, &i);
	aux_value_asig(token, &value, &i, type);
	result = verify_if_already_set(data, key, &value, type);
	if (result == TRUE)
	{
		free (key);
		return (SUCCESS);
	}
	else if (result == IGNORE)
	{
		free (key);
		free (value);
	}
	if (result == FALSE)
	{
		if (type == PLUS_ASIGNATION)
			is_it_env_or_local(data, &type, token->id);
		add_var(data, key, value, type);
	}
	return (SUCCESS);
}
