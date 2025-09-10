/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_semicolon.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:16:45 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/10 21:16:48 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Valida la posición de un ';' en la línea de comandos.

	- Si ';' está al inicio, devuelve error de sintaxis.
	- Si el token anterior no es ejecutable o palabra válida
	  (COMMAND, WORD, BUILT_IN, EXPANSION, WILDCAR), también lanza error.
	- Devuelve SUCCESS si ';' está en una posición válida.
*/

int	check_semicolon(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	if (tokens[i].type == SEMICOLON)
	{
		if (i == 0)
		{
			syntax_error(data, ERR_SYNTAX, EXIT_USE, ";");
			return (SYNTAX_ERROR);
		}
		if (!(tokens[i - 1].type == COMMAND
				|| tokens[i - 1].type == WORD
				|| tokens[i - 1].type == BUILT_IN
				|| tokens[i + 1].type == EXPANSION
				|| tokens[i - 1].type == WILDCAR
				|| tokens[i - 1].type == DOUBLE_QUOTE))
		{
			syntax_error(data, ERR_SYNTAX, EXIT_USE, tokens[i - 1].value);
			return (SYNTAX_ERROR);
		}
	}
	return (SUCCESS);
}
