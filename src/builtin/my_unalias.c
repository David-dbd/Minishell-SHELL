/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_unalias.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 20:48:09 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/30 02:05:22 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Se compara el argumento con el cmd->value (cd, echo, etc...) y si hay match
	se hace free.

	Si el usuario hace unalias de un alias, en lugar de encontarlo al comparar
	con cmd->value, se compara con cmd->alias.

	Eg.
	unalias cd -> Se encuetra cd en la lista enlazada y se liberasu alias "hola"
	unalias hola -> Se encuentra hola en la lista enlazada y se libera.
*/

static void	aux_delete(t_cmd **node)
{
	if ((*node)->value)
	{
		free((*node)->value);
		(*node)->value = NULL;
	}
	if ((*node)->alias)
	{
		free((*node)->alias);
		(*node)->alias = NULL;
	}
	free(*node);
	*node = NULL;
}

static void	delete_alias_node(t_cmd *cmd, char *value)
{
	t_cmd	*node;
	t_cmd	*next;

	node = cmd;
	while (node)
	{
		if (node->value && ft_strcmp(node->value, value) == 0)
		{
			if (!node->prev)
				cmd = node->next;
			else
				node->prev->next = node->next;
			if (node->next)
				node->next->prev = node->prev;
			next = node->next;
			aux_delete(&node);
			node = next;
			continue ;
		}
		node = node->next;
	}
}

static void	find_built_in(t_cmd *cmd, char *arg)
{
	while (cmd)
	{
		if (cmd->alias && (ft_strcmp(cmd->alias, arg) == 0 //Tanto si se hace unalias "cd" como unalias "hola"
			|| ft_strcmp(cmd->value, arg) == 0))
		{
			if (cmd->active != INDIFERENT) //Si el nodo no solo existe porque tiene un alias sino también por active, solo se libera el alias.
			{
				free (cmd->alias);
				cmd->alias = NULL;
			}
			else //si el nodo solo existe por alias, entonces se tiene que eliminar el nodo completo ya que no hay otra razón para que este vivo.
				delete_alias_node(cmd, cmd->value);
		}
		cmd = cmd->next;
	}
}

int	my_unalias(t_cmd *cmd, char **args)
{
	int	i;
	int	exit_flag;

	i = 0;
	exit_flag = 0;
	if (!args || !args[0])
		return (0);
	while (args[i])
	{
		if (!check_arg_syntax(args[i], ERR_BUILTIN_UNALIAS))
		{
			exit_flag = EXIT_FAIL;
			i++;
			continue ;
		}
		find_built_in(cmd, args[i]);
		i++;
	}
	return (exit_flag);
}
