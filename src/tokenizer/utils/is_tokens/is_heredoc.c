/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:58 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/17 16:16:05 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Detecta el operador de redirección heredoc '<<' y añade token.
	Avanza el índice para saltar el token.
*/

void	is_heredoc(t_token *tokens, const char *str, int *i)
{
	if (!ft_strncmp(str + *i, "<<", 2))
	{
		add_token(tokens, "<<", REDIR_HEREDOC);
		(*i)++;
		(*i)++;
	}
}
