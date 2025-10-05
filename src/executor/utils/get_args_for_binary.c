/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args_for_binary.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 23:03:38 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/05 15:17:41 by davdiaz-         ###   ########.fr       */
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

static void	aux_alloc_mem(t_shell *data, char ***args, int len)
{
	**args = ft_calloc(len + 1, sizeof(char *));
	if (args)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
}

static void	extract_bin_arg(t_shell *d, char **arg_extract, char *word, int len)
{
	*arg_extract = ft_strdup(word);
	if (!*arg_extract)
		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
}

char	**group_args_for_binary(t_shell *data, t_token *token, int *index)
{
	char	**args;
	int		n_args;
	int		arg_len;
	int		j;

	j = 0;
	n_args = 0;
	if (token->type == COMMAND)
	{
		j++;
		(*index)++;
	}
	while (index < data->prompt.n_tokens && is_arg_type(token[j].type))
	{
		n_args++;
		(*index)++;
	}
	aux_alloc_mem(data, &args, n_args);
	while (j < data->prompt.n_tokens && is_arg_type(token[j].type))
	{
		arg_len = ft_strlen(token[j].value);
		extract_bin_arg(data, &args[j], token[j].value, arg_len);
		j++;
	}
	return (args);
}
