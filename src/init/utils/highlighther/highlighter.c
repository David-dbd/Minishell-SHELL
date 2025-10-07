/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   highlighter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:30:16 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/07 19:14:56 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

static int	comment_hl(ic_highlight_env_t *henv, const char *input, size_t i)
{
	if (input[i] == '#')
	{
		ic_highlight(henv, i, ft_strlen(input) - i, "comment");
		return (1);
	}
	return (0);
}

int	highlight_manager(ic_highlight_env_t *henv, const char *s, size_t *i)
{
	if (double_string_hl(henv, s, i))
		return (1);
	if (single_string_hl(henv, s, i))
		return (1);
	if (and_hl(henv, s, i) || or_hl(henv, s, i))
		return (1);
	if (pipe_hl(henv, s, i) || background_hl(henv, s, i))
		return (1);
	if (redir_hl(henv, s, i))
		return (1);
	if (separator_hl(henv, s, i) || paren_hl(henv, s, i))
		return (1);
	if (number_hl(henv, s, i))
		return (1);
	return (0);
}

void	highlighter(ic_highlight_env_t *henv, const char *input, void *args)
{
	size_t	i;

	i = 0;
	(void)args;
	while (input[i])
	{
		if (comment_hl(henv, input, i))
			break ;
		if (highlight_manager(henv, input, &i))
			continue ;
		i++;
	}
}
