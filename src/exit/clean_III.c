/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_III.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 21:43:11 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/13 00:41:39 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Elimina las variables `TEMP_ASIGNATION` del ciclo. Elimina solo los que se
	ejecuten/afecten en el shell padre/global, ya que los que se ejecuten
	en child procesess se borraran solos.
*/

void	clean_temp_variables(t_shell *d, t_env *e, t_token *t, t_node *node)
{
	my_clean_unset(d, e, t, node->arg_types);
}

/*
	Restea toda la memorua de `t_extras` y la deja apuntando
	a NULL para evitar accesos inválidos.
*/

void	clean_extras(t_extras *extras)
{
	if (extras->user_name)
	{
		free(extras->user_name);
		extras->user_name = NULL;
	}
	*extras = (t_extras){0};
}

/*
	Cierra los fd originales al finalizar el programa
	asi cumplimos con la limpieza para el sistema.
*/

void	clean_fd(t_exec *exec)
{
	close(exec->original_stdin);
	close(exec->original_stdout);
}

void	clean_builtins_selection(t_shell *data)
{
	ft_free_str_array(data->builtins->builtins_selection);
	free (data->builtins);
	data->builtins = NULL;
}
