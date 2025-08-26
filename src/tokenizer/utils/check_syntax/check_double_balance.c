/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_double_balance.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 22:40:25 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/08/26 18:00:45 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

// /*
// 	Verifica si el balance de comillas está equilibrado.
// 	Si hay más cierres que aperturas o viceversa, lanza error de sintaxis.
// */

// static int	check_balance(t_shell *data, int balance, char *quote_type, int)
// {
// 	if (balance < 0) // cierre sin apertura
// 	{
// 		syntax_error(data, ERR_SYNTAX, EXIT_USE, quote_type);
// 		return (SYNTAX_ERROR);
// 	}
// 	else if (balance > 0) // apertura sin cierre
// 	{
// 		syntax_error(data, ERR_SYNTAX, EXIT_USE, quote_type);
// 		return (SYNTAX_ERROR);
// 	}
// 	return (SUCCESS);
// }

// /*
// 	Comprueba que las comillas dobles estén correctamente emparejadas.
// 	Luego llama a check_single_balance para validar también las comillas simples.
// */
// static void	first_coincidence(t_token *tokens, int *flag, int *balance, int i)
// {
// 	if (tokens[i].type == DOUBLE_QUOTE && flag == FALSE)
// 	{
// 		*flag = TRUE;
// 		(*balance)++;
// 	}
// }

// int	check_double_balance(t_shell *data, t_prompt *promp, t_token *tokens, int i)
// {
// 	int	balance;
// 	int	flag_check;

// 	balance = 0;
// 	flag_check = FALSE;
// 	if (tokens[i].type == DOUBLE_QUOTE)
// 	{
// 		while (i < promp->n_tokens)
// 		{
// 			first_coincidence(tokens, &flag_check, &balance, i);
// 			if (tokens[i].type == DOUBLE_QUOTE && i > 0)
// 			{
// 				flag_check = FALSE;
// 				balance--;
// 			}
// 			if (!check_the_balance(data, balance, "\""))
// 				return (SYNTAX_ERROR);
// 			i++;
// 		}
// 		if (!check_the_balance(data, balance, "\""))
// 			return (SYNTAX_ERROR);
// 	}
// 	return (SUCCESS);
// }

/*
	Comprueba que las comillas dobles estén correctamente emparejadas.
	Usa un estado booleano: dentro o fuera de comillas.
	Cada comilla doble cambia el estado.
*/
int	check_double_balance(t_shell *data, t_prompt *promp, t_token *tokens, int start_i)
{
	int	inside_quotes;
	int	i;

	inside_quotes = FALSE;
	i = 0;
	while (i < promp->n_tokens)
	{
		if (tokens[i].type == DOUBLE_QUOTE) // Cada comilla doble cambia el estado
		{
			if (inside_quotes == FALSE)
				inside_quotes = TRUE;  // Entramos en comillas
			else
				inside_quotes = FALSE; // Salimos de comillas
		}
		i++;
	}
	if (inside_quotes == TRUE)  // Si terminamos dentro de comillas, hay error
	{
		syntax_error(data, ERR_SYNTAX, EXIT_USE, "\"");
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}
