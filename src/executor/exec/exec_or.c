/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_or.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:23:17 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/08 20:20:39 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

void	exec_or(t_shell *data, t_node *node, int mode)
{
	executor_recursive(data, node->left, mode);
	if (data->exit_code != 0 && node->right)
		executor_recursive(data, node->right, mode);
}
