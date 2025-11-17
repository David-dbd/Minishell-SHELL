/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args_for_binary.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 23:03:38 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/17 17:43:11 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	En esta funcion se verifica cuantos argumnetos tiene un cmd para poder crear
	un **args y poder retornarlo ya construido, transformando cada token WORD
	en un arg.

	Logica:
	1. Se cuentan cuantos args hay.
	2. Se crea el **args
	3. Se vuevlen a recorrer los tokens, esta vez haciendo un strlen sobre cada
		uno para poder saber su len y poder crear el str en uno de los espacios
		de **args
	4. Se avanza index del arbol ya que esta función se llama desde la
		construccion del arbol, de modo que omitimos todos los args de cada cmd
		y redirs ya que ninguno de estos deberian contarse como un nodo.
*/

static int	aux_alloc_mem(t_shell *data, char ***args, int n_args)
{
	*args = ft_calloc(n_args + 1, sizeof(char *));
	if (!*args)
	{
		ft_printf_fd(STDERR, ERR_MALLOC);
		data->error_state = TRUE;
		return (FAILURE);
	}
	return (SUCCESS);
}

static int	extract_bin_arg(t_shell *data, char **arg_extract, char *word)
{
	if (!word)
		return (FAILURE);
	*arg_extract = ft_strdup(word);
	if (!*arg_extract)
	{
		ft_free_str_array(&arg_extract);
		data->error_state = TRUE;
		return (ERROR);
	}
	return (SUCCESS);
}

static void	arg_count(t_token *tokens, int n_tokens, int *i, int *n_args)
{
	while (*i < n_tokens)
	{
		if (is_delimiter_type(tokens[*i].type))
			break ;
		if (is_redir_type(tokens[*i].type))
		{
			while (*i < n_tokens && (!is_arg_type(tokens[*i].type)
					&& tokens[*i].type != COMMAND))
			{
				if (is_delimiter_type(tokens[*i].type))
					break ;
				(*i)++;
			}
		}
		if ((is_arg_type(tokens[*i].type) || tokens[*i].type == COMMAND
				|| tokens[*i].type == NO_SPACE || tokens[*i].type == INDIFERENT
				|| tokens[*i].type == WILDCARD) && tokens[*i].value)
			(*n_args)++;
		if (is_delimiter_type(tokens[*i].type))
			break ;
		(*i)++;
	}
}

char	**get_args_for_binary(t_shell *data, t_token *tokens, int *i)
{
	char	**args;
	int		n_args;
	int		j;
	int		k;

	if (tokens[*i].type == BUILT_IN)
		safe_index_plus(i, data->prompt.n_tokens);
	j = *i;
	k = 0;
	n_args = 0;
	arg_count(tokens, data->prompt.n_tokens, i, &n_args);
	if (n_args == 0)
		return (NULL);
	if (!aux_alloc_mem(data, &args, n_args))
		return (NULL);
	while (k < n_args && j < data->prompt.n_tokens)
	{
		if (is_delimiter_type(tokens[j].type))
			break ;
		if (is_redir_type(tokens[j].type)
			|| tokens[j].type == FILENAME || tokens[j].type == DELIMITER)
		{
			j++;
			continue ;
		}
		if (tokens[j].value)
		{
			if (extract_bin_arg(data, &args[k], tokens[j].value) == SUCCESS)
				k++;
			if (data->error_state == TRUE)
				return (NULL);
		}
		j++;
	}
	if (k == 0)
	{
		ft_free_str_array(&args);
		return (NULL);
	}
	return (args);
}
