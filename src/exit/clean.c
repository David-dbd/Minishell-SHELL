/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 21:47:38 by migarrid          #+#    #+#             */
/*   Updated: 2025/09/12 23:53:40 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Libera los valores de los tokens allocados y luego libera
	la memoria del array de tokens.
*/

void	clean_tokens(t_prompt *prompt)
{
	int	i;

	i = 0;
	while (i < prompt->n_alloc_tokens)
	{
		if (prompt->tokens[i].value
			&& (prompt->tokens[i].type == WORD
				|| prompt->tokens[i].type == BUILT_IN
				|| prompt->tokens[i].type == COMMAND
				|| prompt->tokens[i].type == WILDCAR
				|| prompt->tokens[i].type == REDIR_APPEND
				|| prompt->tokens[i].type == REDIR_HEREDOC
				|| prompt->tokens[i].type == REDIR_INPUT
				|| prompt->tokens[i].type == REDIR_OUTPUT
				|| prompt->tokens[i].type == EXPANSION))
		{
			free(prompt->tokens[i].value);
			prompt->tokens[i].value = NULL;
		}
		i++;
	}
	free(prompt->tokens);
	prompt->tokens = NULL;
}

/*
	Libera la cadena 'prompt' del input, los tokens y reinicia la
	estructura de 'prompt' a cero.
*/

void	clean_prompt(t_prompt *prompt)
{
	if (prompt->prompt)
		free(prompt->prompt);
	clean_tokens(prompt);
	*prompt = (t_prompt){0};
}

/*
	Libera la memoria reservada para `user_name` en `t_extras`
	y lo deja apuntando a NULL para evitar accesos inválidos.
*/

void	clean_extras(t_extras *extra_features)
{
	if (extra_features->user_name)
		free(extra_features->user_name);
	extra_features->user_name = NULL;
}

/*
	Libera la lista enlazada de variables de entorno, incluyendo
	'key' y 'value' de cada nodo.
*/

void	clean_env(t_var *vars)
{
	t_var	*var;
	t_var	*next;

	var = vars;
	while (var)
	{
		next = var->next;
		if (var->key)
			free(var->key);
		if (var->value)
			free(var->value);
		free(var);
		var = next;
	}
}

/*
	Limpia todos los recursos del shell: historial, prompt y
	variables de entorno asociadas a 'data'.
*/

void	clean_all(t_shell *data)
{
	clear_history();
	clean_prompt(&data->prompt);
	clean_env(data->env.vars);
	clean_extras(&data->extra_features);
}
