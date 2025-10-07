/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_or.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:20:29 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/07 20:24:23 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_node	*parse_and_or(t_shell *data, t_token *tokens, int *i, int n_tokens)
{
	t_node	*central;
	t_node	*left;
	t_node	*right;

	right = NULL;
	left = parse_pipes(data, tokens, i, n_tokens);
	if (!left)
		return (NULL);
	while (*i < n_tokens && (tokens[*i].type == AND || tokens[*i].type == OR))
	{
		central = create_node(data, &tokens[*i], tokens[*i].type);
		safe_index_plus(i, n_tokens);
		right = parse_pipes(data, tokens, i, n_tokens);
		if (!right)
			return (NULL);
		central->left = left;
		central->right = right;
		left = central;
	}
	return (left);
}
