/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_highlighter_II.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:41:06 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/07 19:15:08 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

int	background_hl(ic_highlight_env_t *henv, const char *s, size_t *i)
{
	if (s[*i] == '&')
	{
		ic_highlight(henv, *i, 1, "op-background");
		(*i)++;
		return (1);
	}
	return (0);
}

int	redir_hl(ic_highlight_env_t *henv, const char *s, size_t *i)
{
	if (s[*i] == '>' && s[*i + 1] == '>')
		return (ic_highlight(henv, *i, 2, "op-redirect"), *i += 2, 1);
	if (s[*i] == '>')
		return (ic_highlight(henv, *i, 1, "op-redirect"), (*i)++, 1);
	if (s[*i] == '<' && s[*i + 1] == '<')
		return (ic_highlight(henv, *i, 2, "op-redirect"), *i += 2, 1);
	if (s[*i] == '<')
		return (ic_highlight(henv, *i, 1, "op-redirect"), (*i)++, 1);
	return (0);
}

int	separator_hl(ic_highlight_env_t *henv, const char *s, size_t *i)
{
	if (s[*i] == ';')
	{
		ic_highlight(henv, *i, 1, "op-separator");
		(*i)++;
		return (1);
	}
	return (0);
}

int	paren_hl(ic_highlight_env_t *henv, const char *s, size_t *i)
{
	if (s[*i] == '(' || s[*i] == ')')
	{
		ic_highlight(henv, *i, 1, "op-paren");
		(*i)++;
		return (1);
	}
	return (0);
}

/* --- Números --- */
int	number_hl(ic_highlight_env_t *henv, const char *s, size_t *i)
{
	size_t	start;

	if (!ft_isdigit(s[*i]))
		return (0);
	start = *i;
	while (s[*i] && (ft_isdigit(s[*i]) || s[*i] == '.'))
		(*i)++;
	ic_highlight(henv, start, *i - start, "number");
	return (1);
}
