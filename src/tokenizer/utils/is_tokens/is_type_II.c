/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_type_II.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 20:48:00 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/11 17:50:17 by migarrid         ###   ########.fr       */
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

int	is_cmd_builtin_type(int type)
{
	if (type == COMMAND || type == BUILT_IN)
		return (1);
	return (0);
}
