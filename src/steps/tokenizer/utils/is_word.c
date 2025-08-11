/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_word.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 19:43:39 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/11 21:54:23 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

void	is_word(t_shell *data, t_token *tokens, const char *str, int *i)
{
	int		len;
	int		start;
	char	*word;

	start = *i;
	while (str[*i] && !ft_isspace(str[*i] && ft_isspecial(str[*i])))
		(*i)++;
	len = *i - start;
	if (len > 0)
	{
		word = ft_substr(str, start, len);
		if (!word)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		add_token(tokens, word, WORD);
		is_word_or_cmd(data, &data->prompt, tokens, word);
		free(word);
		(*i)--;
	}
}
