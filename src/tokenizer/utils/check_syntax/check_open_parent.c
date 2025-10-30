/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_open_parent.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 22:40:41 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/29 18:10:45 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Comprueba que un paréntesis de apertura tenga un comando antes de cerrarse.
	Si no encuentra un cierre válido, lanza un error de sintaxis.
*/

static int	is_valid_before_open(t_token *tokens, int i)
{
	int	type;

	if (i == 0)
		return (FALSE);
	type = tokens[i - 1].type;
	if (type == PAREN_CLOSE || type == COMMAND || type == WORD
		|| type == BUILT_IN || type == EXPANSION)
		return (TRUE);
	return (FALSE);
}

int	check_open_parent(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	int	content_flag;

	content_flag = 0;
	if (tokens[i].type != PAREN_OPEN)
		return (SUCCESS);
	if (is_valid_before_open(tokens, i))
	{
		syntax_error(data, ERR_SYNTAX, EXIT_USE, "(");
		return (SYNTAX_ERROR);
	}
	while (i < prompt->n_tokens)
	{
		if (tokens[i].type != PAREN_OPEN && tokens[i].type != PAREN_CLOSE)
			content_flag++;
		if (tokens[i].type == PAREN_CLOSE && i > 1 && content_flag >= 1)
		{
			prompt->n_parentesis++;
			return (SUCCESS);
		}
		i++;
	}
	syntax_error(data, ERR_SYNTAX, EXIT_USE, "(");
	return (SYNTAX_ERROR);
}
