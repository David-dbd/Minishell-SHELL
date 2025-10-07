/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_highlighter_I.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 19:18:18 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/07 19:18:20 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/* --- Strings dobles ("...") --- */
int	double_string_hl(ic_highlight_env_t *env, const char *s, size_t *i)
{
	size_t	start;

	if (s[*i] != '"')
		return (0);
	start = (*i)++;
	while (s[*i] && s[*i] != '"')
	{
		if (s[*i] == '\\' && s[*i + 1])
			(*i) += 2;
		else
			(*i)++;
	}
	if (s[*i])
		(*i)++;
	ic_highlight(env, start, *i - start, "string");
	return (1);
}

/* --- Strings simples ('...') --- */
int	single_string_hl(ic_highlight_env_t *env, const char *s, size_t *i)
{
	size_t	start;

	if (s[*i] != '\'')
		return (0);
	start = (*i)++;
	while (s[*i] && s[*i] != '\'')
	{
		if (s[*i] == '\\' && s[*i + 1])
			(*i) += 2;
		else
			(*i)++;
	}
	if (s[*i])
		(*i)++;
	ic_highlight(env, start, *i - start, "string-single");
	return (1);
}

/* --- Operadores --- */
int	and_hl(ic_highlight_env_t *henv, const char *s, size_t *i)
{
	if (s[*i] == '&' && s[*i + 1] == '&')
	{
		ic_highlight(henv, *i, 2, "op-and");
		*i += 2;
		return (1);
	}
	return (0);
}

int	or_hl(ic_highlight_env_t *henv, const char *s, size_t *i)
{
	if (s[*i] == '|' && s[*i + 1] == '|')
	{
		ic_highlight(henv, *i, 2, "op-or");
		*i += 2;
		return (1);
	}
	return (0);
}

int	pipe_hl(ic_highlight_env_t *henv, const char *s, size_t *i)
{
	if (s[*i] == '|')
	{
		ic_highlight(henv, *i, 1, "op-pipe");
		(*i)++;
		return (1);
	}
	return (0);
}
