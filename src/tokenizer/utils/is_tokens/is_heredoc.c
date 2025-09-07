/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:58 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/07 21:22:39 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Detecta un operador de heredoc '<<' en el input.
	- Si hay un número antes (ej. '3<<'), considera el descriptor, toma 3 chars.
	- Si no hay número, detecta solo '<<'.
	- Crea token REDIR_HEREDOC y avanza el índice según la longitud del token.
*/

void	is_heredoc(t_shell *data, t_prompt *prompt, const char *str, int *i)
{
	char	*heredoc;

	if (ft_isdigit(str[*i]) && str[*i + 1] == '<' && str[*i + 2] == '<')
	{
		heredoc = ft_substr(str, *i, 3);
		if (!heredoc)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		add_token(data, prompt, heredoc, REDIR_HEREDOC);
		(*i) += 3;
	}
	else if (str[*i] == '<' && str[*i + 1] == '<')
	{
		heredoc = ft_strdup("<<");
		if (!heredoc)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		add_token(data, prompt, heredoc, REDIR_HEREDOC);
		(*i) += 2;
	}
}
