/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd_externs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:35:07 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/27 14:44:03 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

static int	search_for_cmd(t_token *tokens, int i)
{
	while (i > 0 && (is_redir_type(tokens[i].type)
		|| tokens[i].type == FILENAME || tokens[i].type == WORD))
		i--;
	if (is_cmd_builtin_type(tokens[i].type))
		return (TRUE);
	return (FALSE);
}

int	check_cmd_externs(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	if (tokens[i].type != COMMAND)
		return (SUCCESS);
	if (i >= 2 && tokens[i - 1].type
		&& tokens[i - 1].type == FILENAME && tokens[i].type != WORD
		&& search_for_cmd(tokens, --i))
	{
		syntax_error(data, ERR_SYNTAX, EXIT_USE, tokens[i].value);
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}
