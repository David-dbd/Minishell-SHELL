/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args_for_binary.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 23:03:38 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/17 19:19:33 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

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

char	**group_args_for_binary(t_shell *data, t_token *token)
{
	char	**args;
	int		n_args;
	int		n_chars;
	int		i;

	i = 0;
	n_args = 0;
	if (token->type == COMMAND)
		i++;
	while (token[i].type == WORD || token[i].type == ASIGNATION)
		n_args++;
	aux_alloc_mem(data, &args, n_args);
	while (token[i].type && !is_delimiter_type(token[i].type))
	{
		n_chars = ft_strlen(token[i].value);
		extract_bin_arg(data, args[i], token[i].value, n_chars);
		i++;
	}
	return (args);
}
