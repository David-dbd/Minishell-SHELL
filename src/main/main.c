/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:07:14 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/11 16:30:17 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Aquí inicializamos la estructura principal `data` con las listas y datos
	con los que trabajaremos. También inicializamos la estructura `sa`
	para el manejo de señales con `sigaction`. Luego definimos un bucle
	infinito donde llamamos a `recieve_input` (que usa `readline`).
	Guardamos la entrada en el historial de comandos y empezamos el proceso
	con `steps_manager`.
*/

int	main(int argc, char **argv)
{
	t_shell	data;
	char	*input;

	init_signals();									//inicializamos sa
	init_data(&data, &input);						//inicializamos data
	while (recieve_input(&input, &data) != NULL)	//llamamos a readline
	{
		if (*input)									//por si el usuario presiona solo ENTER "\0"
			add_history(input);
		if (!steps_manager(&data, input))			//Empezamos el proceso. La razon por la que se pasa input es porque no se quiere anadir input a data. Queda feo.
			return (exit_error(&data, NULL, EXIT_FAILURE));
	}
	ft_printf_fd(STDOUT, "Exit\n");
	return (0);
}
