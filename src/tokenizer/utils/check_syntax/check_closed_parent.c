/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_closed_parent.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 22:40:47 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/08/26 18:50:56 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Comprueba que un paréntesis de cierre tenga un comando antes de la apertura.
	Si no encuentra un paréntesis abierto válido, lanza error de sintaxis.
*/

/*static void	is_only_symbol(int i, t_token *tokens, int content_flag, int *error)
{
	if (i >= 1 && tokens[i - 1].type
		&& (tokens[i - 1].type == PIPE
			|| tokens[i - 1].type == OR
			|| tokens[i - 1].type == AND)
		&& content_flag == 0)
	{
		*error = TRUE;
	}
}

int	check_close_parent(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	int	content_flag;
	int	error_flag;
	int	j;

	content_flag = 0;
	error_flag = FALSE;
	j = i;
	if (tokens[i].type == PAREN_CLOSE)
	{
		prompt->n_parentesis++;
		is_only_symbol(i, tokens, content_flag, &error_flag);
		while (i >= 0)
		{
			if (tokens[i].type != PAREN_OPEN && tokens[i].type != PAREN_CLOSE)
				content_flag++;
			if (tokens[i].type == PAREN_OPEN && i < j && content_flag == 1 && error_flag == TRUE) //Si es un comando que tiene "|" justo antes de ")" y nada mas: (|)
				return (SUCCESS);
			else if (tokens[i].type == PAREN_OPEN && i < j && content_flag >= 1 && error_flag == FALSE) //Si es un comando que no tiene un "|" justo antes del ")": (ls)
				return (SUCCESS);
			else if (tokens[i].type == PAREN_OPEN && i < j && content_flag > 1 && error_flag == TRUE) //Si lo que esta antes de ")" es un "|" y hay mas contenido: (ls |)
			{
				syntax_error(data, ERR_SYNTAX, EXIT_USE, ")");
				return (SYNTAX_ERROR);
			}
			i--;
		}
		syntax_error(data, ERR_SYNTAX, EXIT_USE, ")");
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}*/

int	check_close_parent(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	int	content_flag;
	int	j;

	content_flag = 0;
	j = i;
	if (tokens[i].type != PAREN_CLOSE)
		return (SUCCESS);
	prompt->n_parentesis++;
	while (i >= 0 && tokens[i].type != PAREN_OPEN)
	{
		if (tokens[i].type != PAREN_CLOSE)
			content_flag++;
		if (tokens[i].type == PIPE || tokens[i].type == OR || tokens[i].type == AND)
			if (content_flag == 1)
				return (syntax_error(data, ERR_SYNTAX, EXIT_USE, ")"), SYNTAX_ERROR);
		i--;
	}
	if (i < 0 || content_flag == 0)
		return (syntax_error(data, ERR_SYNTAX, EXIT_USE, ")"), SYNTAX_ERROR);
	return (SUCCESS);
}
