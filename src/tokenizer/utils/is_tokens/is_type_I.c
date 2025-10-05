/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_type_I.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 20:48:00 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/05 15:04:47 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Devuelve 1 si el tipo es un comando, built-in o palabra.
*/

int	is_cmd_type(int type)
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
	Devuelve 1 si el tipo es redirección de salida o append.
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
	if (type == PIPE || type == SEMICOLON || type == AND || type == OR)
		return (1);
	return (0);
}

/*
	Devuelve 1 si el tipo ocupa memoria y requiere liberación al limpiar tokens.
*/

int	is_alloc_type(int type)
{
	if (type == WORD || type == BUILT_IN || type == COMMAND || type == WILDCAR
		|| type == REDIR_APPEND || type == REDIR_HEREDOC
		|| type == REDIR_INPUT || type == REDIR_OUTPUT
		|| type == EXPANSION || type == ASIGNATION || type == TEMP_ASIGNATION)
		return (1);
	return (0);
}

int	is_arg_type(int type)
{
	if (type == ASIGNATION || type == WORD)
		return (1);
	return (0);
}

int	is_asignation_type(int type)
{
	if (type == ASIGNATION || type == PLUS_ASIGNATION
		|| type == TEMP_ASIGNATION)
		return (1);
	return (0);
}

int	is_symbol(int c)
{
	if (c == '?' || c == '!' || c == '$' || c == '_')
		return (1);
	return (0);
}
