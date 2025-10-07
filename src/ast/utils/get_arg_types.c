/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_arg_types.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 01:44:27 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/07 18:18:01 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	get_arg_types() sirve para contextualizar los args de un cmd o built_in.
	En este caso en concreto solo se necesita contextualizar export debido a
	los diferentes tipos de argumentos que puede manejar.

	Eg.
	export [args: var=1, var+=1, hola] -> [context: ASIGNATION, PLUS_ASIG, WORD]

	Esto evita tener que reanalizar los args de export, es decir, duplicar el
	trabajo que ya se hizo en otras funciones que verifican los tokens.
*/

static void	create_dinamic_arr(t_shell *data, int **arg_types, int i, int j)
{
	*arg_types = ft_calloc((j - i) - 1, sizeof(int));
	if (!*arg_types)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
}

int	*get_arg_types(t_shell *data, t_token *tokens, int start_i, int end_j)
{
	int		*arg_types;
	int		arg_index;

	arg_index = 0;
	if (ft_strcmp(tokens[start_i].value, BUILTIN_EXPORT) != 0)
		return (NULL);
	create_dinamic_arr(data, &arg_types, start_i, end_j);
	start_i += 1;
	while (start_i < data->prompt.n_tokens && start_i < end_j)
	{
		arg_types[arg_index] = tokens[start_i].id;
		start_i++;
		arg_index++;
	}
	return (arg_types);
}
