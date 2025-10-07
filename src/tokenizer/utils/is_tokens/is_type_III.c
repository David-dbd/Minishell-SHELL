/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_type_III.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 17:23:51 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/07 20:10:03 by davdiaz-         ###   ########.fr       */
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
 * Verifica si el carácter es un símbolo válido para identificadores:
 * acepta ?, !, $ y _ como parte de nombres de variables
 */

int	is_symbol(int c)
{
	if (c == '?' || c == '!' || c == '$' || c == '_')
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
