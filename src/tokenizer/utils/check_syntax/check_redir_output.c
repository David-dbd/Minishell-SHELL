/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redir_output.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:39:51 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/17 16:15:19 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

// void	check_redir_output(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
// {
// 	if (tokens[i].type == REDIR_OUTPUT || tokens[i].type == REDIR_APPEND)
// 	{
// 		if (tokens[i + 1].type)
// 		{
// 			if ((tokens[i + 1].type == SINGLE_QUOTE
// 					&& tokens[i + 2].type == SINGLE_QUOTE)
// 				|| (tokens[i + 1].type == DOUBLE_QUOTE
// 					&& tokens[i + 2].type == DOUBLE_QUOTE))
// 			{
// 				return ((void)syntax_error(data, ERR_SYNTAX, EXIT_MISUSE));
// 			}
// 			// // if (tokens[i + 1].type == COMMAND
// 			// // 	|| tokens[i + 1].type == WORD
// 			// // 	|| tokens[i + 1].type == BUILT_IN)
// 			// // {
// 			// if (access(tokens[i + 1].value, F_OK) == 0)
// 			// {
// 			// 	if (is_directory(tokens[i + 1].value))
// 			// 		return ((void)syntax_error(data, ERR_IS_DIR, EXIT_FAILURE));
// 			// 	if (access(tokens[i + 1].value, W_OK) != 0)
// 			// 		return ((void)syntax_error(data, ERR_PERM_DENIED, EXIT_FAILURE));
// 			// }
// 			else if (tokens[i + 1].value[0] == '*')
// 				return ((void)syntax_error(data, ERR_AMBIGUOUS_REDIR, EXIT_FAILURE));
// 			return ;
// 		}
// 		syntax_error(data, ERR_SYNTAX, EXIT_MISUSE);
// 	}
// }

void	check_redir_output(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	if (tokens[i].type == REDIR_OUTPUT || tokens[i].type == REDIR_APPEND)
	{
		if (!tokens[i + 1].type)
			return ((void)syntax_error(data, ERR_SYNTAX, EXIT_MISUSE));
		if (!(tokens[i + 1].type == COMMAND
				|| tokens[i + 1].type == WORD
				|| tokens[i + 1].type == BUILT_IN
				|| tokens[i + 1].type == WILDCAR))
			return ((void)syntax_error(data, ERR_SYNTAX, EXIT_MISUSE));
	}
}
