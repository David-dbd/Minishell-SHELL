/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_type_II.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 20:48:00 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/11 03:54:14 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Devuelve 1 si el tipo es WORD, usado para tokens dentro de comillas.
*/

int	is_between_quotes_type(int type)
{
	if (type == WORD || type == COMMAND || type == BUILT_IN)
		return (1);
	return (0);
}

/*
	Devuelve 1 si el tipo se puede simplificar al concatenar tokens:
	WORD, NO_SPACE o comillas simples/dobles.
*/

int	is_simplify_type(int type)
{
	if (type == WORD || type == NO_SPACE
		|| type == SINGLE_QUOTE || type == DOUBLE_QUOTE)
		return (1);
	return (0);
}

/*
 * Verifica si el tipo de token corresponde a un comando ejecutable:
 * retorna 1 para comandos normales y built-ins, 0 para otros tipos
 */

int	is_cmd_builtin_type(int type)
{
	if (type == COMMAND || type == BUILT_IN)
		return (1);
	return (0);
}

/*
 * Verifica si el carácter es un símbolo válido para identificadores:
 * acepta ?, !, $ y _ como parte de nombres de variables
 */

int	is_symbol(int c)
{
	if (c == '?' || c == '!' || c == '$' || c == '_')
		return (1);
	return (0);
}
