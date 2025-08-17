/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_or.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:54 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/17 16:16:33 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Detecta el operador lógico '||' y añade un token de tipo OR.
	Avanza el índice para no volver a procesar el token.
*/

void	is_or(t_token *tokens, const char *str, int *i)
{
	if (!ft_strncmp(str + *i, "||", 2))
	{
		add_token(tokens, "||", OR);
		(*i)++;
		(*i)++;
	}
}
