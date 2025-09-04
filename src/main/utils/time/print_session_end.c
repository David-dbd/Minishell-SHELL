/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_session_end.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 02:11:39 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/09/03 22:04:32 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Calcula y muestra la duración de la sesión del usuario en el minishell.
	Utiliza la hora actual y la hora de inicio registrada para obtener los
	minutos y segundos transcurridos. Imprime el resultado en un mensaje
	formateado y con color en la salida estándar (STDOUT).
*/

void	print_session_end(time_t start)
{
	int		minutes;
	int		seconds;
	time_t	end;
	double	duration;

	end = time(NULL);
	duration = difftime(end, start);
	minutes = (int)(duration / 60);
	seconds = (int)(duration) % 60;
	ft_printf_fd(STDOUT, MSG_TIME_END, minutes, seconds);
}
