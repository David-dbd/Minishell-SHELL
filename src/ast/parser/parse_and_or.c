/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_and_or.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:20:29 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/20 22:32:56 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
 *	Analiza la secuencia de comandos conectados por lógica (&& o ||).
 *	Esta funcion tiene una precedencia mayor que la tubería. Toma
 *	comandos o pipes como hijos (`parse_pipes`) y los une creando
 *	nodos `AND` u `OR`. Esto construye la estructura de control
 *	del flujo de ejecucion, de izquierda a derecha.
 */

t_node	*parse_and_or(t_shell *data, t_token *tokens, int *i, int n_tokens)
{
	t_node	*central;
	t_node	*left;
	t_node	*right;

	right = NULL;
	left = parse_pipes(data, tokens, i, n_tokens);
	if (!left)
		return (NULL);
	while (*i < n_tokens && (tokens[*i].type == AND || tokens[*i].type == OR))
	{
		central = create_node(data, &tokens[*i], tokens[*i].type);
		if (!central)
			return (clean_node(&left), NULL);
		safe_index_plus(i, n_tokens);
		right = parse_pipes(data, tokens, i, n_tokens);
		if (!right)
			return (clean_node(&central), clean_ast(&left), NULL);
		central->left = left;
		central->right = right;
		left = central;
	}
	return (left);
}
