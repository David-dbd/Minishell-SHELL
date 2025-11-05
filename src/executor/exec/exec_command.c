/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:23:14 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/05 00:16:14 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	En exec_command se revisa el parámetro `mode`, que puede ser
	PARENT o CHILD. Este valor indica si la función se está
	ejecutando desde el proceso padre o desde un hijo.

	Si el nodo actual es un PIPE, el modo siempre será CHILD.
	Esto se debe a que el nodo PIPE ya realiza dos fork para
	ejecutar los comandos a sus lados. Por lo tanto, no se deben
	hacer más fork dentro de exec_command, ya que eso duplicaría
	los procesos innecesariamente (hasta 4 fork en total).

	En cambio, si el modo es PARENT, significa que el nodo no es
	un PIPE, sino un operador o comando simple. En ese caso, sí
	se debe hacer un fork, porque aún no se ha creado ningún
	proceso hijo para ejecutar el comando.

	Importante: que sea modo PARENT no implica que el comando se
	ejecute directamente en el padre. Todo comando (excepto los
	built-in) debe ejecutarse en un proceso hijo. La diferencia
	es que en modo PARENT se hace un único fork, mientras que en
	modo CHILD no se hace ninguno porque ya se está dentro del
	hijo creado por el PIPE.

	En ambos modos, primero se aplican las propiedades del nodo:
	asignaciones temporales y redirecciones. Luego se ejecuta el
	comando, y se revisa si tiene la propiedad `background`
	activada. Si no la tiene, se espera con waitpid. Si sí la
	tiene, se imprime el PID y se omite el wait.
*/

void	wait_cmd_background(t_shell *data, t_node *node, pid_t pid)
{
	int	sig;
	int	status;

	if (!node->background) //si no tiene background se hace el waitpid
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			data->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			data->exit_code = 128 + WTERMSIG(status);
			if (sig == SIGQUIT)
				ft_printf_fd(STDERR, "Quit (core dumped)\n");
			else if (sig == SIGINT)
				ft_printf_fd(STDERR, "\n");
		}
	}
	g_signal[0] = SIG_INTERACTIVE;
	if (node->background)
	{
		data->last_background_pid = pid;
		ft_printf_fd(STDOUT, "[&] %d\n", pid);
		data->exit_code = OK;
	}
}

void	execute_cmd_from_child(t_shell *data, t_node *node, t_env *env)
{
	char	*path;

	apply_properties(data, node, CHILD);
	path = get_path(data, node->token->value, env->envp);
	add_var_and_envp(data, ft_strdup("_"), path, ENV);
	execve(path, node->args, env->envp);
	exit_error(data, ERR_EXEC, EXIT_CMD_NOT_EXEC, node->token->value);
	//si es child solo se retorna el exit_code ya que el subshell o pipe
	//haran el waitpid
}

void	execute_cmd_from_father(t_shell *data, t_node *node, t_env *env)
{
	pid_t	pid;
	char	*path;

	pid = fork();
	if (pid == ERROR)
		exit_error(data, ERR_FORK, EXIT_FAILURE);
	g_signal[0] = SIG_CHILD;
	if (pid == 0)
	{
		setup_signals_child();
		apply_properties(data, node, CHILD);
		path = get_path(data, node->token->value, env->envp);
		add_var_and_envp(data, ft_strdup("_"), path, ENV);
		execve(path, node->args, env->envp);
		exit_error(data, ERR_EXEC, EXIT_CMD_NOT_EXEC, node->token->value);
		//el hijo retorna su exit_code para el padre
	}
	wait_cmd_background(data, node, pid);//padre checkea
}

void	exec_command(t_shell *data, t_node *node, t_exec *exec, int mode)
{
	if (mode == CHILD)
		execute_cmd_from_child(data, node, exec->env);
	if (mode == FATHER)
		execute_cmd_from_father(data, node, exec->env);
	clean_temp_variables(data, exec->env, data->prompt.tokens, node);
}
