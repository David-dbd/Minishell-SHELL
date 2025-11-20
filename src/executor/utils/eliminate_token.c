/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eliminate_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 00:04:17 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/20 23:38:57 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static void	free_single_token(t_token *tokens, int i)
{
	if (is_alloc_type(tokens[i].type))
	{
		if (tokens[i].value)
			ft_free((void **)&tokens[i].value);
	}
	tokens[i] = (t_token){0};
}

static void	reorganize(t_prompt *prompt, t_token *tokens, int i)
{
	int	tokens_to_move;

	free_single_token(tokens, i);
	tokens_to_move = prompt->n_tokens - i - 1;
	if (tokens_to_move > 0)
	{
		ft_memmove(&tokens[i], &tokens[i + 1],
			tokens_to_move * sizeof(t_token));
	}
	prompt->n_tokens--;
	void_tokens_at_the_end(tokens, prompt->n_alloc_tokens, prompt->n_tokens);
}

void	convert_to_true_token(t_shell *data, t_token *tokens, int i)
{
	if (is_alloc_type(tokens[i].type))
	{
		if (tokens[i].value)
			ft_free((void **)&tokens[i].value);
	}
	tokens[i].value = ft_strdup("true");
	if (!tokens[i].value)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
}

int	is_expansion_cmd(t_node *node, t_token *tokens, int i)
{
	if (!node)
		return (FALSE);
	if (node->token->hash == tokens[i].hash
		&& node->type == COMMAND && ft_count_str_in_arr(node->args) == 1)
		return (TRUE);
	if (is_expansion_cmd(node->left, tokens, i))
		return (TRUE);
	return (is_expansion_cmd(node->right, tokens, i));
}

void	eliminate_token(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	int	j;

	j = i;
	if (is_expansion_cmd(data->ast_root, tokens, i))
		return (convert_to_true_token(data, tokens, i));
	reorganize(prompt, tokens, i);
	while (j < prompt->n_tokens)
	{
		tokens[j].id -= 1;
		j++;
	}
}
