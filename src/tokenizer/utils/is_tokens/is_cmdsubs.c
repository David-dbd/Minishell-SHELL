/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_cmdsubs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 20:47:41 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/31 21:33:00 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Si encuentra '`', añade un token CMD_SUBSTITUTION y avanza el índice.
*/

void	is_cmdsubs(t_shell *data, t_prompt *prompt, const char *str, int *i)
{
	if (str[*i] == '`')
	{
		add_token(data, prompt, "`", CMD_SUBSTITUTION);
		(*i)++;
	}
}
