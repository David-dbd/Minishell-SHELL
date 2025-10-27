/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_properties.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:02:23 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/27 14:11:05 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	apply_properties(t_shell *data, t_node *node, t_env *env, int mode)
{
	if (node->assig_tmp)
	{
		// printf("IT HAS ASSIG_TEMP\n\n");
		apply_temp_asig(data, data->prompt.tokens, node, env);
	}
	if (node->redir)
		if (!apply_redirs(data, node, mode))
		{
			data->exit_code = FAIL;
			return (FAILURE);
		}
	return (SUCCESS);
}
