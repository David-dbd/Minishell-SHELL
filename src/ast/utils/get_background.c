/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_background.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 20:47:36 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/20 22:33:41 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
 *	Verifica si el comando actual debe ejecutarse en segundo plano.
 *	Revisa si el token siguiente es el operador de **background** (`&`).
 *	Si lo encuentra, avanza el índice y retorna TRUE. Esta funcion es
 *	llamada al final del parseo de un comando, subshell o redireccion.
 */

int	get_background(t_token *tokens, int n_tokens, int *i)
{
	if (*i < n_tokens && tokens[*i].type == BACKGROUND)
	{
		(*i)++;
		return (TRUE);
	}
	return (FALSE);
}
