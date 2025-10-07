/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:29:52 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/07 18:42:26 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

t_node	*parse_cmd(t_shell *data, t_token *tokens, int *i, int n_tokens)
{
	t_node	*left;
	int		start;

	left = NULL;
	start = *i;
	if (tokens[*i].type == TEMP_ASIGNATION)
		(*i)++;
	if (*i < n_tokens && tokens[*i].type
		&& (is_cmd_builtin_type(tokens[*i].type)
			|| is_real_assignation_type(tokens[*i].type)))
	{
		left = create_node(data, &tokens[*i], tokens[*i].type);
		(*i)++;
		if (is_asignation_type(tokens[*i].type))
			return (left);
		left->redir = get_redirs(data, tokens, COMMAND, i);
		left->args = get_args_for_binary(data, tokens, i);
		//left->temp_asig = get_temp_asignations();
		left->arg_types = get_arg_types(data, tokens, start, *i);
		return (left);
	}
	return (left);
}
