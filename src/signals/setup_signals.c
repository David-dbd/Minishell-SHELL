/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_signals.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 02:02:04 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/06 17:16:06 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

volatile sig_atomic_t	g_signal[2];

/*
 * Configura señales para modo interactivo: maneja Ctrl+C
 * personalizado e ignora Ctrl+\\ para mantener el prompt activo
 */

void	setup_signals_interactive(void)
{
	g_signal[0] = SIG_INTERACTIVE;
	signal(SIGINT, handle_sigint_interative);
	signal(SIGQUIT, SIG_IGN);
}

/*
 * Establece señales por defecto en hijos: permiten que los
 * procesos terminen normalmente con Ctrl+C y Ctrl+\\
 */

void	setup_signals_child(void)
{
	g_signal[0] = SIG_CHILD;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/*
 * Prepara señales para heredoc: manejo especial que interrumpe
 * la lectura sin salir del shell cuando se presiona Ctrl+C
 */
void	setup_signals_heredoc(void)
{
	g_signal[0] = SIG_HEREDOC;
	signal(SIGINT, handle_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
}
