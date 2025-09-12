/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_session_end.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 02:11:39 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/09/12 17:32:27 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Calcula y muestra la duración de la sesión del usuario en el minishell.
	Utiliza la hora actual y la hora de inicio registrada para obtener los
	minutos y segundos transcurridos. Imprime el resultado en un mensaje
	formateado y con color en la salida estándar (STDOUT).
*/

void	print_session_end(time_t start, char *user_name)
{
	int		minutes;
	int		seconds;
	time_t	end;
	double	duration;

	end = time(NULL);
	duration = difftime(end, start);
	minutes = (int)(duration / 60);
	seconds = (int)(duration) % 60;
	if (user_name)
		ft_printf_fd(STDOUT, MSG_TIME_END_V1, user_name, minutes, seconds);
	ft_printf_fd(STDOUT, MSG_TIME_END_V2, minutes, seconds);
}
