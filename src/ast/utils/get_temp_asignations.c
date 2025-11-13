/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_temp_asignations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:54:11 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/13 00:24:48 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	Verifica si el token[i - 1] es una asignación temporal y la guarda como
	parte del contexto/metadata del comando que le sigue.
*/

static void	aux_alloc_mem(t_shell *data, char ***args, int n_args)
{
	*args = ft_calloc(n_args + 1, sizeof(char *));
	if (!*args)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
}

static void	extract_bin_arg(t_shell *d, char **arg_extract, char *word)
{
	*arg_extract = ft_strdup(word);
	if (!*arg_extract)
		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
}

/*
	Esto es porque después de restar a start, puede ser -1 o estar en un token
	que no corresponda a una asignacion temporal, de modo que hay que ubicar
	start en la TEMP_ASIG.
	Eg.
	(var=hola ls) -> start--; -> token[start].type == PAREN_OPEN
	var=hola ls -> start--; -> start == -1
*/

static int	get_correct_index(t_token *tokens, int n_tokens, int start)
{
	if (start >= 1 && is_asignation_type(tokens[start - 1].type))
	{
		start--; //quedar en la temp_asig
		while (start >= 0 && is_asignation_type(tokens[start].type)) //iterar sobre las temp_asig
			start--;
		if (start < 0 && (start < n_tokens || !is_asignation_type(tokens[start].type))) //sumar un indice si es que no se esta en 0
			start++;
	}
	return (start);
}

char	**get_temp_asignations(t_shell *data, t_token *tokens, int i)
{
	char	**args;
	int		temp_count;
	int		start;

	if (i == 0)
		return (NULL);
	start = get_correct_index(tokens, data->prompt.n_tokens, i);
	if (start == i)
		return (NULL);//No hay temp_asig
	temp_count = 0;
	while (start < i && (tokens[start].type == TEMP_ASIGNATION
		|| tokens[start].type == TEMP_PLUS_ASIGNATION))
	{
		temp_count++;
		start++;
	}
	if (temp_count == 0)
		return (NULL);
	aux_alloc_mem(data, &args, temp_count);
	start = i - temp_count;
	temp_count = 0;
	while (start < i)
	{
		extract_bin_arg(data, &args[temp_count], tokens[start].value);
		start++;
		temp_count++;
	}
	return (args);
}

