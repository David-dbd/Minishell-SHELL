/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_redir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:45 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/11 21:08:09 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

void	is_redir(t_token *tokens, const char *str, int *i)
{
	if (!ft_strncmp(str, ">>", 2))
	{
		add_token(tokens, ">>", REDIR_APPEND);
		(*i)++;
	}
	else if (str[0] == '>')
		add_token(tokens, ">", REDIR_OUTPUT);
	else if (str[0] == '<')
		add_token(tokens, "<", REDIR_INPUT);
}
