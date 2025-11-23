/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 17:12:22 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/23 11:26:56 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * Verifica que el comando 'cd' tenga exactamente un argumento.
*/

static int	has_one_arg(char **args)
{
	int	i;
	int	words_flag;

	i = 0;
	words_flag = 0;
	while (args[i] != NULL)
		i++;
	if (i == 1)
	{
		i = 0;
		while (args[0][i] != '\0')
		{
			if (!ft_isspace(args[0][i]))
				words_flag = TRUE;
			i++;
		}
		if (words_flag)
			return (TRUE);
		else if (!words_flag)
			return (IGNORE);
	}
	ft_printf_fd(STDERR, ERR_CD_TOO_MANY);
	return (FALSE);
}

/*
 * Maneja el caso en que el directorio de trabajo actual fue borrado.
 * Si `getcwd` falla, esta funcion asume que el directorio PWD ya
 * no existe. Intenta restaurar el PWD a su directorio padre o a
 * la raiz (`/`), actualizando el path en `old_cwd` y `new_dir`.
 */

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

/*
 * Valida permisos, existencia y tipo del directorio, y cambia a el.
 * Primero maneja el caso especial de `cd -` usando OLDPWD. Luego
 * verifica si el directorio existe (`stat`), si es un directorio
 * (`S_ISDIR`) y si se tiene permiso de ejecucion (`access`). Si todo
 * es correcto, usa `chdir` para moverse y actualiza OLDPWD y PWD.
 */

static int	validate_and_move(t_shell *data, t_var *vars, char *new_dir)
{
	char		new_cwd[PATH_MAX];
	char		old_cwd[PATH_MAX];
	struct stat	info;

	if (ft_strcmp(new_dir, "-") == 0)
	{
		new_dir = get_var_value(vars, "OLDPWD");
		if (!new_dir)
			return (ft_printf_fd(STDOUT, ERR_CD_OLDPWD), EXIT_FAILURE);
		ft_printf_fd(STDOUT, "%s\n", new_dir);
	}
	if (!getcwd(old_cwd, sizeof(old_cwd)))
		cwd_has_been_deleted(vars, old_cwd, &new_dir);
	if (stat(new_dir, &info) == ERROR)
		return (ft_printf_fd(STDERR, ERR_FILE_NOT_FOUND, new_dir), EXIT_FAIL);
	if (!S_ISDIR(info.st_mode))
		return (ft_printf_fd(STDERR, ERR_NOT_DIR, new_dir), ENOTDIR);
	if (access(new_dir, X_OK) == ERROR)
		return (ft_printf_fd(STDERR, ERR_PERM_DENIED, new_dir), EACCES);
	if (chdir(new_dir) == ERROR)
		return (perror("minishell: cd: "), EXIT_FAILURE);
	if (!getcwd(new_cwd, sizeof(new_cwd)))
		return (perror("minishell: cd: "), EXIT_FAILURE);
	update_var(data, old_cwd, "OLDPWD");
	return (update_var(data, new_cwd, "PWD"), OK);
}

/*
 * Busca el directorio HOME y ejecuta el cambio de directorio.
 * Obtiene el valor de la variable de entorno HOME. Si existe,
 * llama a `validate_and_move` para intentar moverse a esa ruta.
 * Si HOME no esta definido, imprime un error y retorna fallo.
*/

static int	find_home_and_move(t_shell *data)
{
	char	*home;

	home = get_var_value(data->env.vars, "HOME");
	if (!home)
		return (ft_printf_fd(STDERR, ERR_HOME_NOT_SET), EXIT_FAILURE);
	validate_and_move(data, data->env.vars, home);
	return (SUCCESS);
}

/*
 * Funcion principal del builtin `cd` (change directory).
 * Si no se proporciona ningun argumento, llama a `find_home_and_move`
 * para ir al directorio HOME. Si se proporciona un argumento, verifica
 * que sea uno solo con `has_one_arg` y luego llama a
 * `validate_and_move` para intentar el cambio de directorio.
 */

int	my_cd(t_shell *data, char **args)
{
	int		result;

	if (!args || !*args)
	{
		result = find_home_and_move(data);
		return (result);
	}
	result = has_one_arg(args);
	if (result == TRUE)
	{
		result = validate_and_move(data, data->env.vars, args[0]);
		return (result);
	}
	else if (result == IGNORE)
		return (FAILURE);
	return (FAIL);
}
