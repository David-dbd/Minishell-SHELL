/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 22:31:21 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/18 03:56:03 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// void	init_minishell(t_shell *data, int ac, char **av, char **envp)
// {
// 	init_signals();
// 	init_data(data, envp);
// 	init_arg(data, ac, av);
// 	print_session_start(data, data->extra_features.session_start,
// 		data->extra_features.user_name);
// }

void	init_minishell(t_shell *data, int ac, char **av, char **envp)
{
	setup_signals_interactive();
	init_data(data, envp);
	init_arg(data, ac, av);
	print_session_start(data, data->extra_features.session_start,
		data->extra_features.user_name);
}
