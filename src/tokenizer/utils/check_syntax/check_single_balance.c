/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_single_balance.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 22:39:40 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/13 01:04:21 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Comprueba que las comillas simples estén correctamente emparejadas.
	Usa un estado booleano: dentro o fuera de comillas.
	Cada comilla doble cambia el estado.
*/

int	check_single_balance(t_shell *data, t_prompt *prompt, t_token *tokens)
{
	int	i;
	int	inside_quotes;

	i = 0;
	inside_quotes = FALSE;
	while (i < prompt->n_tokens)
	{
		if (tokens[i].type == SINGLE_QUOTE) //Cadacomilla simple cambia el estado
		{
			if (inside_quotes == FALSE)
				inside_quotes = TRUE; // Entramos en comillas
			else
				inside_quotes = FALSE; // Salimos de comillas
		}
		i++;
	}
	if (inside_quotes == TRUE) // Si terminamos dentro de comillas, hay error
	{
		// printf("error en check_single_balance\n\n");
		syntax_error(data, ERR_SYNTAX, EXIT_USE, "\'");
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}
