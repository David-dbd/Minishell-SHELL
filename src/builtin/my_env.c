/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 01:09:26 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/12 23:54:21 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Imprime todas las variables de entorno (type == ENV)
	en el formato `clave=valor`. Devuelve 0 al finalizar.
*/

int	my_env(t_var *vars)
{
	t_var	*var;

	var = vars;
	while (var)
	{
		if (var->type == ENV)
			printf("%s=%s\n", var->key, var->value);
		var = var->next;
	}
	return (0);
}
