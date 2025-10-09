/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:36:57 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/09 03:24:52 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	executor_recursive(t_shell *data, t_node *node, int mode)
{
	if (!node || node->executed)
		return ;

	node->executed = true;

	if (node->type == SEMICOLON)
		exec_semicolon(data, node, mode);
	else if (node->type == AND)
		exec_and(data, node, mode);
	else if (node->type == OR)
		exec_or(data, node, mode);
	else if (node->type == PIPE)
		exec_pipe(data, node, mode);
	else if (node->type == SUBSHELL)
		exec_subshell(data, node, mode);
	else if (node->type == BUILT_IN || is_asignation_type(node->token->type))
		exec_builtin(data, node, mode);
	else if (node->type == COMMAND)
		exec_command(data, node, &data->env, mode);
}
