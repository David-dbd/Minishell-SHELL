/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_semicolon.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:48:30 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/13 22:42:13 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Detecta punto y coma ';' y añade un token de tipo SEMICOLON.
*/

void	is_semicolon(t_shell *data, t_prompt *prompt, const char *str, int *i)
{
	char	c;

	c = str[*i];
	if (c == ';' || c == '\n')
	{
		add_token(data, prompt, ";", SEMICOLON);
		(*i)++;
	}
}
