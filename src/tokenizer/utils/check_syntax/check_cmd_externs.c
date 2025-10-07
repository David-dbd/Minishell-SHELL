/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd_externs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:35:07 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/07 17:42:43 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

int	check_cmd_externs(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	if (tokens[i].type != COMMAND)
		return (SUCCESS);
	if (i > 0 && tokens[i - 1].type
		&& tokens[i - 1].type == FILENAME)
		{
			syntax_error(data, ERR_SYNTAX, EXIT_USE, tokens[i].value);
			return (SYNTAX_ERROR);
		}
	return (SUCCESS);
}
