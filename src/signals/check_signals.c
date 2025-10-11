/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 02:47:16 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/11 06:43:28 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * Verifica si se recibieron señales y actúa según el contexto:
 * En modo interactivo actualiza el código de salida y en heredoc
 * termina la ejecución del heredoc con código de Ctrl+C
 */

int	check_signals(t_shell *data, char *line, int *pipe_fd)
{
	if (g_signal[0] == SIG_INTERACTIVE)
	{
		if (g_signal[1] == SIGINT)
		{
			data->exit_code = EXIT_CTRL_C;
			g_signal[1] = VOID;
			return (RECIVED_SIGNAL);
		}
	}
	else if (g_signal[0] == SIG_HEREDOC)
	{
		if (g_signal[1] == SIGINT)
		{
			if (pipe_fd)
				close(pipe_fd[0]);
			if (line)
				free(line);
			data->exit_code = EXIT_CTRL_C;
			g_signal[1] = VOID;
			return (RECIVED_SIGNAL);
		}
	}
	return (VOID);
}
