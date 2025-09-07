/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 01:17:17 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/07 20:07:15 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	shell_pwd(t_shell *data)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		ft_printf_fd(STDERR, "%s", ERR_PWD);
	printf("%s\n", pwd);
	free(pwd);
}
