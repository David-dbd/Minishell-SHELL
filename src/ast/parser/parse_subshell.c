/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:24:05 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/17 14:21:31 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_node	*parse_subshell(t_shell *data, t_token *tokens, int *i, int n_tokens)
{
	t_node	*central;
	t_node	*left;

	if (*i < n_tokens && tokens[*i].type == PAREN_OPEN)
	{
		central = create_node(data, &tokens[*i], SUBSHELL);
		if (!central)
			return (NULL);
		safe_index_plus(i, n_tokens);
		left = parse_sequence(data, tokens, i, n_tokens);
		if (!left)
			return (clean_node(&central), NULL);
		if (*i < n_tokens && tokens[*i].type == PAREN_CLOSE)
			(*i)++;
		central->redir = get_redirs(data, tokens, i, SUBSHELL);
		central->background = get_background(tokens, n_tokens, i);
		central->left = left;
		return (central);
	}
	else if (*i < n_tokens && is_real_assignation_type(tokens[*i].type))
	{
		central = parse_assignations(data, tokens, i, n_tokens);
		if (!central)
			return (NULL);
		return (central);
	}
	central = parse_cmd(data, tokens, i, n_tokens);
	return (central);
}
