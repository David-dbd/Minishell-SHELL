/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_session_start.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 02:13:28 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/09/03 22:04:35 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Imprime el título del minishell y muestra la hora exacta en que se inicia la
	sesión. La hora se formatea en color cian e incluye hora, minutos, segundos,
	día, mes y año.Luego llama a la función print_time_of_day para mostrar un
	mensaje personalizado según el momento del día en que se inicia la sesión.
*/
void	print_minishell_title(void)
{
	printf(TITLE_COLOR "\n\n");
	printf(T1 "\n");
	printf(T2 "\n");
	printf(T3 "\n");
	printf(T4 "\n");
	printf(T5 "\n");
	printf("\n\n" RESET_COLOR);
}

void	print_session_start(time_t start)
{
	struct tm	*local;

	local = localtime(&start);
	print_minishell_title();
	print_time_of_day(start);
	printf(MSG_TIME_START,
		local->tm_hour, local->tm_min, local->tm_sec,
		local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
}
