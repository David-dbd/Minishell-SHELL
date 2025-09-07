/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redir_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:39:51 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/07 21:46:00 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Valida un operador de redirección de entrada '<' o heredoc '<<'.

	- Comprueba que haya un token después del operador.
	  Si no, devuelve error de sintaxis indicando "newline".
	- Verifica que el siguiente token sea algo que pueda recibir la redirección:
	  COMMAND, WORD, BUILT_IN, EXPANSION, WILDCAR, SINGLE_QUOTE o DOUBLE_QUOTE.
	- Si no es válido, lanza error de sintaxis con el símbolo correspondiente.
	- Devuelve SUCCESS si todo es correcto.
*/

int	check_redir_input(t_shell *data, t_prompt *p, t_token *tokens, int i)
{
	if (tokens[i].type == REDIR_INPUT || tokens[i].type == REDIR_HEREDOC)
	{
		if (!tokens[i + 1].type)
		{
			syntax_error(data, ERR_SYNTAX, EXIT_USE, "newline");
			return (SYNTAX_ERROR);
		}
		if (!(tokens[i + 1].type == COMMAND
				|| tokens[i + 1].type == WORD
				|| tokens[i + 1].type == BUILT_IN
				|| tokens[i + 1].type == EXPANSION
				|| tokens[i + 1].type == WILDCAR
				|| tokens[i + 1].type == SINGLE_QUOTE
				|| tokens[i + 1].type == DOUBLE_QUOTE))
		{
			if (tokens[i].type == REDIR_INPUT)
				syntax_error(data, ERR_SYNTAX, EXIT_USE, "<");
			else if (tokens[i].type == REDIR_HEREDOC)
				syntax_error(data, ERR_SYNTAX, EXIT_USE, "<<");
			return (SYNTAX_ERROR);
		}
	}
	return (SUCCESS);
}
