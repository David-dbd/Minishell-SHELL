/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_semicolon.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:48:30 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/31 21:33:48 by davdiaz-         ###   ########.fr       */
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
