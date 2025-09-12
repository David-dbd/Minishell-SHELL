/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   which_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 21:58:35 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/11 22:21:58 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	which_builtin(t_shell *data, t_token *tokens, t_token *token)
{
	if (ft_strcmp(token->value, BUILTIN_ECHO) == 0)
		data->last_exit_code = my_echo(tokens);
	else if (ft_strcmp(token->value, BUILTIN_PWD) == 0)
		data->last_exit_code = my_pwd(data);
	// else if (ft_strcmp(token->value, BUILTIN_CD) == 0)
	// 	// data->last_exit_code = my_cd();
	else if (ft_strcmp(token->value, BUILTIN_EXPORT) == 0)
		data->last_exit_code = my_export(data, token, &data->env);
	else if (ft_strcmp(token->value, BUILTIN_ENV) == 0)
		data->last_exit_code = my_env(data->env.vars);
	// else if (ft_strcmp(token->value, BUILTIN_UNSET) == 0)
	// 	// data->last_exit_code = my_unset();
	else if (ft_strcmp(token->value, BUILTIN_EXIT) == 0)
		my_exit(data, tokens);
}
