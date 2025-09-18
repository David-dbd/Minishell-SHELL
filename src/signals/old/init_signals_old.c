/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_signals_old.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:37:17 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/18 02:41:32 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

volatile sig_atomic_t	g_signal_event;

/*
	Inicializa la estructura `sigaction` en cero para limpiar datos residuales.
	Configura `sa_sigaction` con la funcion `signal_handler`.
	Usa `SA_RESTART` para reanudar syscalls interrumpidas por señales y
	`SA_SIGINFO` para pasar info extra al manejador.
	Registra SIGINT y SIGQUIT con `sigaction`.
	Si falla, muestra error y termina el programa.
*/

// int	init_signals(void)
// {
// 	struct sigaction	sa;

// 	sa = (struct sigaction){0};
// 	sa.sa_sigaction = signal_handler;
// 	sa.sa_flags = SA_RESTART | SA_SIGINFO;
// 	if (sigaction(SIGINT, &sa, NULL) == ERROR)
// 		exit_error(NULL, ERR_SIGNAL, EXIT_FAILURE);
// 	if (sigaction(SIGQUIT, &sa, NULL) == ERROR)
// 		exit_error(NULL, ERR_SIGNAL, EXIT_FAILURE);
// 	return (0);
// }
