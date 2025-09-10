/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 01:09:26 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/10 20:36:04 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	my_env(t_var *vars)
{
	t_var	*var;

	var = vars;
	while (var)
	{
		if (var->type == ENV)
			printf("%s=%s\n", var->key, var->value);
		var = var->next;
	}
}
