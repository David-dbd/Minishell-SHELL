/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_properties.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/11 04:51:08 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/25 16:34:04 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	apply_properties(t_shell *data, t_node *node, t_env *env, int mode)
{
	if (node->assig_tmp)
		my_export(data, data->prompt.tokens, env, node);
	if (node->redir)
		if (!apply_redirs(data, node, mode))
			return (FAILURE);
	return (SUCCESS);
}
