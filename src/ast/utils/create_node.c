/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:41:39 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/29 18:48:17 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	Al no soportar redirecciones como nodos, sino como porpiedades de estos,
	en casos edge en donde hayan redirecciones huerfanas se crea un nuevo
	nodo de tipo COMMAND de manera artificial y se le asgina el comando "true"
	de modo que que siempre pueda ejecutarse (no hace nada).
*/

static t_node	*allocate_true_node(t_shell *data, t_token **true_token)
{
	t_node	*node;

	*true_token = ft_calloc(1, sizeof(t_token));
	if (!(*true_token))
		return (NULL);
	node = ft_calloc(1, sizeof(t_node));
	if (!node)
	{
		free(*true_token);
		return (NULL);
	}
	(*true_token)->value = ft_strdup("true");
	if (!(*true_token)->value)
	{
		free(*true_token);
		free(node);
		return (NULL);
	}
	return (node);
}

t_node	*create_true_node(t_shell *data, t_type type)
{
	t_node	*node;
	t_token	*true_token;

	node = allocate_true_node(data, &true_token);
	if (!node)
		return (exit_error(data, ERR_MALLOC, EXIT_FAIL), NULL);
	node->type = type;
	node->token = true_token;
	node->args = ft_calloc(2, sizeof(char *));
	if (!node->args)
		return (exit_error(data, ERR_MALLOC, EXIT_FAIL), NULL);
	node->args[0] = "true";
	node->redir = NULL;
	node->assig_tmp = NULL;
	node->arg_types = NULL;
	node->left = NULL;
	node->right = NULL;
	node->executed = FALSE;
	node->fake = TRUE;
	node->background = FALSE;
	return (node);
}

/*
 * Crea un nuevo nodo del AST: inicializa todos los campos
 * incluyendo tipo, token asociado y estructuras vacías
*/

t_node	*create_node(t_shell *data, t_token *token, t_type type)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (exit_error(data, ERR_MALLOC, EXIT_FAIL), NULL);
	node->type = type;
	node->token = token;
	node->args = NULL;
	node->redir = NULL;
	node->arg_types = NULL;
	node->assig_tmp = NULL;
	node->left = NULL;
	node->right = NULL;
	node->executed = FALSE;
	node->fake = FALSE;
	node->background = FALSE;
	return (node);
}
