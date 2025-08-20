/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_session_start.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 02:13:28 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/08/20 02:13:44 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
    Imprime el título del minishell y muestra la hora exacta en que se inicia la sesión.
    La hora se formatea en color cian e incluye hora, minutos, segundos, día, mes y año.
    Luego llama a la función print_time_of_day para mostrar un mensaje personalizado
    según el momento del día en que se inicia la sesión.
*/

void print_session_start(time_t start)
{
    struct tm *local = localtime(&start);

    ft_printf_fd(STDOUT, MSG_TITLE);
    printf("\033[0;36mSession started at: %02d:%02d:%02d on %02d/%02d/%04d\033[0m\n\n",
           local->tm_hour, local->tm_min, local->tm_sec,
           local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
    print_time_of_day(start);
}