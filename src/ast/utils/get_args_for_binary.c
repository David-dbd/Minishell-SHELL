/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args_for_binary.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 23:03:38 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/05 18:05:27 by migarrid         ###   ########.fr       */
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
	4. Se avanza index ya que esta funcion se llama desde la construccion del
		arbol de modo que se tiene que omitir todos los args de cada cmd.

*/

static void	aux_alloc_mem(t_shell *data, char ***args, int n_args)
{
	*args = ft_calloc(n_args + 1, sizeof(char *));
	if (!*args)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
}

static void	extract_bin_arg(t_shell *d, char **arg_extract, char *word, int len)
{
	*arg_extract = ft_strdup(word);
	if (!*arg_extract)
		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
}

char	**get_args_for_binary(t_shell *data, t_token *tokens, int *i)
{
	char	**args;
	int		n_args;
	int		arg_len;
	int		j;
	int		k;

	k = 0;
	j = *i;
	n_args = 0;
	while (*i < data->prompt.n_tokens && is_arg_type(tokens[*i].type))
	{
		n_args++;
		(*i)++;
	}
	if (n_args == 0)
		return (NULL);
	aux_alloc_mem(data, &args, n_args);
	while (k < n_args && j < data->prompt.n_tokens && is_arg_type(tokens[j].type))
	{
		arg_len = ft_strlen(tokens[j].value);
		extract_bin_arg(data, &args[k], tokens[j].value, arg_len);
		k++;
		j++;
	}
	return (args);
}
