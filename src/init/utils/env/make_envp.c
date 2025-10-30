/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 20:43:35 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/29 18:01:51 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Crea un array `envp` con las variables de entorno en formato "key=value",
	recorriendo la lista enlazada de `vars`.
	- Reserva memoria dinámica para `envp` según el tamaño en `env->size`.
	- Solo copia las variables con tipo `ENV`.
*/

char	**make_envp(t_shell *data, t_env *env, t_var *vars)
{
	int		i;
	char	**envp;

	i = 0;
	envp = ft_calloc(env->size + 1, sizeof(char *));
	if (!envp)
		return (exit_error(data, ERR_MALLOC, EXIT_FAIL), NULL);
	while (vars)
	{
		if (vars->type == ENV)
		{
			envp[i] = ft_strjoin_multi(3, vars->key, "=", vars->value);
			if (!envp[i])
				return (exit_error(data, ERR_MALLOC, EXIT_FAIL), NULL);
		}
		vars = vars->next;
		i++;
	}
	return (envp);
}

char	**upadte_envp(t_shell *data, t_env *env, t_var *vars)
{
	int		i;
	char	**envp;

	i = 0;
	if (env->envp)
		ft_free_str_array(env->envp);
	envp = ft_calloc(env->size + 1, sizeof(char *));
	if (!envp)
		return (exit_error(data, ERR_MALLOC, EXIT_FAIL), NULL);
	while (vars)
	{
		if (vars->type == ENV)
		{
			envp[i] = ft_strjoin_multi(3, vars->key, "=", vars->value);
			if (!envp[i])
				return (exit_error(data, ERR_MALLOC, EXIT_FAIL), NULL);
		}
		vars = vars->next;
		i++;
	}
	return (envp);
}

