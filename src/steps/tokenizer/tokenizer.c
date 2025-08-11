/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:37:27 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/11 21:55:35 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	add_token(t_token *tokens, char *value, int type)
{
	static int	i = 0;

	tokens[i].value = value;
	tokens[i].type = type;
	if (tokens[i].type == EXPANSION)
		tokens[i].expand = TRUE;
	i++;
}

void	get_tokens(t_shell *data, t_token *tokens, char *input)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
	{
		is_dolar(data, tokens, input[i], &i);
		is_word(data, tokens, input[i], &i);
		is_heredoc(tokens, input[i], &i);
		is_parenten(tokens, input[i]);
		is_scape(tokens, input[i]);
		is_quote(tokens, input[i]);
		is_pipe(tokens, input[i]);
		is_semicolon(tokens, input[i]);
		is_wildcar(tokens, input[i]);
		is_and(tokens, input[i], &i);
		is_or(tokens, input[i], &i);
		is_redir(tokens, input[i], &i);
		is_finished(tokens, input[i]);
		i++;
	}
}

int	tokenizer(t_shell *data, t_prompt *prompt, char *input)
{
	allocate_tokens(data, prompt, input);
	get_tokens(data, prompt->tokens, input);
	valid_tokens(prompt->tokens);
}
