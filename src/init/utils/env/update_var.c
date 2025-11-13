/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 16:32:57 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/13 00:44:13 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

void	update_var(t_shell *d, char *new_value, char *key_to_find)
{
	t_var	*var;
	char	*ptr;
	int		flag_found;

	flag_found = FALSE;
	var = d->env.vars;
	while (var)
	{
		if (ft_strcmp(var->key, key_to_find) == 0)
		{
			flag_found = TRUE;
			if (var->value)
				free (var->value);
			var->value = ft_strdup(new_value);
			if (!var->value)
				exit_error(d, ERR_MALLOC, EXIT_FAILURE);
			update_envp(d);
			break ;
		}
		var = var->next;
	}
	if (!flag_found)
		add_var_and_envp_alloc(d, key_to_find, new_value, ENV);
}
