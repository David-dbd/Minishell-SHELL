/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   which_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:58:35 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/07 16:49:16 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"
/*
static void	asignations(t_shell *data, t_token *token)
{
	 if (token->type == ASIGNATION)
	 	data->last_exit_code = asignation(data, token, LOCAL);
	 else if (token->type == PLUS_ASIGNATION)
	 	data->last_exit_code = asignation(data, token, PLUS_ASIGNATION);
	 else if (token->type == TEMP_ASIGNATION)
	 	data->last_exit_code = asignation(data, token, TEMP_ASIGNATION);
}

static void	env_commands(t_shell *data, t_token *token, t_node *node)
{
	if (ft_strcmp(token->value, BUILTIN_EXPORT) == 0)
		data->last_exit_code = my_export(data, data->prompt.tokens, &data->env, node);
	else if (ft_strcmp(token->value, BUILTIN_UNSET) == 0)
		data->last_exit_code = my_unset(data, &data->env, data->prompt.tokens);
	else if (ft_strcmp(token->value, BUILTIN_ENV) == 0)
		data->last_exit_code = my_env(data->env.vars);
	else if (ft_strcmp(token->value, BUILTIN_UNSET) == 0)
		data->exit_code = my_unset(data, &data->env, node->args);
}

static void	basic_builtins(t_shell *data, t_token *token, t_node *node)
{
	if (ft_strcmp(token->value, BUILTIN_ECHO) == 0)
		data->last_exit_code = my_echo(node->args);
	else if (ft_strcmp(token->value, BUILTIN_PWD) == 0)
		data->last_exit_code = my_pwd();
	else if (ft_strcmp(token->value, BUILTIN_EXIT) == 0)
		my_exit(data, &data->prompt, node->args);
	else if (ft_strcmp(token->value, BUILTIN_CD) == 0)
		data->last_exit_code = my_cd(data, tokens, token);
}
*/
void	which_builtin(t_shell *data, t_token *token, t_node *node)
{
	asignations(data, token);
	env_commands(data, token, node);
	basic_builtins(data, token, node);
}

