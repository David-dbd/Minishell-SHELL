/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_background.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 03:51:59 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/17 18:36:40 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

int	check_background(t_shell *data, t_prompt *p, t_token *tokens, int i)
{
	if (tokens[i].type == BACKGROUND)
	{
		if (i == 0)
		{
			syntax_error(data, ERR_SYNTAX, EXIT_USE, "&");
			return (SYNTAX_ERROR);
		}
		if (!(tokens[i - 1].type == COMMAND
				|| tokens[i - 1].type == WORD
				|| tokens[i - 1].type == BUILT_IN
				|| tokens[i + 1].type == EXPANSION
				|| tokens[i - 1].type == WILDCAR))
			{
				syntax_error(data, ERR_SYNTAX, EXIT_USE, tokens[i - 1].value);
				return (SYNTAX_ERROR);
			}
	}
	return (SUCCESS);
}
