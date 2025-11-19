/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args_for_binary.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 23:03:38 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/19 01:19:58 by davdiaz-         ###   ########.fr       */
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

static void	aux_alloc_mem(t_shell *data, char ***args, int n_args)
{
	*args = ft_calloc(n_args + 1, sizeof(char *));
	if (!*args)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
}

static int	extract_bin_arg(t_shell *d, char **arg_extract, char *word)
{
	//if (!word || word[0] == '\0')
	//	return (FAILURE);
	*arg_extract = ft_strdup(word);
	if (!*arg_extract)
		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
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
			|| tokens[*i].type == WILDCARD))//&& tokens[*i].value && tokens[*i].value[0] != '\0'
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
	aux_alloc_mem(data, &args, n_args);
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
		//if (tokens[j].value && tokens[j].value[0] != '\0')
		//{
		if (extract_bin_arg(data, &args[k], tokens[j].value) == SUCCESS)
			k++;
		//}
		j++;
	}
	if (k == 0)
		return (free(args), NULL);
	return (args);
}
