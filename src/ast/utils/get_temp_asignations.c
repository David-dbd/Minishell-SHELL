/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_temp_asignations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:54:11 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/08 03:20:25 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/*
	Verifica si el token[i - 1] es una asignación temporal y la guarda como
	parte del contexto/metadata del comando que le sigue.
*/

static void	aux_alloc_mem(t_shell *data, char ***args, int n_args)
{
	*args = ft_calloc(n_args + 1, sizeof(char *));
	if (!*args)
		exit_error(data, ERR_MALLOC, EXIT_FAILURE);
}

static void	extract_bin_arg(t_shell *d, char **arg_extract, char *word, int len)
{
	*arg_extract = ft_strdup(word);
	if (!*arg_extract)
		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
}

char	**get_temp_asignations(t_shell *data, t_token *tokens, int i)
{
	char	**args;
	int		temp_count;
	int		temp_len;
	int		start;

	start = i - 1;
	temp_count = 0;
	while (start > 0 && tokens[start].type == TEMP_ASIGNATION)
	{
		temp_count++;
		start--;
	}
	if (temp_count == 0)
		return (NULL);
	aux_alloc_mem(data, &args, temp_count);
	while (start < i)
	{
		temp_len = ft_strlen(tokens[start].value);
		extract_bin_arg(data, &args[start], tokens[start].value, temp_len);
		start++;
	}
	return (args);
}
