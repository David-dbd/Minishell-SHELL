/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_expansion_result.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 23:19:18 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/19 13:26:45 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

static void	set_alternative_arr(t_shell *data, char ***arr, int add_space_index)
{
	char	*tmp;

	if (add_space_index != 0)
	{
		tmp = ft_strjoin((*arr)[add_space_index], " ");
		if (!tmp)
			free ((*arr)[add_space_index]);
		(*arr)[add_space_index] = tmp;
	}
	if (add_space_index == 0)
	{
		tmp = ft_strjoin(" ", (*arr)[add_space_index]);
		if (!tmp)
			free ((*arr)[add_space_index]);
		(*arr)[add_space_index] = tmp;
	}
}

static int	is_it_just_spaces(char *str)
{
	int	i;
	int	letter_flag;

	i = 0;
	letter_flag = FALSE;
	while (str[i] != '\0')
	{
		if (ft_isalnum(str[i]))
			letter_flag = TRUE;
		i++;
	}
	return (letter_flag);
}

/*
	Esta función verifica si el value del token solo tiene espacios o no.
	Si los tiene, hace un split de espacios para el word_splitting y luego
	verifica si hay que añadir un espacio tanto al inicio como al final del
	value siempre y cuando el value tenga espacios.

	var="  a   b  " ->hace split y pone espacio al inicio y al final
	var="a  b  " ->hace split y pone espacio al final
	var="  a  b" ->hace split y pone espacio al inicio

	Si solo son espacios, entonces los elimina y solo deja uno de ellos
*/

static int	set_arr(t_shell *data, char ***arr, int i, int *count)
{
	char	*tmp;
	int		len;

	if (!is_it_just_spaces(data->prompt.tokens[i].value))
	{
		data->prompt.tokens[i].value[0] = ' ';
		data->prompt.tokens[i].value[1] = '\0';
		return (0);
	}
	len = ft_strlen(data->prompt.tokens[i].value);
	*arr = ft_split(data->prompt.tokens[i].value, ' ');
	if (!*arr)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	if (data->prompt.tokens[i].value[0] == ' '
		|| data->prompt.tokens[i].value[0] == '\t')
		set_alternative_arr(data, arr, 0);
	if (len > 0 && (data->prompt.tokens[i].value[len - 1] == ' '
		|| data->prompt.tokens[i].value[len - 1] == '\t')
		&& ((i + 1) < data->prompt.n_tokens
		&& data->prompt.tokens[i + 1].type == NO_SPACE))
		set_alternative_arr(data, arr, ft_count_str_in_arr(*arr) - 1);
	*count = ft_count_str_in_arr(*arr);
	return (1);
}

static int	if_was_expansion(t_prompt *prompt, t_token *tokens, int i)
{
	if (prompt->before_tokens_type[i] == EXPANSION && tokens[i].type == WORD
		&& !tokens[i].double_quoted && !tokens[i].single_quoted
		&& (ft_strchr(tokens[i].value, ' ')
			|| ft_strchr(tokens[i].value, '\t')))
		return (TRUE);
	return (FALSE);
}

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

void	split_expansion_result(t_shell *data, t_prompt *prompt, t_token *tokens)
{
	char	**arr;
	int		result;
	int		count;
	int		i;

	i = 0;
	while (i < prompt->n_tokens)
	{
		if (if_was_expansion(prompt, tokens, i))
		{
			if (!set_arr(data, &arr, i, &count))
			{
				i++;
				continue ;
			}
			result = reorder_tokens(data, &tokens[i], count, arr);
			if (result == ERROR)
				return (ft_free_str_array(arr),
					(void)exit_error(data, ERR_MALLOC, 1));
			tokens = data->prompt.tokens;
			adjust_id(tokens, prompt->n_tokens);
			i += count;
			ft_free_str_array(arr);
		}
		else
			i++;
	}
}
