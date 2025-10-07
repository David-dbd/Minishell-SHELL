/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sequence.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:04:50 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/07 20:43:49 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_node	*parse_sequence(t_shell *data, t_token *tokens, int *i, int n_tokens)
{
	t_node	*central;
	t_node	*left;
	t_node	*right;

	right = NULL;
	left = parse_and_or(data, tokens, i, n_tokens);
	if (!left)
		return (NULL);
	while (*i < n_tokens
		&& (tokens[*i].type == SEMICOLON))
	{
		central = create_node(data, &tokens[*i], tokens[*i].type);
		central->left = left;
		safe_index_plus(i, n_tokens);
		right = parse_and_or(data, tokens, i, n_tokens);
		if (!right)
			return (central);
		central->right = right;
		left = central;
	}
	return (left);
}
