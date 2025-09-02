/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_trans_args_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 17:04:19 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/02 19:34:05 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

static int	is_cmd_type(int type)
{
	if (type == COMMAND || type == BUILT_IN || type == WORD)
		return (1);
	return (0);
}

static int	is_quote_type(int type)
{
	if (type == DOUBLE_QUOTE || type == SINGLE_QUOTE)
		return (1);
	return (0);
}

static int	is_redir_type(int type)
{
	if (type == REDIR_APPEND || type == REDIR_OUTPUT)
		return (1);
	return (0);
}

void	logic_trans_args_cmd(t_shell *data, t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens[i].type)
	{
		if ((i == 1 && is_cmd_type(tokens[i - 1].type))
			|| (i > 1 && is_cmd_type(tokens[i - 1].type)
				&& !is_redir_type(tokens[i - 2].type))
			|| (i > 1 && is_quote_type(tokens[i - 1].type)
				&& is_cmd_type(tokens[i - 2].type))
			|| (i > 3 && is_quote_type(tokens[i - 1].type)
				&& is_quote_type(tokens[i - 2].type)
				&& tokens[i - 3].type == NO_SPACE
				&& is_cmd_type(tokens[i - 4].type)))
		{
			if (is_quote_type(tokens[i].type))
			{
				if (is_cmd_type(tokens[i + 1].type))
					tokens[i + 1].type = WORD;
			}
			else if (is_cmd_type(tokens[i].type))
				tokens[i].type = WORD;
		}
		else if ((i > 0 && tokens[i - 1].type == NO_SPACE)
			|| (i > 1 && tokens[i - 2].type == NO_SPACE)
			|| (i > 2 && tokens[i - 3].type == NO_SPACE)
			|| (i > 3 && tokens[i - 4].type == NO_SPACE))
		{
			if (is_cmd_type(tokens[i].type))
				tokens[i].type = WORD;
		}
		i++;
	}
}
