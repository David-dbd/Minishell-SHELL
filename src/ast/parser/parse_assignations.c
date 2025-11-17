/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_assignations.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 23:12:05 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/17 13:32:06 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	parse_asignations() maneja múltiples asignaciones consecutivas.

	Se crea un nodo SEMICOLON para cada asignación adicional,
	permitiendo que el ejecutor las procese en orden.

	Ejemplos:
	- VAR=1 VAR+=2 → SEMICOLON con dos asignaciones -> VAR=1 ; VAR+=2
	- VAR=1 → un solo nodo de asignación (sin SEMICOLON)

	Otro ejemplo mas complejo:
	- VAR=1 VAR+=2 export A=1 A+=21

	Se crean los nodos de ASIGNATION y PLUS_ASIGNATION, pero al llegar al export
	retornaria el el nodo.

	El parse_cmd creara el nodo de export y se le atribuiran las propiedades
	obtenidas como asignaciones, args, etc... Si sus args son asignaciones
	entonces las omitiremos.

	De este modo podemos distinguir entre ASIGNATIONS que son argumentos de
	COMMANDS o BUILT-IN ó ASIGNATIONS que son independientes.
*/

t_node	*parse_assignations(t_shell *dat, t_token *tokens, int *i, int n_tokens)
{
	t_node	*central;
	t_node	*left;
	t_node	*right;

	if (*i >= n_tokens || !is_real_assignation_type(tokens[*i].type))
		return (NULL);
	left = create_node(dat, &tokens[*i], tokens[*i].type);
	if (!left)
		return (NULL);
	safe_index_plus(i, n_tokens);
	while (*i < n_tokens && is_real_assignation_type(tokens[*i].type))
	{
		central = create_node(dat, NULL, SEMICOLON);
		if (!central)
			return (clean_node(&left), NULL);
		central->left = left;
		right = create_node(dat, &tokens[*i], tokens[*i].type);
		if (!right)
			return (clean_node(&central), clean_ast(&left), NULL);
		safe_index_plus(i, n_tokens);
		central->right = right;
		left = central;
	}
	return (left);
}
