/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 22:31:21 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/25 18:10:06 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
 * Inicializa minishell: configura señales interactivas,
 * variables de entorno y procesa el primer argumento como un script de bash
 */

void	init_minishell(t_shell *data, int ac, char **av, char **envp)
{
	setup_signals_interactive();
	init_data(data, envp);
	init_arg(data, ac, av);
}
