/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_temp_asignations.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:54:11 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/25 22:29:26 by migarrid         ###   ########.fr       */
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

/*
	Esto es porque después de restar a start, puede ser -1 o estar en un token
	que no corresponda a una asignacion temporal, de modo que hay que ubicar
	start en la TEMP_ASIG.
	Eg.
	(var=hola ls) -> start--; -> token[start].type == PAREN_OPEN
	var=hola ls -> start--; -> start == -1
*/

static int	get_correct_index(t_token *tokens, int start)
{
	while (start <= 0)
	{
		start++;
		return (start);
	}
	while (!is_asignation_type(tokens[start].type))
		start++;
	return (start);
}

char	**get_temp_asignations(t_shell *data, t_token *tokens, int i)
{
	char	**args;
	int		temp_count;
	int		temp_len;
	int		start;

	start = i;
	start = get_correct_index(tokens, i);
	temp_count = 0;
	//printf("%d\n\n", start);
	while (start >= 0 && tokens[start].type == TEMP_ASIGNATION
		|| tokens[start].type == TEMP_PLUS_ASIGNATION)
	{
		temp_count++;
		start++;
	}
	if (temp_count == 0 || start == 0)
		return (NULL);
	aux_alloc_mem(data, &args, temp_count);
	if (start < 0 || (start == 0 && !is_asignation_type(tokens[start].type)) || (start > 0 && !is_asignation_type(tokens[start].type)))
		start = get_correct_index(tokens, start);
	//printf("token acutal: %s\n\n", tokens[start].value);
	temp_count = 0;
	while (start < i)
	{
		temp_len = ft_strlen(tokens[start].value);
		extract_bin_arg(data, &args[temp_count], tokens[start].value, temp_len);
		//printf("Temp[%d]: %s\n\n", start, args[start]);
		start++;
		temp_count++;
	}
	return (args);
}
