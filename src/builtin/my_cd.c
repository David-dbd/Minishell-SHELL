/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 17:12:22 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/13 00:16:56 by migarrid         ###   ########.fr       */
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

void	cwd_has_been_deleted(t_var *vars, char *old_cwd, char **new_dir)
{
	char	*cwd;
	char	*last_slash;

	cwd = get_var_value(vars, "PWD");
	if (!cwd || !*cwd)
	{
		ft_strlcpy(old_cwd, "/", PATH_MAX);
		*new_dir = "/";
		return ;
	}
	last_slash = ft_strrchr(cwd, '/');
	if (last_slash && last_slash != cwd)
		*last_slash = '\0';
	else if (last_slash == cwd)
		*(last_slash + 1) = '\0';
	ft_strlcpy(old_cwd, cwd, PATH_MAX);
	*new_dir = cwd;
}

static int	validate_and_move(t_shell *data, t_var *vars, char *new_dir)
{
	char		new_cwd[PATH_MAX];
	char		old_cwd[PATH_MAX];
	struct stat	info;

	if (ft_strcmp(new_dir, "-") == 0) // expandir real OLD_PWD
	{
		new_dir = get_var_value(vars, "OLDPWD");
		ft_printf_fd(STDOUT, "%s\n", new_dir);
	}
	if (!getcwd(old_cwd, sizeof(old_cwd))) // actual dir para OLDPWD
		cwd_has_been_deleted(vars, old_cwd, &new_dir);
	if (stat(new_dir, &info) == ERROR) // Para saber si existe
		return (ft_printf_fd(STDERR, ERR_FILE_NOT_FOUND, new_dir), EXIT_FAIL);
	if (!S_ISDIR(info.st_mode)) // Para saber si es un dir
		return (ft_printf_fd(STDERR, ERR_NOT_DIR, new_dir), ENOTDIR);
	if (access(new_dir, X_OK) == ERROR) //Para saber si tiene permiso o no
		return (ft_printf_fd(STDERR, ERR_PERM_DENIED, new_dir), EACCES);
	if (chdir(new_dir) == ERROR) //Para cambiar de directorio
		return (perror("minishell: cd: "), EXIT_FAILURE);
	if (!getcwd(new_cwd, sizeof(new_cwd))) // nuevo dir para PWD
		return (perror("minishell: cd: "), EXIT_FAILURE);
	update_var(data, old_cwd, "OLDPWD");
	update_var(data, new_cwd, "PWD");
	return (OK);
}

static int	find_home_and_move(t_shell *data)
{
	char	*home;
	int		result;

	result = 0;
	home = get_var_value(data->env.vars, "HOME");
	if (!home)
		return (ft_printf_fd(STDERR, ERR_HOME_NOT_SET), EXIT_FAILURE);
	result = validate_and_move(data, data->env.vars, home);
	return (SUCCESS);
}

int	my_cd(t_shell *data, char **args)
{
	int		result;

	if (!args || !*args)
	{
		result = find_home_and_move(data);
		return (result);
	}
	if (has_one_arg(args))
	{
		result = validate_and_move(data, data->env.vars, args[0]);
		return (result);
	}
	return (FAIL);
}
