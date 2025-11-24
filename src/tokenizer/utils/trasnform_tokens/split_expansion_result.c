/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_expansion_result.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 23:19:18 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/24 21:19:15 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	En esta función se hace lo mismo que se hace en expansion_final_process.
	Los tokens se comparan con before_tokens despues de la expansión.
	De este modo, identificamos los tokens que se expandieron y que cumplen con
	las condiciones necesarias para hacer word_spliting.

	Se usa reorder_tokens para insertar esos strs que resultan del split, de
	modo que podemos reemplazar el token actual por los tokens nuevos generados
	a partir de los strs.

	Ejemplo:

	USER="que hay de nuevo viejo"
	echo "hola $USER como estas"
	"hola que hay de nuevo viejo como estas" ->resultado de la expansión
	{"hola", "que". "hay", "de", "nuevo" ...} -> de str se pasan a tokens
*/

static void	update_info(t_prompt *prompt, char **arr, int count, int *i)
{
	adjust_id(prompt->tokens, prompt->n_tokens);
	*i += count;
	ft_free_str_array(&arr);
}

static int	if_was_expansion(t_token *tokens, int i)
{
	if (tokens[i].type == WORD
		&& !tokens[i].double_quoted && !tokens[i].single_quoted
		&& (ft_strchr(tokens[i].value, ' ')
			|| ft_strchr(tokens[i].value, '\t')))
		return (TRUE);
	return (FALSE);
}

void	split_expansion_result(t_shell *data, t_prompt *prompt, t_token *tokens)
{
	char	**arr;
	int		result;
	int		count;
	int		i;

	i = 0;
	while (i < prompt->n_tokens)
	{
		if (if_was_expansion(tokens, i))
		{
			if (!set_arr(data, &arr, i, &count))
			{
				i++;
				continue ;
			}
			result = reorder_tokens(data, &tokens[i], count, arr);
			if (result == ERROR)
				return (ft_free_str_array(&arr),
					(void)exit_error(data, ERR_MALLOC, 1));
			tokens = data->prompt.tokens;
			update_info(prompt, arr, count, &i);
		}
		else
			i++;
	}
}
