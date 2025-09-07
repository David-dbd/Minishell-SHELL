/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_not_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:41:47 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/07 21:23:15 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Avanza el índice si el carácter actual es un espacio.
	No genera ningún token.
*/

void	is_not_token(const char *str, int *i)
{
	char	c;

	c = str[*i];
	if (ft_isspace(c))
		(*i)++;
}
