/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redir_output.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:39:51 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/31 12:57:20 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Valida un operador de redirección de salida '>' o '>>'.

	- Comprueba que haya un token después del operador.
	  Si no, devuelve error de sintaxis indicando "newline".
	- Verifica que el siguiente token sea algo válido para redirección:
	  COMMAND, WORD, BUILT_IN, EXPANSION, WILDCAR, SINGLE_QUOTE o DOUBLE_QUOTE.
	- Si no es válido, lanza error de sintaxis con el símbolo correspondiente.
	- Devuelve SUCCESS si todo es correcto.
*/

int	check_redir_output(t_shell *data, t_prompt *p, t_token *tokens, int i)
{
	if (tokens[i].type == REDIR_OUTPUT || tokens[i].type == REDIR_APPEND)
	{
		if (i > p->n_tokens || !tokens[i + 1].type)
		{
			syntax_error(data, ERR_SYNTAX, EXIT_USE, "newline");
			return (SYNTAX_ERROR);
		}
		if (!(tokens[i + 1].type == COMMAND
				|| tokens[i + 1].type == WORD
				|| tokens[i + 1].type == BUILT_IN
				|| tokens[i + 1].type == EXPANSION
				|| tokens[i + 1].type == WILDCARD
				|| tokens[i + 1].type == SINGLE_QUOTE
				|| tokens[i + 1].type == DOUBLE_QUOTE))
		{
			syntax_error(data, ERR_SYNTAX, EXIT_USE, tokens[i + 1].value);
			return (SYNTAX_ERROR);
		}
	}
	return (SUCCESS);
}
