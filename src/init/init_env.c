/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 01:02:53 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/03 21:48:02 by migarrid         ###   ########.fr       */
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
}

static void	init_env(t_shell *data, char **envp)
{
	int		i;
	int 	j;
	int		k;
	char 	*key;
	char	*value;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] != '=')
			j++;
		k = j;
		while (envp[i][k] != '\0')
			k++;
		key = ft_substr(envp[i], 0, j);
		value = ft_substr(envp[i], j + 1, k);
		add_var(data, key, value, ENV);
		i++;
	}
}

void	init_enviroment(t_shell *data, char **envp)
{
	if (!envp || !*envp)
		init_no_env(data);
	else
		init_env(data, envp);
}
