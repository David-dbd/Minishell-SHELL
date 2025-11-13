/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_arg.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 22:46:42 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/13 00:44:18 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
 * Maneja errores de apertura de archivo:
 * verifica si el archivo no existe o no tiene permisos.
 */

static void	open_error(t_shell *data, char *file)
{
	if (errno == ENOENT)
		exit_error(data, ERR_NO_INPUT_FILE, EXIT_USE);
	else if (errno == EACCES)
		exit_error(data, ERR_PERM_DENIED, EXIT_CMD_NOT_EXEC, file);
	exit_error(data, ERR_NO_INPUT_FILE, EXIT_USE);
}

/*
 * Procesa argumentos como script: abre el archivo, lee línea por línea
 * ejecutando cada comando y termina el programa al finalizar el script
*/

void	init_arg(t_shell *data, int ac, char **av)
{
	int		fd;

	if (ac >= 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd < 0)
			open_error(data, av[1]);
		data->prompt.input = get_next_line(fd);
		while (data->prompt.input)
		{
			if (!tokenizer(data, &data->prompt, data->prompt.input))
				exit_error(data, NULL, EXIT_USE);
			ast_builder(data, data->prompt.tokens, data->prompt.n_tokens);
			executor_recursive(data, data->ast_root, &data->exec, FATHER);
			clean_cycle(data, &data->prompt, &data->ast_root);
			data->prompt.input = get_next_line(fd);
		}
		get_next_line(RESET);
		exit_succes(data, NULL, data->exit_code);
	}
}
