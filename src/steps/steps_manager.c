/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   steps_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 21:46:24 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/10 23:54:58 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Gestiona las funciones/pasos clave del programa.
	Primero tokenizamos, luego hacemos la expansión,
	después creamos el árbol de sintaxis (AST),
	y por último usamos `execute_recursive` para recorrer
	todos los nodos mediante recursión y ejecutar los comandos.
*/

int	steps_manager(t_shell *data, char *input)
{
	if (!tokenizer(data, input))
		return (error(data));
	if (!expanstion(data->prompt.tokens, data->env))	//El expansor lo tendremos que colocar dentro de la ejecucion para que nos cubra todos los casos
		return (error(data));
	if (!ast(data, data->prompt.tokens))
		return (error(data));
	if (!execute_recursive(data, data->ast_root))
		return (error(data));
	clean_data(data);
}
