/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_arg_types_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 17:09:50 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/21 17:24:02 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

static void	aux_loop(t_node *node, int *tmp_count)
{
	while (node->assig_tmp[*tmp_count])
		(*tmp_count)++;
}

int	*alloc_arg_types(t_shell *d, t_node *node, int start_i, int end_j)
{
	int	*arg_tps;
	int	tmp_count;
	int	tmp_token_index;
	int	len;

	tmp_count = 0;
	if (node->assig_tmp)
	{
		aux_loop(node, tmp_count);
		if (!create_dinamic_arr(d, &arg_tps, start_i, (end_j + tmp_count)))
			return (NULL);
		len = tmp_count;
		tmp_token_index = tmp_count;
		tmp_count = 0;
		while (tmp_count < len)
		{
			arg_tps[tmp_count] = d->prompt.tokens[start_i - tmp_token_index].id;
			tmp_count++;
			tmp_token_index--;
		}
	}
	else
		if (!create_dinamic_arr(d, &arg_tps, start_i, end_j))
			return (NULL);
	return (arg_tps);
}
