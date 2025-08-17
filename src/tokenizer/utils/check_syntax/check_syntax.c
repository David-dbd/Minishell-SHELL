/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 19:06:35 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/17 18:09:24 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Comprueba que un paréntesis de apertura tenga un comando antes de cerrarse.
	Si no encuentra un cierre válido, lanza un error de sintaxis.
*/

int	check_open_parent(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	int	content_flag;

	content_flag = FALSE;
	if (tokens[i].type == PAREN_OPEN)
	{
		while (tokens[i].type)
		{
			if (tokens[i].type == COMMAND)
				content_flag = TRUE;
			if (tokens[i].type == PAREN_CLOSE && i > 1 && content_flag == TRUE)
			{
				prompt->n_parentesis++;
				return (0);
			}
			i++;
		}
		exit_error(data, ERR_SYNTAX, EXIT_USE);
	}
	return (0);
}

/*
	Comprueba que un paréntesis de cierre tenga un comando antes de la apertura.
	Si no encuentra un paréntesis abierto válido, lanza error de sintaxis.
*/

int	check_close_parent(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	int	content_flag;
	int	j;

	content_flag = FALSE;
	j = i;
	if (tokens[i].type == PAREN_CLOSE)
	{
		prompt->n_parentesis++;
		while (tokens[i].type && i >= 1)
		{
			if (tokens[i].type == COMMAND)
				content_flag = TRUE;
			if (tokens[i].type == PAREN_OPEN && i < j && content_flag == TRUE)
			{
				prompt->n_parentesis++;
				return (0);
			}
			i--;
		}
		exit_error(data, ERR_SYNTAX, EXIT_USE);
	}
	return (0);
}

/*
	Comprueba que el token '|' tenga un comando válido a izquierda y derecha.
	En caso contrario, lanza un error de sintaxis.
*/

int	check_pipe(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	if (tokens[i].type == PIPE)
	{
		if (i > 0 && tokens[i + 1].type && tokens[i - 1].type
			&& ((tokens[i + 1].type == COMMAND
					|| tokens[i + 1].type == WORD
					|| tokens[i + 1].type == BUILT_IN)
				&& (tokens[i - 1].type == COMMAND
					|| tokens[i - 1].type == WORD
					|| tokens[i - 1].type == BUILT_IN)))
		{
			prompt->n_pipes++;
			return (0);
		}
		exit_error(data, ERR_SYNTAX, EXIT_USE);
	}
	return (0);
}

/*
	Comprueba que los operadores '||' y '&&' tengan un comando válido a ambos
	lados. Si no se cumple, lanza un error de sintaxis.
*/

int	check_or_and(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	if (tokens[i].type == OR || tokens[i].type == AND)
	{
		if (i > 0 && tokens[i + 1].type && tokens[i - 1].type
			&& ((tokens[i + 1].type == COMMAND
					|| tokens[i + 1].type == WORD
					|| tokens[i + 1].type == BUILT_IN)
				&& (tokens[i - 1].type == COMMAND
					|| tokens[i - 1].type == WORD
					|| tokens[i - 1].type == BUILT_IN)))
		{
			return (0);
		}
		exit_error(data, ERR_SYNTAX, EXIT_USE);
	}
	return (0);
}

/*
	Verifica que paréntesis, comillas y caracteres de escape estén en número
	par y correctamente emparejados.
*/

int	valid_pair_operands(t_shell *data, t_prompt *prompt)
{
	if (prompt->n_parentesis > 0)
	{
		if (prompt->n_parentesis % 2 != 0)
			exit_error(data, ERR_SYNTAX, EXIT_USE);
	}
	if (prompt->n_double_quotes > 0)
	{
		if (prompt->n_single_quotes % 2 != 0)
			exit_error(data, ERR_SYNTAX, EXIT_USE);
	}
	if (prompt->n_escape > 0)
	{
		if (prompt->n_escape % 2 != 0)
			exit_error(data, ERR_SYNTAX, EXIT_USE);
	}
	return (0);
}
