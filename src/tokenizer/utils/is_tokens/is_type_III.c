/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_type_III.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 17:23:51 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/03 01:50:50 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
 * Determina si el tipo es una asignación de variable:
 * incluye asignaciones normales, con += y temporales
*/

int	is_asignation_type(int type)
{
	if (type == ASIGNATION || type == PLUS_ASIGNATION
		|| type == TEMP_ASIGNATION)
		return (1);
	return (0);
}

/*
 * Comprueba si el tipo puede ser argumento de comando:
 * palabras simples o asignaciones de variables
*/

int	is_arg_type(int type)
{
	if (type == ASIGNATION || type == WORD || type == PLUS_ASIGNATION
		|| type == EXPANSION)
		return (1);
	return (0);
}

/*
 * Comprueba si el tipo es assignaciones reales:
 * aplicadas en el proceso del padre.
*/

int	is_real_assignation_type(int type)
{
	if (type == ASIGNATION || type == PLUS_ASIGNATION)
		return (1);
	return (0);
}

/*
 * Comprueba si el tipo son asignaciones de output:
 * Para nodos true.
 */

int	is_redir_output_type(int type)
{
	if (type == REDIR_APPEND || type == REDIR_OUTPUT)
		return (1);
	return (0);
}

/*
 * Comprueba si el tipo son asignaciones de input:
 * Para nodos command.
 */

int	is_redir_input_type(int type)
{
	if (type == REDIR_HEREDOC || type == REDIR_INPUT)
		return (1);
	return (0);
}
