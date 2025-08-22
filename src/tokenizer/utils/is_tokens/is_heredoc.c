/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:58 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/22 03:08:02 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Detecta el operador de redirección heredoc '<<' y añade token.
	Avanza el índice para saltar el token.
*/

void	is_heredoc(t_shell *data, t_token *tokens, const char *str, int *i)
{
	char	*heredoc;

	if (ft_isdigit(str[*i]) && str[*i + 1] == '<' && str[*i + 2] == '<')
	{
		heredoc = ft_substr(str, *i, 3);
		if (!heredoc)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		add_token(tokens, heredoc, REDIR_HEREDOC);
		(*i) += 3;
	}
	else if (str[*i] == '<' && str[*i + 1] == '<')
	{
		heredoc = ft_strdup("<<");
		if (!heredoc)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		add_token(tokens, heredoc, REDIR_HEREDOC);
		(*i) += 2;
	}
}
