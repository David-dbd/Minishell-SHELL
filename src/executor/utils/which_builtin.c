/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   which_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:58:35 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/31 17:07:58 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static void	extras(t_shell *data, t_token *token, t_node *node)
{
	if (ft_strcmp(token->value, BUILTIN_ALIAS) == 0)
		data->exit_code = my_alias(data, data->extras.cmd, node->args);
	else if (ft_strcmp(token->value, BUILTIN_UNALIAS) == 0)
		data->exit_code = my_unalias(data->extras.cmd, node->args);
}

static void	asignations(t_shell *data, t_token *token)
{
	if (token->type == ASIGNATION)
		data->exit_code = asignation(data, token, LOCAL);
	else if (token->type == PLUS_ASIGNATION)
		data->exit_code = asignation(data, token, PLUS_ASIGNATION);
}

static void	env_cmds(t_shell *data, t_env *env, t_token *token, t_node *node)
{
	if (ft_strcmp(token->value, BUILTIN_EXPORT) == 0)
		data->exit_code = my_export(data, data->prompt.tokens, env, node);
	else if (ft_strcmp(token->value, BUILTIN_UNSET) == 0)
		data->exit_code = my_unset(data, env, node->args);
	else if (ft_strcmp(token->value, BUILTIN_ENV) == 0)
		data->exit_code = my_env(env->vars, node->args);
}

static void	basic_builtins(t_shell *data, t_token *token, t_node *node)
{
	if (ft_strcmp(token->value, BUILTIN_ECHO) == 0)
		data->exit_code = my_echo(node->args);
	else if (ft_strcmp(token->value, BUILTIN_PWD) == 0)
		data->exit_code = my_pwd();
	else if (ft_strcmp(token->value, BUILTIN_EXIT) == 0)
		my_exit(data, node->args);
	else if (ft_strcmp(token->value, BUILTIN_CD) == 0)
		data->exit_code = my_cd(data, node->args);
}

void	which_builtin(t_shell *data, t_token *token, t_node *node)
{
	asignations(data, token);
	env_cmds(data, &data->env, token, node);
	basic_builtins(data, token, node);
	extras(data, token, node);
}
