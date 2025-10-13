/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_type_I.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 20:48:00 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/12 18:20:42 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Devuelve 1 si el tipo es un comando, built-in o palabra.
*/

int	is_primitive_cmd_type(int type)
{
	if (type == COMMAND || type == BUILT_IN || type == WORD)
		return (1);
	return (0);
}

/*
	Devuelve 1 si el tipo es comilla simple o doble.
*/

int	is_quote_type(int type)
{
	if (type == DOUBLE_QUOTE || type == SINGLE_QUOTE)
		return (1);
	return (0);
}

/*
	Devuelve 1 si el tipo es redirección.
*/

int	is_redir_type(int type)
{
	if (type == REDIR_APPEND || type == REDIR_OUTPUT || type == REDIR_HEREDOC
		|| type == REDIR_INPUT)
		return (1);
	return (0);
}

/*
	Devuelve 1 si el tipo es un delimitador de comandos (pipe, ;, &&, ||).
*/

int	is_delimiter_type(int type)
{
	if (type == PIPE || type == SEMICOLON || type == AND || type == OR
		|| type == PAREN_OPEN || type == PAREN_CLOSE || type == BACKGROUND)
		return (1);
	return (0);
}

/*
	Devuelve 1 si el tipo ocupa memoria y requiere liberación al limpiar tokens.
*/

int	is_alloc_type(int type)
{
	if (type == WORD || type == BUILT_IN || type == COMMAND || type == WILDCAR
		|| type == REDIR_APPEND || type == REDIR_HEREDOC || type == REDIR_INPUT
		|| type == REDIR_OUTPUT || type == EXPANSION || type == FILENAME
		|| type == ASIGNATION || type == TEMP_ASIGNATION
		|| type == PLUS_ASIGNATION || type == DELIMITER
		|| type == FILENAME)
		return (1);
	return (0);
}
