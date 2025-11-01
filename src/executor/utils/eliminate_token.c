/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eliminate_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 00:04:17 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/31 22:42:45 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static void	free_single_token(t_token *tokens, int i)
{
	if (is_alloc_type(tokens[i].type))
	{
		if (tokens[i].value)
			free(tokens[i].value);
	}
}

static void	reorganize(t_prompt *prompt, t_token *tokens, int i)
{
	int	tokens_to_move;

	free_single_token(tokens, i);
	tokens[i] = (t_token){0};
	tokens_to_move = prompt->n_tokens - i - 1;
	ft_memmove(&tokens[i], &tokens[i + 1],
		tokens_to_move * sizeof(t_token));
	prompt->n_tokens--;
	void_tokens_at_the_end(tokens, prompt->n_alloc_tokens, prompt->n_tokens);
}

void	eliminate_token(t_prompt *prompt, t_token *tokens, int index)
{
	int	j;

	j = index;
	reorganize(prompt, tokens, index);
	while (j < prompt->n_tokens)
	{
		tokens[j].id -= 1;
		j++;
	}
}
