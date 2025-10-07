/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:41:39 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/07 20:52:36 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
 * Crea un nuevo nodo del AST: inicializa todos los campos
 * incluyendo tipo, token asociado y estructuras vacías
*/

t_node	*create_true_node(t_shell *data, t_type type)
{
	t_node	*node;
	t_token	*true_token;

	true_token = ft_calloc(1, sizeof(t_token));
	if (!true_token)
		return (exit_error(data, ERR_MALLOC, EXIT_FAIL), NULL);
	node = ft_calloc(1, sizeof(t_node));
	if (!node)
	{
		free (true_token);
		return (exit_error(data, ERR_MALLOC, EXIT_FAIL), NULL);
	}
	true_token->value = ft_strdup("true");
	if (!true_token->value)
	{
		free (true_token);
		free (node);
		return (exit_error(data, ERR_MALLOC, EXIT_FAIL), NULL);
	}
	node->type = type;
	node->token = true_token;
	node->args = NULL;
	node->redir = NULL;
	node->arg_types = NULL;
	node->left = NULL;
	node->right = NULL;
	node->pid = 0;
	node->exit_code = 0;
	node->executed = FALSE;
	node->fake = TRUE;
	node->background = FALSE;
	return (node);
}

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
	node->left = NULL;
	node->right = NULL;
	node->pid = 0;
	node->exit_code = 0;
	node->executed = FALSE;
	node->fake = FALSE;
	node->background = FALSE;
	return (node);
}
