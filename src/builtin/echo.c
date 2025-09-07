/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 01:23:24 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/07 20:49:21 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Procesa los argumentos de 'echo' y detecta todas las opciones '-n'
	consecutivas o -nnnnn..., para evitar imprimir la nueva línea final y
	avanzar al primer argumento real.
*/

static void	handle_n_flag(char **args, int *new_line, int *i)
{
	int	j;

	j = 0;
	while (args[*i] && args[*i][0] == '-' && args[*i][1] == 'n')
	{
		j += 2;
		while (args[*i][j] == 'n')
			j++;
		if (args[*i][j] == '\0')
		{
			*new_line = FALSE;
			(*i)++;
		}
		else
			break ;
	}
}

/*
	Implementa el comando 'echo': imprime los argumentos y añade
	una nueva línea a menos que se use '-n'.
*/

void	shell_echo(char **args)
{
	int	i;
	int	new_line;

	i = 1;
	new_line = TRUE;
	handle_n_flag(args[i], &new_line, &i);
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (new_line)
		printf("\n");
}
