/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 16:42:21 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/30 01:53:34 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	exec_subshell(t_shell *data, t_node *node, t_exec *exec, int mode)
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
		apply_properties(data, node, FATHER);
		executor_recursive(data, node->left, exec, FATHER); //tiene que ser fhater ya que si es child en cuanto ejecuta el primer cmd, hace exit y se queda sin ejecutar el resto
		exit_succes(data, NULL, data->exit_code);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		data->exit_code = 128 + WTERMSIG(status);
	if (mode == CHILD)
		exit_succes(data, NULL, data->exit_code);
}
