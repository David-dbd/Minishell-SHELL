/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_quote.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:47 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/11 19:45:13 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

void	is_quote(t_token *tokens, char c)
{
	if (c == '\'')
		add_token(tokens, "\'", SINGLE_QUOTE);
	else if (c == '\"')
		add_token(tokens, "\"", DOUBLE_QUOTE);
	else if (c == '`')
		add_token(tokens, "`", CMD_SUBSTITUTION);
}
