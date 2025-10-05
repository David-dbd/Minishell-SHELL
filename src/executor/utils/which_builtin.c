/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   which_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:58:35 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/05 22:45:11 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static void	asignations(t_shell *data, t_token *token)
{
	if (token->type == ASIGNATION)
		data->last_exit_code = asignation(data, token, LOCAL);
	else if (token->type == PLUS_ASIGNATION)
		data->last_exit_code = asignation(data, token, PLUS_ASIGNATION);
	else if (token->type == TEMP_ASIGNATION)
		data->last_exit_code = asignation(data, token, TEMP_ASIGNATION);
}

static void	env_commands(t_shell *data, t_token *tokens, t_token *token)
{
	int	flag_error;

	flag_error = 0;
	if (ft_strmatch_cmp(token->value, BUILTIN_EXPORT, &flag_error) == 0)
		data->last_exit_code = my_export(data, tokens, token, &data->env);
	else if (ft_strmatch_cmp(token->value, BUILTIN_UNSET, &flag_error) == 0)
		data->last_exit_code = my_unset(data, &data->env, tokens);
	else if (ft_strmatch_cmp(token->value, BUILTIN_ENV, &flag_error) == 0)
		data->last_exit_code = my_env(data->env.vars);
	else if (ft_strmatch_cmp(token->value, BUILTIN_UNSET, &flag_error) == 0)
		data->exit_code = my_unset(data, &data->env, tokens);
}

static void	basic_builtins(t_shell *data, t_token *tokens, t_token *token)
{
	int	flag_error;

	flag_error = 0;
	if (ft_strmatch_cmp(token->value, BUILTIN_ECHO, &flag_error) == 0)
		data->last_exit_code = my_echo(&data->prompt, token);
	else if (ft_strmatch_cmp(token->value, BUILTIN_PWD, &flag_error) == 0)
		data->last_exit_code = my_pwd(data);
	else if (ft_strmatch_cmp(token->value, BUILTIN_CD, &flag_error) == 0)
		data->last_exit_code = my_cd(data, tokens, token);
	else if (ft_strmatch_cmp(token->value, BUILTIN_EXIT, &flag_error) == 0)
		my_exit(data, &data->prompt, token);
}

void	which_builtin(t_shell *data, t_token *tokens, t_token *token)
{
	asignations(data, token);
	env_commands(data, tokens, token);
	basic_builtins(data, tokens, token);
}

