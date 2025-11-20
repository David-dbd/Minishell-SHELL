/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_expansion_result_utils.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 21:01:25 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/21 00:01:00 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

static void	altern_arr(t_shell *d, char ***arr, int add_space, int multiple_str)
{
	char	*tmp;

	tmp = NULL;
	if (add_space != 0 || (add_space == 0 && !multiple_str))
	{
		tmp = ft_strjoin((*arr)[add_space], " ");
		if (!tmp)
		{
			ft_free_str_array(arr);
			exit_error(d, ERR_MALLOC, EXIT_FAILURE);
		}
		free((*arr)[add_space]);
		(*arr)[add_space] = tmp;
	}
	else if (add_space == 0 && multiple_str)
	{
		tmp = ft_strjoin(" ", (*arr)[add_space]);
		if (!tmp)
		{
			ft_free_str_array(arr);
			exit_error(d, ERR_MALLOC, EXIT_FAILURE);
		}
		free((*arr)[add_space]);
		(*arr)[add_space] = tmp;
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

static int	prepare_arr(t_shell *data, char ***arr, int i)
{
	if (!is_it_just_spaces(data->prompt.tokens[i].value))
	{
		data->prompt.tokens[i].value[0] = ' ';
		data->prompt.tokens[i].value[1] = '\0';
		return (0);
	}
	*arr = ft_split(data->prompt.tokens[i].value, ' ');
	if (!*arr)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
	return (1);
}

static void	adjust_arr_spaces(t_shell *data, char ***arr, int i)
{
	int	len;

	if ((data->prompt.tokens[i].value[0] == ' '
			|| data->prompt.tokens[i].value[0] == '\t')
		&& (i > 0 && data->prompt.tokens[i - 1].type == NO_SPACE))
	{
		altern_arr(data, arr, 0, TRUE);
	}
	len = ft_strlen(data->prompt.tokens[i].value);
	if (len > 0 && (data->prompt.tokens[i].value[len - 1] == ' '
			|| data->prompt.tokens[i].value[len - 1] == '\t')
		&& ((i + 1) < data->prompt.n_tokens
			&& data->prompt.tokens[i + 1].type == NO_SPACE))
	{
		if (ft_count_str_in_arr(*arr) > 1)
			altern_arr(data, arr, ft_count_str_in_arr(*arr) - 1, TRUE);
		else
			altern_arr(data, arr, 0, FALSE);
	}
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

int	set_arr(t_shell *data, char ***arr, int i, int *count)
{
	if (!prepare_arr(data, arr, i))
		return (0);
	adjust_arr_spaces(data, arr, i);
	*count = ft_count_str_in_arr(*arr);
	return (1);
}
