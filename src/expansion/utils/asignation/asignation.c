/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asignation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 22:35:11 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/07 15:38:24 by davdiaz-         ###   ########.fr       */
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

/*static int	verify_if_already_set(t_shell *data, char *key, char **value, int t)
{
	t_var	*var;
	char	*tmp;

	var = data->env.vars;
	while (var)
	{
		if (ft_strcmp(var->key, key) == 0)
		{
			printf("Found it-> %s=%s\n\n", key, *value);
			if (t == EXP) // si solo es una palabra sin ""="" -> hola
				return (IGNORE);
			if (var->value && (t == LOCAL || t == ENV)) // si existe su valor y es local o env, solo hay que cambiarlo
			{
				printf("if t == LOCAL\n");
				if (var->value)
					free (var->value);
				var->value = *value;
			}
			if (!var->value && t != EXP) // Si no existe su valor, solo hay que agregarselo
				var->value = *value;
			if (var->value && t == PLUS_ASIGNATION) //Si existe su valor y es PLUS_ASIGNATION entonces hay que sumarla
			{
				printf("if t == PLUS_ASIG\n");
				printf("%s=%s\n\n", key, *value);
				tmp = ft_strjoin(var->value, *value);
				if (!tmp)
				{
					free(key);
					free(*value);
					exit_error(data, ERR_MALLOC, EXIT_FAILURE);
				}
				free (var->value);
				var->value = tmp;
				printf("var->value: %s\n\n", var->value);
			}
			if ((var->type == LOCAL || var->type == EXP)
				&& (t == ENV || t == PLUS_ASIGNATION)
				|| (var->type == EXP && t == LOCAL))
				var->type = ENV;
			return (TRUE);
		}
		var = var->next;
	}
	return (FALSE);
}*/

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
		data->env.size++;
		if (type == PLUS_ASIGNATION)
			is_it_env_or_local(data, &type, token->id);
		add_var(data, key, value, type);
	}
	return (SUCCESS);
}
