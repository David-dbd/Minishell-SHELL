/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 23:17:43 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/25 17:52:46 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
 * Inicializa la estructura principal del shell: establece valores
 * por defecto, configura el entorno, imprime la sesión y prepara
 * readline para la entrada interactiva
 */

void	init_data(t_shell *data, char **envp)
{
	*data = (t_shell){0};
	data->shell_pid = getpid();
	data->extra_features.session_start = time(NULL);
	// print_session_start(data, data->extra_features.session_start,
	// 	data->extra_features.user_name);
	init_ic_readline();
	init_enviroment(data, envp);
}
