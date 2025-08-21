/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_quote.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:47 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/21 23:49:45 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Detecta comillas simples o dobles y añade el token correspondiente.
	El último caso sería para sustitución de comandos (pendiente).
*/

char	*cleanner_word(t_shell *data, char *word, int len, char quote)
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

int	ft_is_dead_space(const char *str, int *i, char quote)
{
	if ((str[*i] == quote && str[*i + 1] != quote))
		return (1);
	else if (str[*i] == quote && str[*i + 1] == quote)
	{
		(*i)++;
		(*i)++;
	}
	return (0);
}

int	is_special_word(t_shell *data, t_token *tokens, const char *str, int *i)
{
	int		len;
	int		start;
	char	*word;
	char	quote;
	int		token_id;

	quote = str[*i - 1];
	if (*i + 1 < ft_strlen(str) && ft_strchr(str + *i + 1, quote))
	{
		start = *i;
		while (str[*i] != '\0' && !ft_is_dead_space(str, i, quote))
			(*i)++;
		len = *i - start;
		if (len > 0)
		{
			word = ft_substr(str, start, len);
			if (!word)
				exit_error(data, ERR_MALLOC, EXIT_FAILURE);
			word = cleanner_word(data, word, len, quote);
			token_id = add_token(tokens, word, WORD);
			is_cmd(data, &data->prompt, &tokens[token_id], word);
		}
		return (TRUE);
	}
	return (FALSE);
}

void	is_quote(t_shell *data, t_token *tokens, const char *str, int *i)
{
	if (str[*i] == '\'')
	{
		add_token(tokens, "\'", SINGLE_QUOTE);
		(*i)++;
		if (is_special_word(data, tokens, str, i))
		{
			add_token(tokens, "\'", SINGLE_QUOTE);
			(*i)++;
		}
	}
	else if (str[*i] == '\"')
	{
		add_token(tokens, "\"", DOUBLE_QUOTE);
		(*i)++;
		if (is_special_word(data, tokens, str, i))
		{
			add_token(tokens, "\"", DOUBLE_QUOTE);
			(*i)++;
		}
	}
	else if (str[*i] == '`')
	{
		add_token(tokens, "`", CMD_SUBSTITUTION);
		(*i)++;
	}
}
