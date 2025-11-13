/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_node_rule.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 20:48:20 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/28 16:03:32 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

void	add_node_rule(t_shell *data, char *value, char *alias, int state)
{
	t_cmd	*new_rule_node;
	t_cmd	*last_rule_node;

	new_rule_node = ft_calloc(1, sizeof(t_cmd));
	if (!new_rule_node)
		return (free(alias), free(value), (void)exit_error(data, ERR_MALLOC, 1));
	new_rule_node->value = value;
	new_rule_node->alias = alias;
	new_rule_node->active = state;
	new_rule_node->next = NULL;
	new_rule_node->prev = NULL;
	last_rule_node = lstlast_var(data->extras.cmd, 'c');
	if (!last_rule_node)
		data->extras.cmd = new_rule_node;
	else
	{
		last_rule_node->next = new_rule_node;
		new_rule_node->prev = last_rule_node;
	}
}
