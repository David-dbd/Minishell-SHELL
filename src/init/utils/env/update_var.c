/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 16:32:57 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/01 18:10:03 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"


static char	*if_need_to_add_var(t_shell *dat, int need_to_add, char *key_to_add)
{
	char	*ptr;

	if (need_to_add == TRUE)
	{
		ptr = ft_strdup(key_to_add);
		if (!ptr)
			exit_error(dat, ERR_MALLOC, EXIT_FAILURE);
		return (ptr);
	}
	return (NULL);
}

void	update_var(t_shell *data, char *new_value, char *key_to_find)
{
	t_var	*var;
	char	*ptr;
	int		flag_found;

	flag_found = FALSE;
	var = data->env.vars;
	while (var)
	{
		if (ft_strcmp(var->key, key_to_find) == 0)
		{
			flag_found = TRUE;
			if (var->value)
				free (var->value);
			var->value = ft_strdup(new_value);
			if (!var->value)
				exit_error(data, ERR_MALLOC, EXIT_FAILURE);
			break ;
		}
		var = var->next;
	}
	if (!flag_found)
	{
		ptr = if_need_to_add_var(data, flag_found, key_to_find);
		add_var(data, ptr, new_value, ENV);
	}
}
