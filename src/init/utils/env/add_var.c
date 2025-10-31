/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 01:05:00 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/31 16:38:00 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Devuelve el último nodo de la lista de variables o NULL si está vacía.
*/

void	*lstlast_var(void *data, char type)
{
	t_var	*lst;
	t_cmd	*cmd_lst;

	if (!data)
		return (NULL);
	if (type == 'v')//t_var
	{
		lst = (t_var *)data;
		while (lst->next != NULL)
			lst = lst->next;
		return (void *)lst;
	}
	else if (type == 'c')//t_cmd
	{
		cmd_lst = (t_cmd *)data;
		while (cmd_lst->next != NULL)
			cmd_lst = cmd_lst->next;
		return (void *)cmd_lst;
	}
	return (NULL);
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
	new_var->prev = NULL;
	last_var = lstlast_var(data->env.vars, 'v');
	if (!last_var)
		data->env.vars = new_var;
	else
	{
		last_var->next = new_var;
		new_var->prev = last_var;
	}
	data->env.size++;
}

/*
	Crea una nueva variable de entorno con 'key' y 'value' y la añade
	al final de la lista de variables del shell. Ademas reconstruye el
	char **envp necesario para execve
*/

void	add_var_and_envp(t_shell *data, char *key, char *value, int type)
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
	new_var->prev = NULL;
	last_var = lstlast_var(data->env.vars, 'v');
	if (!last_var)
		data->env.vars = new_var;
	else
	{
		last_var->next = new_var;
		new_var->prev = last_var;
	}
	data->env.size++;
	update_envp(data);
}
