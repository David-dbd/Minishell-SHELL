/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 22:40:33 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/13 01:10:02 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Valida un pipe '|' en la línea de comandos.

	- Comprueba que haya un token válido antes y después del pipe.
	  Tokens válidos antes: COMMAND, WORD, BUILT_IN, PAREN_CLOSE, DOUBLE_QUOTE,
	  SINGLE_QUOTE, EXPANSION.
	  Tokens válidos después: COMMAND, WORD, BUILT_IN, PAREN_OPEN, DOUBLE_QUOTE,
	  SINGLE_QUOTE, EXPANSION.
	- Si ambos lados son válidos, incrementa el contador y devuelve SUCCESS.
	- Si no, lanza error de sintaxis.
*/

int	check_pipe(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	if (tokens[i].type == PIPE)
	{
		if ((i > 0 && i + 1< prompt->n_tokens) && tokens[i + 1].type
			&& tokens[i - 1].type && ((tokens[i + 1].type == COMMAND
					|| tokens[i + 1].type == WORD
					|| tokens[i + 1].type == BUILT_IN
					|| tokens[i + 1].type == PAREN_OPEN
					|| tokens[i + 1].type == EXPANSION
					|| is_quote_type(tokens[i + 1].type)
					|| is_redir_type(tokens[i + 1].type)
					|| is_asignation_type(tokens[i + 1].type)))
				&& (tokens[i - 1].type == COMMAND
					|| tokens[i - 1].type == WORD
					|| tokens[i - 1].type == BUILT_IN
					|| tokens[i - 1].type == PAREN_CLOSE
					|| tokens[i - 1].type == EXPANSION
					|| tokens[i - 1].type == DELIMITER
					|| tokens[i - 1].type == FILENAME
					|| is_quote_type(tokens[i - 1].type)
					|| is_asignation_type(tokens[i - 1].type)))
		{
			prompt->n_pipes++;
			return (SUCCESS);
		}
		syntax_error(data, ERR_SYNTAX, EXIT_USE, tokens[i].value);
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}
