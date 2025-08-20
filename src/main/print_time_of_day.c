/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_time_of_day.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 02:12:51 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/08/20 02:13:05 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
    Muestra un mensaje de bienvenida personalizado según la hora del día en que se inicia la sesión.
    Convierte el tiempo de inicio en hora local y selecciona el mensaje adecuado:
    madrugada, mañana, tarde, noche o medianoche.
    El mensaje se imprime en la salida estándar (STDOUT) con formato.
*/

void print_time_of_day(time_t start)
{
    struct tm *local = localtime(&start);
    int hour;
    
    hour = local->tm_hour;
    if (hour < 6)
        ft_printf_fd(STDOUT, MSG_WELCOME_EARLY);
    else if (hour < 12)
        ft_printf_fd(STDOUT, MSG_WELCOME_MORNING);
    else if (hour < 18)
        ft_printf_fd(STDOUT, MSG_WELCOME_AFTER);
    else if (hour < 22)
        ft_printf_fd(STDOUT, MSG_WELCOME_NIGHT);
    else
        ft_printf_fd(STDOUT, MSG_WELCOME_MIDNIGHT);
}
