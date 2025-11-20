/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 21:27:37 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/20 22:16:50 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
 *	Inicializa la estructura de ejecución `t_exec`.
 *	Guarda descriptores de archivo de STDIN y STDOUT originales
 *	mediante `dup` para poder restaurar la entrada/salida estándar
 *	después de ejecutar comandos y manejar redirecciones en el padre.
 */

void	init_exec(t_exec *exec, t_env *env)
{
	exec->original_stdin = dup(STDIN_FILENO);
	exec->original_stdout = dup (STDOUT_FILENO);
	exec->env = env;
}
