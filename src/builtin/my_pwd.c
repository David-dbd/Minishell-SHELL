/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 01:17:17 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/01 20:23:35 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Imprime el directorio de trabajo actual.
	- Usa getcwd() para obtenerlo dinámicamente.
	- Si falla, muestra el error.
*/

int	my_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_printf_fd(STDOUT, ERR_PWD);
		return (EXIT_FAILURE);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}
