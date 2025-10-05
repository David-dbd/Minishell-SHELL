/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:24:05 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/05 21:59:38 by migarrid         ###   ########.fr       */
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
		if (*i < n_tokens)
			(*i)++;
		left = parse_sequence(data, tokens, i, n_tokens);
		if (*i < n_tokens && tokens[*i].type == PAREN_CLOSE)
			(*i)++;
		central->redir = get_redirs(data, tokens, SUBSHELL, i);
		central->left = left;
		return (central);
	}
	central = parse_cmd(data, tokens, i, n_tokens);
	return (central);
}
