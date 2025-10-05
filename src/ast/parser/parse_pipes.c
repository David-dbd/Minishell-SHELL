/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:21:21 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/05 20:58:29 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_node	*parse_pipes(t_shell *data, t_token *tokens, int *i, int n_tokens)
{
	t_node	*central;
	t_node	*left;
	t_node	*right;

	right = NULL;
	left = parse_subshell(data, tokens, i, n_tokens);
	if (!left)
		return (NULL);
	while (*i < n_tokens && tokens[*i].type == PIPE)
	{
		central = create_node(data, &tokens[*i], PIPE);
		(*i)++;
		right = parse_subshell(data, tokens, i, n_tokens);
		if (!right)
			return (NULL);
		central->left = left;
		central->right = right;
		left = central;
	}
	return (left);
}
