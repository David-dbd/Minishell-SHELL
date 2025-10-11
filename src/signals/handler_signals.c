/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler_signals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 02:47:09 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/11 06:46:18 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// void	handle_sigint_interative(int sig)
// {
// 	(void)sig;
// 	g_signal[1] = SIGINT;
// 	rl_on_new_line();
// 	rl_redisplay();
// 	ft_putstr_fd("\n", 1);
// 	rl_replace_line("", 0);
// 	rl_on_new_line();
// 	rl_redisplay();
// }

/*
 * Maneja Ctrl+C en modo interactivo: marca la señal
 * recibida para ser procesada posteriormente
 */

void	handle_sigint_interative(int sig)
{
	(void)sig;
	g_signal[1] = SIGINT;
}

/*
 * Maneja Ctrl+C en heredoc: marca la señal y escribe
 * un salto de línea para limpiar la entrada
 */

void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	g_signal[1] = SIGINT;
}
