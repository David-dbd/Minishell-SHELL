/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 01:02:53 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/03 18:36:26 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	init_no_env(t_shell *data)
{
	char	**var;

	var = ft_calloc(6, sizeof(char *));
	var[0] = strdup("PWD");
	var[1] = getcwd(NULL, 0);
	var[2] = strdup("SHLVL");
	var[3] = strdup("1");
	var[4] = strdup("_");
	var[5] = strdup("/usr/bin/env");
	if (!var[0] || !var[1] || !var[2] || !var[3] || !var[4] || !var[5])
		return (ft_free_str_array(var), (void)exit_error(data, ERR_MALLOC, 1));
	add_var(data, var[0], var[1], ENV);
	add_var(data, var[2], var[3], ENV);
	add_var(data, var[4], var[5], ENV);
	shell_env(data->env.vars);
}

static void	init_env()
{
}

void	init_enviroment(t_shell *data, char **envp)
{
	if (!envp || !*envp)
	{
		init_no_env(data);
	}
	else
	{
		init_env();
	}
}
