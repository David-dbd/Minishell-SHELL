/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 22:40:33 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/08/27 00:13:46 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Comprueba que el token '|' tenga un comando válido a izquierda y derecha.
	En caso contrario, lanza un error de sintaxis.
*/

int	check_pipe(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	// heardoc: cmd | nothing if (tokens[i - 1].type && !tokens[i - 1].type)
	// command not found: cmd | cmd success; cmd | word fails;
	//word | cmd fails and success;  word | word fails;
	// syntax error near unexpected token `|': nothing | nothing fails;
	// nothing | cmd fails;
	if (tokens[i].type == PIPE)
	{
		if ((i > 0 && i < prompt->n_tokens)
			&& tokens[i + 1].type && tokens[i - 1].type
			&& ((tokens[i + 1].type == COMMAND
					|| tokens[i + 1].type == WORD
					|| tokens[i + 1].type == BUILT_IN
					|| tokens[i + 1].type == PAREN_OPEN
					|| tokens[i + 1].type == DOUBLE_QUOTE
					|| tokens[i + 1].type == SINGLE_QUOTE
					|| tokens[i + 1].type == EXPANSION)
				&& (tokens[i - 1].type == COMMAND
					|| tokens[i - 1].type == WORD
					|| tokens[i - 1].type == BUILT_IN
					|| tokens[i - 1].type == PAREN_CLOSE
					|| tokens[i - 1].type == DOUBLE_QUOTE
					|| tokens[i - 1].type == SINGLE_QUOTE
					|| tokens[i - 1].type == EXPANSION)))
		{
			prompt->n_pipes++;
			return (SUCCESS);
		}
		syntax_error(data, ERR_SYNTAX, EXIT_USE, tokens[i].value);
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}
