/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_it_quoted.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 21:23:00 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/20 22:53:39 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

static void	handle_quotes(t_token *token, int *in_single_q, int *in_double_q)
{
	if (token->type == SINGLE_QUOTE)
		*in_single_q = !(*in_single_q);
	else if (token->type == DOUBLE_QUOTE)
		*in_double_q = !(*in_double_q);
}

static void	mark_token_quotes(t_token *token, int in_double_q, int in_single_q)
{
	if (in_double_q)
		token->double_quoted = TRUE;
	else
		token->double_quoted = FALSE;
	if (in_single_q)
		token->single_quoted = TRUE;
	else
		token->single_quoted = FALSE;
}

void	is_it_quoted(t_prompt *prompt, t_token *tokens)
{
	int	i;
	int	in_double_quotes;
	int	in_single_quotes;

	i = 0;
	in_double_quotes = FALSE;
	in_single_quotes = FALSE;
	while (i < prompt->n_tokens)
	{
		handle_quotes(&tokens[i], &in_double_quotes, &in_single_quotes);
		if (!(tokens[i].type == DOUBLE_QUOTE || tokens[i].type == SINGLE_QUOTE))
			mark_token_quotes(&tokens[i], in_double_quotes, in_single_quotes);
		i++;
	}
}
