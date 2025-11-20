/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sequence.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:04:50 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/20 22:27:36 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
 *	Analiza secuencias de comandos separadas por punto y coma (;).
 *	Esta funcion construye la parte superior del Arbol de Sintaxis,
 *	siguiendo la regla de secuenciacion. Itera enlazando comandos
 *	previos con el nuevo nodo de secuenciacion, de izquierda a derecha.
 */

t_node	*parse_sequence(t_shell *data, t_token *tokens, int *i, int n_tokens)
{
	t_node	*central;
	t_node	*left;
	t_node	*right;

	right = NULL;
	left = parse_and_or(data, tokens, i, n_tokens);
	if (!left)
		return (NULL);
	while (*i < n_tokens
		&& (tokens[*i].type == SEMICOLON))
	{
		central = create_node(data, &tokens[*i], tokens[*i].type);
		if (!central)
			return (clean_ast(&left), NULL);
		central->left = left;
		safe_index_plus(i, n_tokens);
		right = parse_and_or(data, tokens, i, n_tokens);
		if (!right)
			return (clean_node(&central), left);
		central->right = right;
		left = central;
	}
	return (left);
}
