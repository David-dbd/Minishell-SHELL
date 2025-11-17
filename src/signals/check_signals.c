/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 02:47:16 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/17 15:43:32 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * Verifica si se recibieron señales y actúa según el contexto:
 * En modo interactivo actualiza el código de salida y en heredoc
 * termina la ejecución del heredoc con código de Ctrl+C
*/

int	check_signals(t_shell *data, t_redir *redir, char *line)
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
			if (line)
				free(line);
			redir->signal = TRUE;
			data->exit_code = EXIT_CTRL_C;
			g_signal[1] = VOID;
			return (RECIVED_SIGNAL);
		}
	}
	return (VOID);
}

/*
 * Verifica si se recibieron señales y actúa según el contexto:
 * Funcion auxiliar que en el proceso del nodo comprueba la flag de señal
 * evita la ejecucion del nodo por CTRL+C
*/

int	check_signal_node_heredoc(t_node *node)
{
	if (!node)
		return (FALSE);
	if (node->redir)
	{
		if (node->redir->signal == RECIVED_SIGNAL)
		{
			return (TRUE);
		}
	}
	return (FALSE);
}
