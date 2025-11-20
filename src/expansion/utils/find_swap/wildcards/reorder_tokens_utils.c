/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reorder_tokens_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 23:56:26 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/20 23:50:45 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../../inc/minishell.h"

/*
	Funcion para eliminar los tokens tmp que se han creado hasta el moemento.

	Si i = 5; y falla malloc, entonces se liberan los tmp[5] que si fueron
	allocated para que no hayan leaks
*/

void	free_tmp_tokens(t_token *tmp, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (tmp[i].value)
		{
			free(tmp[i].value);
			tmp[i].value = NULL;
		}
		i++;
	}
	free(tmp);
}

/*
	Funcion para generar hashes validos, ya que al expandir un wildcard hay
	altas posibilidades de que los haya colisión entre los hashes de los tokens
	y los hashes que se le asignan a los nuevos tokens insertados a partir
	del wildcard.

	Ejemplo:

	Tokens: 1 2 3 4 5 6 ->El 3 es el wildcard a expandir

	new_tokens: 1 2 new_token new_token new_token 4 5 6 -> colisión, ya que
	los cada new_sigue el orden, es decir: 1 2 3 4 5 4 5 6. ERRROR
	deberia de ser 1 2 3 4 5 6 7 8
*/

int	create_hash(t_shell *da, int n_dirs, int n_tokens, int starting_hash)
{
	int	i;
	int	collision;
	int	hash_generator;

	hash_generator = starting_hash + da->prompt.n_tokens + n_dirs;
	while (1)
	{
		collision = FALSE;
		i = 0;
		while (i < n_tokens)
		{
			if (da->prompt.tokens[i].hash == hash_generator)
			{
				collision = TRUE;
				hash_generator++;
				break ;
			}
			i++;
		}
		if (collision == FALSE)
			return (hash_generator);
	}
}

void	free_tokens(t_prompt *prompt)
{
	int	i;

	if (!prompt || !prompt->tokens)
		return ;
	i = 0;
	while (i < prompt->n_tokens)
	{
		if (is_alloc_type(prompt->tokens[i].type) && prompt->tokens[i].value)
		{
			free (prompt->tokens[i].value);
			prompt->tokens[i].value = NULL;
		}
		i++;
	}
	free (prompt->tokens);
	prompt->tokens = NULL;
	prompt->n_tokens = 0;
	prompt->n_alloc_tokens = 0;
}
