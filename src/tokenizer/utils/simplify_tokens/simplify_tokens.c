/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simplify_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 20:37:22 by migarrid          #+#    #+#             */
/*   Updated: 2025/11/20 22:19:48 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
 *	Concatena los valores de tokens tipo COMMAND dentro de un rango
 *	y reorganiza los tokens resultando en uno solo.
*/

void	join_tokens(t_shell *d, t_prompt *prompt, t_token *tokens, int *range)
{
	int		i;
	char	*tmp;
	char	*result;

	i = range[0];
	result = ft_strdup("");
	if (!result)
		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
	while (i <= range[1])
	{
		if (is_needed_to_simplify(tokens[i].type))
		{
			tmp = result;
			result = ft_strjoin(result, tokens[i].value);
			free(tmp);
		}
		i++;
	}
	reorganize_tokens(prompt, tokens, range, result);
}

/*
 *	Comprueba si un rango de tokens puede simplificarse:
 *	- Tiene al menos un NO_SPACE que indica unión pegada.
 *	- Si tiene EXPANSION, no deja simplificar los tokens.
*/

static int	is_possible_simplify(t_token *tokens, int *range)
{
	int	i;
	int	has_no_space;

	i = range[0];
	has_no_space = 0;
	while (i <= range[1])
	{
		if (tokens[i].type == NO_SPACE)
			has_no_space = 1;
		if (tokens[i].type == EXPANSION)
			return (0);
		i++;
	}
	return (has_no_space);
}

/*
 *	Busca el siguiente bloque de tokens pegados (NO_SPACE)
 *	y devuelve su rango (start/end).
*/

int	get_no_space_range(t_token *tokens, int *range, int start_i, int n_tokens)
{
	int	i;

	i = start_i;
	while (i < n_tokens && tokens[i].type)
	{
		if (tokens[i].type == NO_SPACE)
		{
			range[0] = find_range_start(tokens, i);
			range[1] = find_range_end(tokens, i, n_tokens);
			return (SUCCESS);
		}
		i++;
	}
	return (FAILURE);
}

/*
 *	Remueve todos los tokens de comillas ya que ya no nos seran utiles
*/

void	remove_quotes_tokens(t_prompt *prompt, t_token *tokens)
{
	int	read_index;
	int	write_index;
	int	quotes_removed;

	read_index = 0;
	write_index = 0;
	quotes_removed = 0;
	while (read_index < prompt->n_tokens && tokens[read_index].type)
	{
		if (is_quote_type(tokens[read_index].type))
			quotes_removed++;
		else
		{
			if (read_index != write_index)
				tokens[write_index] = tokens[read_index];
			write_index++;
		}
		read_index++;
	}
	prompt->n_tokens -= quotes_removed;
	void_tokens_at_the_end(tokens, prompt->n_alloc_tokens, prompt->n_tokens);
}

/*
 *	Recorre todos los tokens y une los que están pegados (NO_SPACE)
 *	para simplificar la lista de tokens finales.
*/
void	simplify_tokens(t_shell *data, t_prompt *prompt, t_token *tokens)
{
	int	i;
	int	range[2];

	i = 0;
	while (i < prompt->n_tokens && tokens[i].type)
	{
		if (get_no_space_range(tokens, range, i, prompt->n_tokens))
		{
			if (is_possible_simplify(tokens, range))
			{
				if (no_space_at_end(data, prompt, tokens)
					|| no_space_at_delimiter(data, prompt, tokens))
					return ;
				join_tokens(data, prompt, tokens, range);
				i = range[0] + 1;
				continue ;
			}
		}
		i++;
	}
	remove_quotes_tokens(prompt, tokens);
	adjust_id(tokens, prompt->n_tokens);
}
