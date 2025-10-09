/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:23:20 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/09 15:53:25 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	Esta unidad implementa la ejecución de un nodo PIPE dentro del AST,
	es decir, dos comandos conectados mediante una tubería (cmd1 | cmd2).

	La función `exec_pipe` crea un pipe con `pipefd` y lanza dos procesos hijo:
	- El hijo izquierdo (cmd1) escribe en el pipe.
	- El hijo derecho (cmd2) lee desde el pipe.

	Cada hijo se configura en `handle_child`, donde se redirigen las entradas
	y salidas estándar según su posición en la tubería:
	- El proceso izquierdo duplica el extremo de escritura del pipe en STDOUT.
	- El proceso derecho duplica el extremo de lectura del pipe en STDIN.

	Después de crear ambos hijos, el proceso padre cierra sus descriptores
	del pipe para evitar fugas y espera la finalización de los hijos con
	`waitpid()`. Si el comando derecho no se ejecuta en segundo plano, su
	código de salida se almacena en `data->exit_code`, y si `exec_pipe` fue
	llamado en modo CHILD, el proceso actual termina con ese mismo estado.

	Este diseño evita forks redundantes dentro de `executor_recursive`,
	mantiene una jerarquía clara de procesos y asegura la propagación
	correcta del código de salida en pipelines anidados o dentro de subshells.
	PS: No se revisa si el comando izquiero se ejecuta en background porque la
	sintaxis no permite que hayan cosas como: "ls & | gre hola". El background
	solo puede estar a la derecha.
*/

void	close_pipes(int *pipefd)
{
	close(pipefd[0]);
	close(pipefd[1]);
}

void	handle_child(t_shell *data, t_node *node, int *pipefd, int side)
{
	if (side == LEFT)
	{
		setup_signals_child();
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		executor_recursive(data, node, CHILD);
	}
	if (side == RIGHT)
	{
		setup_signals_child();
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		executor_recursive(data, node, CHILD);
	}
}

static void	check_mode(t_shell *data, int mode)
{
	//verifica el mode porque puede venir de un subshell
	if (mode == CHILD)
		exit_error(data, NULL, data->exit_code);
}

static int check_if_background(t_shell *dat, t_node *node, pid_t *pid, int mode)
{
	int	status;

	status = 0;
	waitpid(pid[0], &status, 0);
	if (!node->right->background)
	{
		waitpid(pid[1], &status, 0);
		if (WIFEXITED(status))
			dat->exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			dat->exit_code = 128 + WTERMSIG(status);
	}
	check_mode(dat, mode);
	return (dat->exit_code);
}

void	exec_pipe(t_shell *data, t_node *node, int mode)
{
	int		pipefd[2];
	pid_t	pid[2];

	if (pipe(pipefd) == ERROR)
		exit_error(data, ERR_PIPE, EXIT_FAILURE);
	pid[0] = fork();
	if (pid[0] == ERROR)
		return(close_pipes(pipefd), (void)exit_error(data, ERR_FORK, FAIL));
	if (pid[0] == 0)
		handle_child(data, node->left, pipefd, LEFT);
	pid[1] = fork();
	if (pid[1] == ERROR)
		return(close_pipes(pipefd), (void)exit_error(data, ERR_FORK, FAIL));
	if (pid[1] == 0)
		handle_child(data, node->right, pipefd, RIGHT);
	close_pipes(pipefd);
	check_if_background(data, node, pid, mode);
}
