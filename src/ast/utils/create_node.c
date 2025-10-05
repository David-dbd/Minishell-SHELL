/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:41:39 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/05 20:41:13 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

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
	node->left = NULL;
	node->right = NULL;
	node->pid = 0;
	node->exit_code = 0;
	node->executed = FALSE;
	return (node);
}
