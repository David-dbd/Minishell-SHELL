/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd_externs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:35:07 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/09 23:20:47 by migarrid         ###   ########.fr       */
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
