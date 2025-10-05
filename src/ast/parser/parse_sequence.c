/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sequence.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:04:50 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/05 19:34:39 by migarrid         ###   ########.fr       */
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
		&& (tokens[*i].type == SEMICOLON || tokens[*i].type == BACKGROUND))
	{
		central = create_node(data, &tokens[*i], tokens[*i].type);
		(*i)++;
		right = parse_and_or(data, tokens, i, n_tokens);
		if (!right)
			return (NULL);
		central->left = left;
		central->right = right;
		left = central;
	}
	return (left);
}
