/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_hash.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:48:47 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/13 15:02:17 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Detecta el carácter '#' que inicia un comentario.
	Ignora todo el resto de la línea desde el '#' hasta el final.
	Avanza el índice hasta el final del string para terminar el parsing.
*/

void	is_hash(const char *str, int *i)
{
	if (str[*i] == '#')
	{
		while (str[*i] != '\0')
			(*i)++;
	}
}
