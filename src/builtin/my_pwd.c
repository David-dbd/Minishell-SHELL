/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 01:17:17 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/03 02:01:00 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Imprime el directorio de trabajo actual.
	- Usa getcwd() para obtenerlo dinámicamente.
	- Si falla, muestra el error.
*/

int	my_pwd(t_var *vars)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		pwd = get_var_value(vars, "PWD");
		if (!pwd)
			return (ft_printf_fd(STDERR, ERR_PWD), EXIT_FAILURE);
		ft_printf_fd(STDOUT, "%s\n", pwd);
		return (0);
	}
	ft_printf_fd(STDOUT, "%s\n", pwd);
	free(pwd);
	return (0);
}
