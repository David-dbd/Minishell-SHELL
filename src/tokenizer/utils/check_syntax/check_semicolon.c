/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_semicolon.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 03:53:16 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/17 16:15:23 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

void	check_semicolon(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	if (tokens[i].type == SEMICOLON)
	{
		if (i == 0)
			return ((void)syntax_error(data, ERR_SYNTAX, EXIT_MISUSE));
		if (!(tokens[i - 1].type == COMMAND
				|| tokens[i - 1].type == WORD
				|| tokens[i - 1].type == BUILT_IN
				|| tokens[i - 1].type == WILDCAR))
			return ((void)syntax_error(data, ERR_SYNTAX, EXIT_MISUSE));
	}
}
