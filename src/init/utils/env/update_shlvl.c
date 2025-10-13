/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_shlvl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 21:47:52 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/13 19:56:06 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
 * Incrementa el nivel de shell (SHLVL): suma 1 al valor actual,
 * maneja casos límite como valores demasiado altos o negativos
 */

void	update_shlvl(t_var *vars)
{
	t_var			*var;
	long			lvl;

	var = vars;
	while (var)
	{
		if (ft_strcmp(var->key, "SHLVL") == 0)
		{
			lvl = ft_atol(var->value) + 1;
			if (lvl > INT_MAX)
			{
				ft_printf_fd(STDERR, ERR_SHLVL_HIGH);
				lvl = 1;
			}
			else if (lvl < 0)
			{
				ft_printf_fd(STDERR, ERR_SHLVL_LOW);
				lvl = 1;
			}
			free(var->value);
			var->value = ft_itoa(lvl);
			return ;
		}
		var = var->next;
	}
}
