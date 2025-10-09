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
