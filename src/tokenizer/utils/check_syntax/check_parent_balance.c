/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parent_balance.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 22:39:51 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/30 16:34:24 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Verifica que los paréntesis estén correctamente balanceados.
	Si hay más cierres que aperturas o viceversa, lanza error de sintaxis.
	Recorre todos los tokens y ajusta el balance según el tipo.
	Devuelve SUCCESS si el balance es cero al final.
*/

int	check_parent_balance(t_shell *data, t_prompt *prompt, t_token *tokens)
{
	int	i;
	int	balance;

	i = 0;
	balance = 0;
	while (i < prompt->n_tokens)
	{
		if (tokens[i].type == PAREN_OPEN)
			balance++;
		else if (tokens[i].type == PAREN_CLOSE)
			balance--;
		if (balance < 0)
		{
			syntax_error(data, ERR_SYNTAX, EXIT_USE, ")");// cierre sin apertura
			return (SYNTAX_ERROR);
		}
		i++;
	}
	if (balance > 0)
	{
		syntax_error(data, ERR_SYNTAX, EXIT_USE, "(");// apertura sin cierre
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}
