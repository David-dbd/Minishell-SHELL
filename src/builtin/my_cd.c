/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 17:12:22 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/27 21:33:03 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	has_one_arg(char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
		i++;
	if (i == 1)
		return (TRUE);
	ft_printf_fd(STDERR, ERR_CD_TOO_MANY);
	return (FALSE);
}

//EN OBRAS...no esta listo

static char	*find_home_value_in_env(t_env *env)
{
	t_var	*var;

	var = env->vars;
	while (var)
	{
		if (ft_strcmp(var->key, "HOME") == 0)
		{
			if (var->value && var->value[0] != '\0')
				return (var->value);
		}
		var = var->next;
	}
	return (NULL);
}

static int	figure_out_information(char *ptr)
{
	struct	stat info;

	if (stat(ptr, &info) == ERROR) //STAT?
	{
		ft_printf_fd(STDERR, ERR_FILE_NOT_FOUND);
		return (EXIT_FAILURE);
	}
	if (!S_ISDIR(info.st_mode)) //S_ISDIR debe ser para saber si es un directorio o no. Seguramente ahi entra stat y el struct
	{
		ft_printf_fd(STDERR, ERR_NOT_DIR);
		return (ENOTDIR);
	}
	if (access(ptr, X_OK) == ERROR) //Para saber si se puede leer o no. Si se tiene permiso o no
	{
		ft_printf_fd(STDERR, ERR_PERM_DENIED);
		return (EACCES);
	}
	if (chdir(ptr) == ERROR) //Para cambiar de directorio
	{
		perror("minishell: chdir: ");
		return (EXIT_FAILURE);
	}
	return (SUCCESS);
}

static int	find_home(t_shell *data)
{
	char	new_cwd[PATH_MAX];
	char	cwd[PATH_MAX];
	char	*ptr;
	int		result;

	result = 0; 	//obtener el home y su valor para sabe si existe primero, si no se da por malo. Luego se hace stat para saber si el usuario lo cambio a un enlace o un archivo, es decir, ya no es valido.
	ptr = find_home_value_in_env(&data->env); //Se busca en el env primero. si no se encuentra se da error directamente
	if (!ptr)
	{
		ft_printf_fd(STDERR, ERR_HOME_NOT_SET); //O EL ERROR QUE SEA MEJOR
		return (EXIT_FAILURE); //ERROR?
	}
	result = figure_out_information(ptr);
	if (result != SUCCESS)
		return (result);
	if (!getcwd(new_cwd, sizeof(new_cwd)))
	{
		perror("minishell: cd");
		return (EXIT_FAILURE);
	}
	update_var(data, cwd, "OLDPWD");
	update_var(data, new_cwd, "PWD");
	return (SUCCESS);
}

int	my_cd(t_shell *data, char **args)
{
	char	new_cwd[PATH_MAX];
	char	cwd[PATH_MAX];
	int		result;

	if (!args || !*args)
	{
		result = find_home(data);
		return (result); //Se retorna int codigo que haya sido
	}
	if (!getcwd(cwd, sizeof(cwd))) // get "current_working_directory". Puede fallar con error 0. Esto puede ser para obtener el OLDPWD
	{
		perror("minishell: cd: ");
		return (EXIT_FAILURE);
	}
	if (!has_one_arg(args))
		return (EXIT_FAILURE);
	result = figure_out_information(*args);
	if (result != SUCCESS)
		return (result);
	if (!getcwd(new_cwd, sizeof(new_cwd)))
	{
		perror("minishell: cd: ");
		return (EXIT_FAILURE);
	}
	update_var(data, cwd, "OLDPWD");
	update_var(data, new_cwd, "PWD");
	return (0);
}
