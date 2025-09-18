/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   which_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:58:35 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/18 02:38:30 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	which_builtin(t_shell *data, t_token *tokens, t_token *token)
{
	if (token->type == ASIGNATION)
		data->exit_code = asignation(data, token, LOCAL);
	if (ft_strcmp(token->value, BUILTIN_ECHO) == 0)
		data->exit_code = my_echo(&data->prompt, token);
	else if (ft_strcmp(token->value, BUILTIN_PWD) == 0)
		data->exit_code = my_pwd(data);
	else if (ft_strcmp(token->value, BUILTIN_ECHO) == 0)
		data->exit_code = my_echo(&data->prompt, token);
	else if (ft_strcmp(token->value, BUILTIN_EXPORT) == 0)
		data->exit_code = my_export(data, token, &data->env);
	else if (ft_strcmp(token->value, BUILTIN_UNSET) == 0)
		data->exit_code = my_unset(data, &data->env, tokens);
	else if (ft_strcmp(token->value, BUILTIN_ENV) == 0)
		data->exit_code = my_env(data->env.vars);
	else if (ft_strcmp(token->value, BUILTIN_EXIT) == 0)
		my_exit(data, &data->prompt, token);
}
