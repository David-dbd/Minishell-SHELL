/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 17:07:14 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/09 16:19:50 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include ".h"

/*Here we are initializing the struct data that contains the main structs/lists
that we'll work on. We also set or inialize the struc sa that comes with the use
os sigaction. We are defining an infinite loop in which we call recieve_input,
which contains the readline();. Then we add the input to our command log history
and begin the process with steps_manager*/

int	main(int argc, char **argv)
{
	t_shell	data;
	char	*input;

	init_signals(); //inicializamos sa
	init_data(&data, &input); //inicializamos data
	while (recieve_input(&input) != NULL) //llamamos a readline
	{
		if (*input) //por si el usuario presiona solo ENTER "\0"
			add_history(input);
		if (!steps_manager(&data, input)) //Empezamos el proceso. La razon por la que se pasa input es porque no se quiere anadir input a data. Queda feo.
			return (exit_error(&data));
	}
	ft_printf("Exit\n");
	return (0);
}