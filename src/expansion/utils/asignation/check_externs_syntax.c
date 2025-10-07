/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_externs_syntax.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:30:59 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/07 18:15:20 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

static int	check_case_1(t_token *tokens, t_token *token, int n_tokens)
{
	if (token->id < n_tokens - 1
		&& tokens[token->id - 1].type && tokens[token->id + 1].type)
	{
		if ((tokens[token->id - 1].type == AND
			|| tokens[token->id - 1].type == OR
			|| tokens[token->id - 1].type == ASIGNATION
			|| tokens[token->id - 1].type == DOUBLE_QUOTE
			|| tokens[token->id - 1].type == SINGLE_QUOTE
			|| (tokens[token->id - 1].type == BUILT_IN
				&& ft_strcmp(tokens[token->id - 1].value,
					BUILTIN_EXPORT) == 0))
			&& (tokens[token->id + 1].type == AND
				|| tokens[token->id + 1].type == OR
				|| tokens[token->id + 1].type == ASIGNATION
				|| tokens[token->id + 1].type == DOUBLE_QUOTE
				|| tokens[token->id + 1].type == SINGLE_QUOTE
				|| tokens[token->id + 1].type == REDIR_OUTPUT
				|| tokens[token->id + 1].type == WORD
				|| (tokens[token->id + 1].type == BUILT_IN
					&& ft_strcmp(tokens[token->id + 1].value,
						BUILTIN_EXPORT) == 0)))
			return (1);
	}
	return (0); //comillas hace que no se tenga en cuenta "hola""var=hola"
}

static int	check_case_2(t_token *tokens, t_token *token, int n_tokens)
{
	if (token->id > 0 && token->id <= n_tokens)
	{
		if (tokens[token->id - 1].type == AND
			|| tokens[token->id - 1].type == OR
			|| tokens[token->id - 1].type == DOUBLE_QUOTE
			|| tokens[token->id - 1].type == SINGLE_QUOTE
			|| tokens[token->id - 1].type == ASIGNATION
			|| tokens[token->id - 1].type == WORD
			|| ((tokens[token->id - 1].type == BUILT_IN
					&& ft_strcmp(tokens[token->id - 1].value,
						BUILTIN_EXPORT) == 0)))
			return (1);
	}
	return (0); //if not, then it needs to remain as WORD
}

//Aqui solo va a entrar si es local,
//porque si fuese por export, si existiria i - 1
/*static int	check_case_3(t_token *tokens, t_token *token)
{
	if (token->id == 0 && tokens[token->id + 1].type)
	{
		if (tokens[token->id + 1].type == AND
			|| tokens[token->id + 1].type == OR
			|| tokens[token->id + 1].type == DOUBLE_QUOTE
			|| tokens[token->id + 1].type == SINGLE_QUOTE
			|| (tokens[token->id + 1].type == ASIGNATION
				&& verify_till_valid_token(tokens, token->id))
			|| (tokens[token->id + 1].type == BUILT_IN
				&& ft_strcmp(tokens[token->id + 1].value, BUILTIN_EXPORT) == 0))
			return (1);
		else if (tokens[token->id + 1].type == WORD
			|| tokens[token->id + 1].type == COMMAND
			|| (tokens[token->id + 1].type == ASIGNATION
				&& !verify_till_valid_token(tokens, token->id))
			|| (tokens[token->id + 1].type == BUILT_IN
				&& ft_strcmp(tokens[token->id + 1].value, BUILTIN_EXPORT) != 0))
			return (TEMP_ASIGNATION); //Yep, that's right, it needs to
			ignore this token.
	}
	return (0);
}*/

static int	check_case_3(t_token *tokens, t_token *token, int n_tokens)
{
	if (token->id == 0 && n_tokens > 1 && tokens[token->id + 1].type)
	{
		if (tokens[token->id + 1].type == AND
			|| tokens[token->id + 1].type == OR
			|| tokens[token->id + 1].type == DOUBLE_QUOTE
			|| tokens[token->id + 1].type == SINGLE_QUOTE
			|| tokens[token->id + 1].type == ASIGNATION
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
		if (check_case_1(tokens, token, d->prompt.n_tokens))
			result = check_asignation_syntax(token, type);
		if (check_case_2(tokens, token, d->prompt.n_tokens))
			result = check_asignation_syntax(token, type);
		if (check_case_3(tokens, token, d->prompt.n_tokens))
			result = check_asignation_syntax(token, type);
	}
	else
		result = check_asignation_syntax(token, type);
	//en caso de que sea el primero y no haya ningun otro token
	return (result);
}
