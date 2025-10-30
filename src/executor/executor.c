/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:36:57 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/30 01:54:21 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	executor_recursive(t_shell *data, t_node *node, t_exec *exec, int mode)
{
	if (!node || node->executed)
		return ;
	node->executed = true;

	if (node->type == SEMICOLON)
		exec_semicolon(data, node, exec, mode);
	else if (node->type == AND)
		exec_and(data, node, exec, mode);
	else if (node->type == OR)
		exec_or(data, node, exec, mode);
	else if (node->type == PIPE)
		exec_pipe(data, node, exec, mode);
	else if (node->type == SUBSHELL)
		exec_subshell(data, node, exec, mode);
	else if (node->type == BUILT_IN || is_asignation_type(node->token->type))
		exec_builtin(data, node, mode);
	else if (node->type == COMMAND)
		exec_command(data, node, exec, mode);
}
