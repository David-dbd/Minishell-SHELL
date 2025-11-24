/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_ambiguous_redir.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 01:02:29 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/24 14:53:09 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

int	is_filename_expanded(t_shell *data, t_redir *redir, t_token *tokens)
{
	int	i;

	i = 0;
	while (i < data->prompt.n_tokens)
	{
		if (redir->filename_hash == tokens[i].hash)
		{
			if (strcmp(redir->filename, tokens[i].value) != 0)
			{
				ft_free((void **)&redir->filename);
				redir->filename = ft_strdup(tokens[i].value);
				if (!redir->filename)
					return (exit_error(data, ERR_MALLOC, EXIT_FAIL));
				return (SUCCESS);
			}
			return (SUCCESS);
		}
		i++;
	}
	return (FAILURE);
}

int	check_ambiguous_redir(t_shell *data, t_redir *redir, int mode)
{
	if (redir->type == REDIR_HEREDOC)
		return (OK);
	if (!redir->filename)
		return (FAIL);
	if (redir->filename[0] == '*' && redir->filename[1] == '\0')
	{
		if (mode == CHILD)
			return (exit_error(data, ERR_AMBIGUOUS_REDIR, EXIT_FAILURE, "*"));
		if (mode == FATHER || mode == SUBSHELL)
			return (ft_printf_fd(STDERR, ERR_AMBIGUOUS_REDIR, "*"), FAIL);
	}
	else if (!is_filename_expanded(data, redir, data->prompt.tokens))
	{
		if (mode == CHILD)
			return (exit_error(data, ERR_AMBIGUOUS_REDIR, EXIT_FAILURE, redir->filename));
		if (mode == FATHER)
			return (ft_printf_fd(STDERR, ERR_AMBIGUOUS_REDIR, redir->filename), FAIL);
	}
	return (OK);
}
