/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 01:05:00 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/07 20:52:52 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	Devuelve el último nodo de la lista de variables o NULL si está vacía.
*/

t_var	*lstlast_var(t_var *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

/*
	Crea una nueva variable de entorno con 'key' y 'value' y la añade
	al final de la lista de variables del shell.
*/

void	add_var(t_shell *data, char *key, char *value, int type)
{
	t_var	*new_var;
	t_var	*last_var;

	new_var = ft_calloc(1, sizeof(t_var));
	if (!new_var)
		return (free(key), free(value), (void)exit_error(data, ERR_MALLOC, 1));
	new_var->key = key;
	new_var->value = value;
	new_var->type = type;
	new_var->next = NULL;
	last_var = lstlast_var(data->env.vars);
	if (!last_var)
		data->env.vars = new_var;
	else
		last_var->next = new_var;
	data->env.size++;
}
