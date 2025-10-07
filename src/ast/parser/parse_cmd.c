/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:29:52 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/07 19:22:34 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static	t_node *special_cases(t_shell *d, t_token *tokens, int *i, int n_tokens)
{
	t_node	*left;

	if (tokens[*i].type == TEMP_ASIGNATION)
	{
		(*i)++;
	}
	else if (tokens[*i].type == REDIR_OUTPUT)
	{
		left = create_fake_node(d, COMMAND);
		left->redir = get_redirs(d, tokens, COMMAND, i);
		return (left);
	}
	return (NULL);
}

static void	get_information(t_shell *dat, t_token *tokens, int *i, t_node *left)
{
	int		start;

	start = *i;
	left->redir = get_redirs(dat, tokens, COMMAND, i);
	left->args = get_args_for_binary(dat, tokens, i);
	//left->temp_asig = get_temp_asignations();
	left->arg_types = get_arg_types(dat, tokens, start, *i);
}

t_node	*parse_cmd(t_shell *data, t_token *tokens, int *i, int n_tokens)
{
	t_node	*left;

	left = NULL;
	left = special_cases(data, tokens, i, n_tokens);
	if (left)
		return (left);
	if (*i < n_tokens && tokens[*i].type
		&& (is_cmd_builtin_type(tokens[*i].type)
		|| tokens[*i].type == ASIGNATION || tokens[*i].type == PLUS_ASIGNATION))
	{
		left = create_node(data, &tokens[*i], tokens[*i].type);
		if (is_asignation_type(tokens[*i].type))
			return (left);
		(*i)++;
		get_information(data, tokens, i, left);
		return (left);
	}
	return (left);
}
