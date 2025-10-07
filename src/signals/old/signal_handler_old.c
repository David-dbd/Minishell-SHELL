/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler_old.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:45:52 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/07 18:11:20 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../../inc/minishell.h"

/*
	Define el comportamiento cuando se recibe una señal SIGINT o SIGQUIT.
	Usa funciones de la biblioteca GNU Readline para gestionar la línea actual.
	En SIGINT: limpia la línea y marca la variable global como CLEAN.
	En SIGQUIT: marca la variable global como CONTINUE.
	En distintos puntos del programa se revisa esta variable para actuar en
	consecuencia.
*/

// void	signal_handler(int sig, siginfo_t *info, void *context)
// {
// 	if (sig == SIGINT)
// 	{
// 		rl_on_new_line();
// 		rl_replace_line("", 0);
// 		rl_redisplay();
// 		g_signal_event = CLEAN;
// 	}
// 	else if (sig == SIGQUIT)
// 	{
// 		g_signal_event = CONTINUE;
// 	}
// 	return ;
// }

// void	signal_handler(int sig, siginfo_t *info, void *context)
// {
// 	if (sig == SIGINT)
// 	{
// 		ic_print("\n");
// 		g_signal_event = CLEAN;
// 	}
// 	else if (sig == SIGQUIT)
// 	{
// 		g_signal_event = CONTINUE;
// 	}
// 	return ;
// }
