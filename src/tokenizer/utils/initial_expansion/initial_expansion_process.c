/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initial_expansion_process.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davdiaz- <davdiaz-@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/23 16:41:34 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/11/23 17:03:27 by davdiaz-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

static void	search_value_in_tokens(t_token *tokens, char *str, int n_tokens, int i)
{
	while (i < n_tokens)
	{
		if (tokens[i].type == EXPANSION)
		{
			if (ft_strcmp(tokens[i].value + 1, str) == 0) //omito el dolar
				tokens[i].expand = FALSE;
		}
		i++;
	}
}

static void	if_asignation(t_shell *data, t_token *token)
{
	char	*key;
	char	*value;
	char	*result;
	int		i;

	i = 0;
	result = NULL;
	key = NULL;
	value = NULL;
	if (check_asignation_syntax(token, ENV))
	{
		if (aux_mem_alloc_asignation(&key, &value, ft_strlen(token->value)) == ERROR)
			exit_error(data, ERR_MALLOC, EXIT_FAILURE);
		aux_key_asig(token, &key, &i);
		aux_value_asig(token, &value, &i);
		result = get_var_value(data->env.vars, key);
		if (result && ft_strcmp(result, value) != 0)
			search_value_in_tokens(data->prompt.tokens, key, data->prompt.n_tokens, token->id);
	}
	free (key);
	free (value);
}

static int	check_for_context(t_token *tokens, int i)
{
	while (i >= 0)
	{
		if (tokens[i].type == BUILT_IN
				&& ft_strcmp(tokens[i].value, BUILTIN_EXPORT) == 0)
			return (TRUE);
		if (is_delimiter_type(tokens[i].type))
			return (FALSE);
		i--;
	}
	return (FALSE);
}

/*
	Funcion para que en casos como:

	Prompt_1: var=1;
	Prompt_2: var=2 && echo $var ->Esto expandira a 1 antes de que se asigne 2

	Solucion
	Prompt_1: var=1;
	Prompt_2: var=2 && echo $var ->Dont expand $var hasta la ejecucion

	Es decir, se verifica si algun token que sea asignacion valida
	esta actualizando su valor, de modo que si es asi, entonces se bloquean
	todas las expansiones relacionadas con la clave de esa asignacion. De este
	modo no se expandiran valores desactualiados antes de que se ejecute
	la asignacion
*/
static void	dont_expand_this(t_shell *data, t_prompt *prompt, t_token *tokens)
{
	int	i;

	i = 0;
	while (i < prompt->n_tokens)
	{
		if (i == 0)
			if_asignation(data, &tokens[i]); //aqui checo el caso especial de la primera posicion
		else if (i > 0 && is_delimiter_type(tokens[i].type)) //aqui busco export por la izquierda del delimitador y también compruebo por la derecha del delimi
		{
			if ((i + 1) < prompt->n_tokens) //check cmd after delimiters
				if_asignation(data, &tokens[i + 1]);
			if (check_for_context(tokens, i - 1)) //check for export
				if_asignation(data, &tokens[i - 1]);
		}
		i++;
	}
}
static void	adjust_expansion(t_token *tokens, int n_tokens)
{
	int	i;

	i = 0;
	while (i < n_tokens)
	{
		if (tokens[i].type == EXPANSION)
			tokens[i].expand = TRUE;
		i++;
	}
}

int	initial_expansion_process(t_shell *data, t_prompt *prompt)
{
	dont_expand_this(data, prompt, prompt->tokens);
	create_before_tokens(data, prompt->tokens, prompt);
	if (expansion(data, prompt->tokens, 0, INITIAL_PHASE) == ERROR)
		return (FAILURE);
	adjust_expansion(prompt->tokens, prompt->n_tokens);
	split_expansion_result(data, prompt, prompt->tokens);
	return (SUCCESS);
}
