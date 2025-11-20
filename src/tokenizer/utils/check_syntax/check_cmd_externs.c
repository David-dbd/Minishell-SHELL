/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd_externs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:35:07 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/21 00:00:28 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

static int	search_for_previous_cmd(t_token *tokens, int i)
{
	while (i > 0 && (is_redir_type(tokens[i].type)
			|| tokens[i].type == FILENAME || tokens[i].type == WORD))
		i--;
	if (is_cmd_builtin_type(tokens[i].type))
		return (TRUE);
	return (FALSE);
}

int	after_filename_just_argument_unless_cmd_after_redir(t_token *tokens, int i)
{
	if (tokens[i].type != COMMAND)
		return (1);
	if (i >= 2 && tokens[i - 1].type == FILENAME && tokens[i].type != WORD
		&& search_for_previous_cmd(tokens, --i))
		return (0);
	return (1);
}

int	after_a_subshell_no_arg_or_cmd(t_token *tokens, int i)
{
	if (i >= 3 && tokens[i -1].type == PAREN_CLOSE)
	{
		if (is_redir_type(tokens[i].type)
			|| is_delimiter_type(tokens[i].type))
			return (1);
		return (0);
	}
	return (1);
}

int	check_cmd_externs(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	(void)prompt;
	if (!after_filename_just_argument_unless_cmd_after_redir(tokens, i))
	{
		syntax_error(data, ERR_SYNTAX, EXIT_USE, tokens[i].value);
		return (SYNTAX_ERROR);
	}
	if (!after_a_subshell_no_arg_or_cmd(tokens, i))
	{
		syntax_error(data, ERR_SYNTAX, EXIT_USE, tokens[i].value);
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}
