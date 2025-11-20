/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 10:55:44 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/20 22:15:05 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
 *	Verifica si hubo un error de asignación de memoria (NULL)
 *	en el array de strings 'builtins'.
 *	Si encuentra un puntero nulo, libera toda la memoria del array
 *	antes de retornar ERROR, previniendo fugas.
 */

static int	check_if_error(char **builtins, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (!builtins[i])
		{
			ft_free_str_array(&builtins);
			return (ERROR);
		}
		i++;
	}
	return (SUCCESS);
}

/*
 *	Inicializa la lista de comandos internos (builtins) de la shell.
 *	Asigna memoria para el array, duplica el nombre de cada builtin
 *	(cd, echo, export, etc.) y verifica que todas las asignaciones
 *	de memoria hayan sido exitosas antes de almacenarlas en `data`.
 */

void	init_builtins(t_shell *data)
{
	char	**built_ins;
	int		index;
	int		len;

	len = N_BULTINS;
	built_ins = ft_calloc(len + 1, sizeof(char *));
	if (!built_ins)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	built_ins[0] = ft_strdup("cd");
	built_ins[1] = ft_strdup("echo");
	built_ins[2] = ft_strdup("export");
	built_ins[3] = ft_strdup("unset");
	built_ins[4] = ft_strdup("env");
	built_ins[5] = ft_strdup("exit");
	built_ins[6] = ft_strdup("pwd");
	built_ins[7] = NULL;
	if (check_if_error(built_ins, len) == ERROR)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	data->builtins.builtins_selection = built_ins;
	data->builtins.n_builtins = len;
}
