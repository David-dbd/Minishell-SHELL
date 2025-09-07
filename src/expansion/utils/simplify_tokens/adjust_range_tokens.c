/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adjust_range_tokens.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 19:57:07 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/07 21:54:38 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Encuentra el inicio del rango a unir alrededor de un NO_SPACE.
	- Revisa hacia atrás desde la posición del NO_SPACE.
	- Tiene en cuenta combinaciones de comillas consecutivas
	  y tokens que estén "entre comillas".
*/

int	find_range_start(t_token *tokens, int no_space_position)
{
	int	start;

	start = no_space_position;
	if (start >= 5
		&& is_quote_type(tokens[start - 1].type)
		&& is_quote_type(tokens[start - 2].type)
		&& is_quote_type(tokens[start - 3].type)
		&& is_between_quotes_type(tokens[start - 4].type))
		return (start - 5);
	else if (start >= 4
		&& is_quote_type(tokens[start - 1].type)
		&& is_quote_type(tokens[start - 2].type)
		&& is_quote_type(tokens[start - 3].type)
		&& is_quote_type(tokens[start - 4].type))
		return (start - 4);
	else if (start >= 4
		&& is_quote_type(tokens[start - 1].type)
		&& is_quote_type(tokens[start - 2].type)
		&& tokens[start - 3].type == NO_SPACE
		&& is_between_quotes_type(tokens[start - 4].type))
		return (start - 4);
	else if (start >= 3
		&& is_quote_type(tokens[start - 1].type)
		&& is_between_quotes_type(tokens[start - 2].type))
		return (start - 3);
	else if (start >= 2
		&& is_quote_type(tokens[start - 1].type)
		&& is_quote_type(tokens[start - 2].type))
		return (start - 2);
	else if (start >= 1)
		return (start - 1);
	else
		return (start);
}

/*
	Encuentra el final del rango a unir alrededor de un NO_SPACE.
	- Revisa hacia adelante desde la posición del NO_SPACE.
	- Igual que start, considera comillas y combinaciones especiales.
*/

int	find_range_end(t_token *tokens, int no_space_position)
{
	int	end;

	end = no_space_position;
	if (tokens[end + 5].type
		&& is_quote_type(tokens[end + 1].type)
		&& is_quote_type(tokens[end + 2].type)
		&& is_quote_type(tokens[end + 3].type)
		&& is_between_quotes_type(tokens[end + 4].type))
		return (end + 5);
	else if (tokens[end + 4].type
		&& is_quote_type(tokens[end + 1].type)
		&& is_quote_type(tokens[end + 2].type)
		&& is_quote_type(tokens[end + 3].type)
		&& is_quote_type(tokens[end + 4].type))
		return (end + 4);
	else if (tokens[end + 4].type
		&& is_quote_type(tokens[end + 1].type)
		&& is_quote_type(tokens[end + 2].type)
		&& tokens[end + 3].type == NO_SPACE
		&& is_between_quotes_type(tokens[end + 4].type))
		return (end + 4);
	else if (tokens[end + 3].type
		&& is_quote_type(tokens[end + 1].type)
		&& is_between_quotes_type(tokens[end + 2].type))
		return (end + 3);
	else if (tokens[end + 2].type
		&& is_quote_type(tokens[end + 1].type)
		&& is_quote_type(tokens[end + 2].type))
		return (end + 2);
	else if (end >= 1)
		return (end + 1);
	else
		return (end);
}
