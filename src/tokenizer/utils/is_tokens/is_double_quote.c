/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_double_quote.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:47 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/27 01:03:36 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Detecta comillas simples o dobles y añade el token correspondiente.
	El último caso sería para sustitución de comandos (pendiente).
*/

static char	*cleanner_word(t_shell *data, char *word, int len, char quote)
{
	int		j;
	int		k;
	char	*clean_word;

	j = 0;
	k = 0;
	if (ft_strchr(word, quote))
	{
		clean_word = ft_calloc(len + 1, sizeof(char *));
		if (!clean_word)
		{
			free(word);
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		}
		while (word[j])
		{
			if (word[j] != quote)
				clean_word[k++] = word[j];
			j++;
		}
		clean_word[k] = '\0';
		free(word);
		return (clean_word);
	}
	return (word);
}

void	make_word_d(t_shell *data, t_token *tokens, const char *s, int range[2])
{
	char	*word;
	int		token_id;

	word = ft_substr(s, range[0], range[1] - range[0]);
	if (!word)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	word = cleanner_word(data, word, range[1] - range[0], '\"');
	word = cleanner_slash(data, word, range[1] - range[0], '\\');
	if (ft_strcmp(word, ""))
	{
		if (ft_strchr (word, '$'))
			token_id = add_token(tokens, word, EXPANSION);
		else
			token_id = add_token(tokens, word, WORD);
		is_cmd(data, &data->prompt, &tokens[token_id], word);
	}
}

int	ft_is_dead_d(const char *s, int *i, char quote, int *flag)
{
	if ((s[*i] == quote && s[*i + 1] != quote))
	{
		if (ft_isalpha(s[*i + 1]) || s[*i + 1] == '\\')
			*flag = TRUE;
		return (1);
	}
	if (s[*i] == '\\' && s[*i + 1] != '\0')
	{
		(*i)++;
		(*i)++;
	}
	else if (s[*i] == quote && s[*i + 1] == quote)
	{
		if (ft_strchr(s + *i +2, quote))
			(*i)++;
	}
	return (0);
}

int	is_special_word_d(t_shell *data, t_token *tokens, const char *str, int *i)
{
	int		flag;
	int		len;
	int		start_end[2];

	flag = FALSE;
	if (*i + 1 < ft_strlen(str) && ft_strchr(str + *i + 1, '\"'))
	{
		start_end[0] = *i;
		while (str[*i] != '\0' && !ft_is_dead_d(str, i, '\"', &flag))
			(*i)++;
		start_end[1] = *i;
		len = start_end[1] - start_end[0];
		if (len >= 1)
			make_word_d(data, tokens, str, start_end);
		if (flag)
			return (NO_SPACE);
		return (TRUE);
	}
	return (FALSE);
}

void	is_double_quote(t_shell *data, t_token *tokens, const char *str, int *i)
{
	int	status;

	if (str[*i] == '\"')
	{
		add_token(tokens, "\"", DOUBLE_QUOTE);
		(*i)++;
		status = is_special_word_d(data, tokens, str, i);
		if (status == TRUE || status == NO_SPACE)
		{
			add_token(tokens, "\"", DOUBLE_QUOTE);
			(*i)++;
			if (status == NO_SPACE)
				add_token(tokens, "", NO_SPACE);
		}
	}
}
