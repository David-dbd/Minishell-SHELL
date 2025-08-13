/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_and.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:44:05 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/13 15:18:37 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Detecta el operador lógico '&&' y añade un token de tipo AND.
	Avanza el índice para no volver a procesar el token.
*/

void	is_and(t_token *tokens, const char *str, int *i)
{
	if (str[*i] == '&')
	{
		if (str[*i + 1] == '&')
		{
			add_token(tokens, "&&", AND);
			(*i)++;
			(*i)++;
		}
		else
		{
			add_token(tokens, ft_strdup("&"), WORD);
			(*i)++;
		}
	}
}
