/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 20:43:35 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/16 21:22:12 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

char	**make_envp(t_shell *data, t_env *env, t_var *vars)
{
	int		i;
	int		substring_len;
	char	**envp;

	i = 0;
	envp = ft_calloc(env->size + 1, sizeof(char *));
	if (!envp)
		return (exit_error(data, ERR_MALLOC, EXIT_FAIL), NULL);
	while(i < env->size)
	{
		if (vars->type == ENV)
		{
			envp[i] = ft_strjoin_multi(3, vars->key, "=", vars->value);
			if (envp[i])
				return (exit_error(data, ERR_MALLOC, EXIT_FAIL), NULL);
		}
		vars = vars->next;
		i++;
	}
	return (envp);
}
