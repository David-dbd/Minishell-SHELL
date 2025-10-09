/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_background.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 03:51:59 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/08 19:46:26 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Valida la posición de un token '&' (BACKGROUND) en la línea de comandos.

	- Si '&' está al inicio, devuelve error de sintaxis,
	- Si el token anterior no es un comando, palabra, built-in,
	  expansión o wildcard, devuelve error de sintaxis,
	  porque '&' solo puede seguir a algo ejecutable.
	- Devuelve SUCCESS si el '&' está en una posición válida.
*/



int	check_background(t_shell *data, t_prompt *p, t_token *tokens, int i)
{
	if (tokens[i].type == BACKGROUND)
	{
		if (i == 0)
		{
			syntax_error(data, ERR_SYNTAX, EXIT_USE, "&");
			return (SYNTAX_ERROR);
		}
		if (i > 0 && !(tokens[i - 1].type == COMMAND
				|| tokens[i - 1].type == WORD || tokens[i - 1].type == BUILT_IN
				|| tokens[i - 1].type == EXPANSION
				|| tokens[i - 1].type == WILDCAR))
		{
			syntax_error(data, ERR_SYNTAX, EXIT_USE, tokens[i - 1].value);
			return (SYNTAX_ERROR);
		}
		if (i < p->n_tokens && (tokens[i + 1].type == PIPE
				|| tokens[i + 1].type == AND || tokens[i + 1].type == OR
				|| tokens[i + 1].type == SEMICOLON
				|| tokens[i + 1].type == BACKGROUND))
		{
			syntax_error(data, ERR_SYNTAX, EXIT_USE, tokens[i + 1].value);
			return (SYNTAX_ERROR);
		}
	}
	return (SUCCESS);
}
