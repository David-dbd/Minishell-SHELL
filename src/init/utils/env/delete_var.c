/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 18:22:00 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/27 18:25:11 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"


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

void	delete_var(t_env *env, char *key)
{
	t_var	*node;
	t_var	*next;

	node = env->vars;
	while (node)
	{
		if (node->key && ft_strcmp(node->key, key) == 0)
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
