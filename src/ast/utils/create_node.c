/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:41:39 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/17 14:15:47 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	Al no soportar redirecciones como nodos, sino como porpiedades de estos,
	en casos edge en donde hayan redirecciones huerfanas se crea un nuevo
	nodo de tipo COMMAND de manera artificial y se le asgina el comando "true"
	de modo que que siempre pueda ejecutarse (no hace nada).
*/

static t_node	*allocate_true_node(t_shell *data)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (error_status_node(data, NULL, ERR_MALLOC));
	node->fake = TRUE;
	node->token = ft_calloc(1, sizeof(t_token));
	if (!node->token)
		return (error_status_node(data, &node, ERR_MALLOC));
	node->token->value = ft_strdup("true");
	if (!node->token->value)
		return (error_status_node(data, &node, ERR_MALLOC));
	return (node);
}

t_node	*create_true_node(t_shell *data, t_type type)
{
	t_node	*node;

	node = allocate_true_node(data);
	if (!node)
		return (NULL);
	node->fake = TRUE;
	node->type = type;
	node->token_hash = data->prompt.n_tokens + 1;
	node->args = ft_calloc(2, sizeof(char *));
	if (!node->args)
		return (error_status_node(data, &node, ERR_MALLOC));
	node->args[0] = ft_strdup("true");
	if (!node->args[0])
		return (error_status_node(data, &node, ERR_MALLOC));
	node->redir = NULL;
	node->assig_tmp = NULL;
	node->arg_types = NULL;
	node->left = NULL;
	node->right = NULL;
	node->executed = FALSE;
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
		return (error_status_node(data, &node, ERR_MALLOC));
	node->type = type;
	if (node->type == WILDCARD)
		node->type = COMMAND;
	node->token = token;
	node->token_hash = token->hash;
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
