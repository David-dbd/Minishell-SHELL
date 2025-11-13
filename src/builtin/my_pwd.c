/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 01:17:17 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/13 01:18:50 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Imprime el directorio de trabajo actual.
	- Usa getcwd() para obtenerlo dinámicamente.
	- Si falla, muestra el error.
*/

// static int same_dir(char *env_value, char *currect_dir)
// {
// 	struct	stat s1;
// 	struct	stat s2;

// 	if (stat(env_value, &s1) == -1 || stat(currect_dir, &s2) == -1)
// 	{
// 		perror("minishell: stat: ");
// 		return (FAILURE);
// 	}
// 	else if (s1.st_ino == s2.st_ino && s1.st_dev == s2.st_dev)
// 		return (SUCCESS);
// 	return (FAILURE);
// }

// int	my_pwd(t_shell *data)
// {
// 	char *env_pwd;
// 	char real_pwd[PATH_MAX];

// 	env_pwd = get_var_value(data->env.vars, "PWD");//se consigue del env nuestro
// 	if (env_pwd && same_dir(env_pwd, "."))//se compara con stat, de modo que asi sabemos si es valido o no
// 	{
// 		ft_printf_fd(STDOUT, "%s\n", env_pwd);
// 		return (0);
// 	}
// 	else if (!getcwd(real_pwd, sizeof(real_pwd)))//si lo anterior no era valido entonces se utiliza getcwd
// 	{
// 		perror("minishell: getcwd: ");
// 		return (FAIL);
// 	}
// 	update_var(data, real_pwd, "PWD");//la actulizamos de una vez aunque no es el comportamiento de bash pero es mas pro asi...
// 	ft_printf_fd(STDOUT, "%s\n", real_pwd);
// 	return (0);
// }

/*
	Imprime el directorio de trabajo actual.
	- Usa getcwd() para obtenerlo dinámicamente.
	- Si falla, muestra el error.
*/

int	my_pwd(t_shell *data)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		pwd = get_var_value(data->env.vars, "PWD");
		if (!pwd)
			return (ft_printf_fd(STDERR, ERR_PWD), EXIT_FAILURE);
		ft_printf_fd(STDOUT, "%s\n", pwd);
		return (0);
	}
	ft_printf_fd(STDOUT, "%s\n", pwd);
	update_var(data, pwd, "PWD");
	free(pwd);
	return (0);
}
