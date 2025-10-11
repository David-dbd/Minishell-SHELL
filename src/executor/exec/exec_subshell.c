/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 16:42:21 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/11 07:35:23 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	exec_subshell(t_shell *data, t_node *node, int mode)
{
	int		status;
	pid_t	pid;

	status = 0;
	pid = fork();
	if (pid == ERROR)
		exit_error(data, ERR_FORK, FAIL);
	if (pid == 0)
	{
		setup_signals_child();
		executor_recursive(data, node->left, FATHER);//tiene que ser fhater ya que si es child en cuanto ejecuta el primer cmd, hace exit y se queda sin ejecutar el resto
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		data->exit_code = 128 + WTERMSIG(status);
}
