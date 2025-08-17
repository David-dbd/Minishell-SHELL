/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_quote.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:47 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/17 16:16:40 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Detecta comillas simples o dobles y añade el token correspondiente.
	El último caso sería para sustitución de comandos (pendiente).
*/

void	is_quote(t_token *tokens, const char *str, int *i)
{
	char	c;

	c = str[*i];
	if (c == '\'')
	{
		add_token(tokens, "\'", SINGLE_QUOTE);
		(*i)++;
	}
	else if (c == '\"')
	{
		add_token(tokens, "\"", DOUBLE_QUOTE);
		(*i)++;
	}
	else if (c == '`')
	{
		add_token(tokens, "`", CMD_SUBSTITUTION);
		(*i)++;
	}
}
