/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redir_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:39:51 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/17 18:36:43 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

// void	check_redir_input(t_shell *data, t_prompt *p, t_token *t, int i)
// {
// 	if (tokens[i].type == REDIR_INPUT)
// 	{
// 		if (tokens[i + 1].type)
// 		{
// 			if (tokens[i + 1].type == COMMAND
// 				|| tokens[i + 1].type == WORD
// 				|| tokens[i + 1].type == BUILT_IN)
// 			{
// 				if (access(tokens[i + 1].value, F_OK) != 0)
// 					return ((void)syntax_error(data, ERR_NO_INPUT_FILE, 1));
// 				if (access(tokens[i + 1].value, R_OK) != 0)
// 					return ((void)syntax_error(data, ERR_NO_INPUT_FILE, 1));
// 				return ;
// 			}
// 			else if (tokens[i + 1].value[0] == '*')
// 				return ((void)syntax_error(data, ERR_AMBIGUOUS_REDIR, 1));
// 		}
// 		syntax_error(data, ERR_SYNTAX, EXIT_USE);
// 	}
// }

int	check_redir_input(t_shell *data, t_prompt *p, t_token *tokens, int i)
{
	if (tokens[i].type == REDIR_INPUT)
	{
		if (!tokens[i + 1].type)
		{
			syntax_error(data, ERR_SYNTAX, EXIT_USE, "newline");
			return (SUCCESS);
		}
		if (!(tokens[i + 1].type == COMMAND
				|| tokens[i + 1].type == WORD
				|| tokens[i + 1].type == BUILT_IN
				|| tokens[i + 1].type == EXPANSION
				|| tokens[i + 1].type == WILDCAR))
			{
				syntax_error(data, ERR_SYNTAX, EXIT_USE, tokens[i + 1].value);
				return (SYNTAX_ERROR);
			}
	}
	return (SUCCESS);
}
