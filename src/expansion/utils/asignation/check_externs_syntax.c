/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_externs_syntax.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:30:59 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/20 23:54:21 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

static int	check_case_both_sides_aux(t_token *tokens, t_token *token)
{
	if ((tokens[token->id - 1].type == AND || tokens[token->id - 1].type == OR
			|| tokens[token->id - 1].type == PAREN_OPEN
			|| tokens[token->id - 1].type == ASIGNATION
			|| tokens[token->id - 1].type == PLUS_ASIGNATION
			|| tokens[token->id - 1].type == DOUBLE_QUOTE
			|| tokens[token->id - 1].type == SINGLE_QUOTE
			|| tokens[token->id - 1].type == SEMICOLON
			|| tokens[token->id - 1].type == PAREN_OPEN
			|| (tokens[token->id - 1].type == BUILT_IN
				&& ft_strcmp(tokens[token->id - 1].value,
					BUILTIN_EXPORT) == 0))
		&& (tokens[token->id + 1].type == AND
			|| tokens[token->id + 1].type == OR
			|| tokens[token->id + 1].type == ASIGNATION
			|| tokens[token->id + 1].type == PLUS_ASIGNATION
			|| tokens[token->id + 1].type == DOUBLE_QUOTE
			|| tokens[token->id - 1].type == PAREN_CLOSE
			|| tokens[token->id + 1].type == SINGLE_QUOTE
			|| tokens[token->id + 1].type == REDIR_OUTPUT
			|| tokens[token->id + 1].type == SEMICOLON
			|| tokens[token->id + 1].type == WORD
			|| (tokens[token->id + 1].type == BUILT_IN
				&& ft_strcmp(tokens[token->id + 1].value, "export") == 0)))
		return (1);
	return (0);
}

static int	check_case_both_sides(t_token *tokens, t_token *token, int n_tokens)
{
	if (token->id < n_tokens - 1
		&& tokens[token->id - 1].type && tokens[token->id + 1].type)
	{
		if (check_case_both_sides_aux(tokens, token))
			return (1);
	}
	return (0); //comillas hace que no se tenga en cuenta "hola""var=hola"
}

static int	check_case_only_left(t_token *tokens, t_token *token, int n_tokens)
{
	if (token->id > 0 && token->id <= n_tokens)
	{
		if (tokens[token->id - 1].type == AND
			|| tokens[token->id - 1].type == OR
			|| tokens[token->id - 1].type == DOUBLE_QUOTE
			|| tokens[token->id - 1].type == SINGLE_QUOTE
			|| tokens[token->id - 1].type == ASIGNATION
			|| tokens[token->id - 1].type == PLUS_ASIGNATION
			|| tokens[token->id - 1].type == SEMICOLON
			|| tokens[token->id - 1].type == WORD
			|| ((tokens[token->id - 1].type == BUILT_IN
					&& ft_strcmp(tokens[token->id - 1].value,
						BUILTIN_EXPORT) == 0)))
			return (1);
	}
	return (0); //if not, then it needs to remain as WORD
}

static int	check_case_only_right(t_token *tokens, t_token *token, int n_tokens)
{
	if (token->id == 0 && n_tokens > 1 && tokens[token->id + 1].type)
	{
		if (tokens[token->id + 1].type == AND
			|| tokens[token->id + 1].type == OR
			|| tokens[token->id + 1].type == DOUBLE_QUOTE
			|| tokens[token->id + 1].type == SINGLE_QUOTE
			|| tokens[token->id + 1].type == ASIGNATION
			|| tokens[token->id + 1].type == PLUS_ASIGNATION
			|| tokens[token->id + 1].type == SEMICOLON
			|| (tokens[token->id + 1].type == BUILT_IN
				&& ft_strcmp(tokens[token->id + 1].value, BUILTIN_EXPORT) == 0)
			|| tokens[token->id + 1].type == WORD
			|| tokens[token->id + 1].type == REDIR_OUTPUT
			|| tokens[token->id + 1].type == COMMAND
			|| (tokens[token->id + 1].type == BUILT_IN
				&& ft_strcmp(tokens[token->id + 1].value, BUILTIN_EXPORT) != 0))
			return (1);
	}
	return (0);
}

int	check_externs_syntax(t_shell *d, t_token *tokens, t_token *token, int type)
{
	int	result;

	result = FALSE;
	if (token->id >= 1 && token->id < d->prompt.n_tokens
		&& (token->id + 1 < d->prompt.n_tokens
			&& (tokens[token->id + 1].type || tokens[token->id - 1].type))
		&& (token->type == ASIGNATION || token->type == WORD))
	{
		if (check_case_both_sides(tokens, token, d->prompt.n_tokens))
			result = check_asignation_syntax(token, type);
		if (check_case_only_left(tokens, token, d->prompt.n_tokens))
			result = check_asignation_syntax(token, type);
		if (check_case_only_right(tokens, token, d->prompt.n_tokens))
			result = check_asignation_syntax(token, type);
	}
	else
		result = check_asignation_syntax(token, type);
	//en caso de que sea el primero y no haya ningun otro token
	return (result);
}
