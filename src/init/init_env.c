/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 01:02:53 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/07 21:59:39 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Asigna un PATH por defecto si no hay entorno.
*/

void	path_null_no_env(t_shell *data, char **path)
{
	*path = strdup("/usr/bin:/bin:/usr/local/bin");
	if (!*path)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
}

/*
	Inicializa variables de entorno básicas si no hay 'envp',
	como PWD, SHLVL y PATH mínimo.
*/

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

/*
	Convierte cada cadena de 'envp' en key/value y las añade al entorno
	del shell.
*/

static void	init_env(t_shell *data, char **envp)
{
	int		i;
	int		j;
	int		k;
	char	*key;
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
		if (!key || !value)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		add_var(data, key, value, ENV);
		i++;
	}
}

/*
	Inicializa el entorno del shell: usa 'envp' si existe o crea uno básico.
*/

void	init_enviroment(t_shell *data, char **envp)
{
	if (!envp || !*envp)
		init_no_env(data);
	else
		init_env(data, envp);
}
