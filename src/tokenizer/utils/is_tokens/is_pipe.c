/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:50 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/31 21:33:33 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Detecta el operador pipe '|' y añade un token de tipo PIPE.
*/

void	is_pipe(t_shell *data, t_prompt *prompt, const char *str, int *i)
{
	char	c;

	c = str[*i];
	if (c == '|')
	{
		add_token(data, prompt, "|", PIPE);
		(*i)++;
	}
}
