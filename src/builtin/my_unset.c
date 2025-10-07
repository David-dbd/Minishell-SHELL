/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 21:56:06 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/07 15:45:08 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * my_unset - Elimina variables de entorno según los tokens proporcionados.
 *
 * @data: Puntero a la estructura principal del shell.
 * @env: Puntero a la estructura que contiene las variables de entorno.
 * @tokens: Array de tokens que pueden representar claves a eliminar.
 *
 * Esta función recorre los tokens recibidos en el prompt y, por cada uno que
 * sea de tipo WORD o ASIGNATION, busca en la lista doblemente enlazada de
 * variables de entorno. Si encuentra una coincidencia con la clave del token,
 * elimina el nodo correspondiente, actualizando los punteros prev y next
 * para mantener la integridad de la lista.
 *
 * Si el nodo a eliminar es el primero, actualiza env->vars.
 * Si es el único nodo, también actualiza env->vars y termina la función.
 * La memoria del nodo se libera mediante aux_delete().
*/

static void	aux_delete(t_var **node, t_env *env)
{
	if ((*node)->key)
	{
		free((*node)->key);
		(*node)->key = NULL;
	}
	if ((*node)->value)
	{
		free((*node)->value);
		(*node)->value = NULL;
	}
	free(*node);
	*node = NULL;
	env->size--;
}

static void	delete_env_var(t_env *env, char *key)
{
	t_var	*node;
	t_var	*next;

	node = env->vars;
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
		{
			if (!node->prev)
				env->vars = node->next;
			else
				node->prev->next = node->next;
			if (node->next)
				node->next->prev = node->prev;
			next = node->next;
			aux_delete(&node, env);
			node = next;
			continue ;
		}
		node = node->next;
	}
}
/*
int	my_unset(t_shell *data, t_env *env, t_token *tokens)
{
	int	i;

	i = 0;
	while (i < data->prompt.n_tokens)
	{
		if (tokens[i].type == WORD || tokens[i].type == ASIGNATION)
			delete_env_var(env, tokens[i].value);
		i++;
	}
	return (0);
}*/

int	my_unset(t_shell *data, t_env *env, char **args)
{
	int	i;

	i = 0;
	while (args[i] != NULL)
	{
		delete_env_var(env, args[i]);
			i++;
	}
	return (0);
}
