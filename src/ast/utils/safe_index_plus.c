/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_index_plus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 20:19:28 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/20 22:39:15 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
 *	Incrementa el índice del token de manera segura.
 *	Simplemente aumenta el valor del puntero `i` en una unidad,
 *	pero solo si el índice actual aún es menor que el número total
 *	de tokens disponibles (`n_tokens`), previniendo desbordamientos.
 */

void	safe_index_plus(int *i, int n_tokens)
{
	if (*i < n_tokens)
		(*i)++;
}
