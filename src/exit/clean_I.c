/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_I.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 21:47:38 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/30 16:26:55 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Libera los valores de los tokens allocados y luego libera
	la memoria del array de tokens.
*/

void	clean_tokens(t_prompt **prompt)
{
	int	i;

	i = 0;
	while (i < (*prompt)->n_alloc_tokens)
	{
		if ((*prompt)->tokens[i].value
			&& (is_alloc_type((*prompt)->tokens[i].type)))
		{
			free((*prompt)->tokens[i].value);
			(*prompt)->tokens[i].value = NULL;
		}
		i++;
	}
	free((*prompt)->tokens);
	(*prompt)->tokens = NULL;
}

/*
	Libera la cadena 'prompt' del input, los tokens y reinicia la
	estructura de 'prompt' a cero.
*/

void	clean_prompt(t_prompt *prompt)
{
	if (prompt->input)
		free(prompt->input);
	clean_tokens(&prompt);
	*prompt = (t_prompt){0};
}

/*
	Libera la lista enlazada de variables de entorno, incluyendo
	'key' y 'value' de cada nodo.
*/

void	clean_env(t_env *env, t_var *vars)
{
	t_var	*var;
	t_var	*next;

	var = vars;
	while (var)
	{
		next = var->next;
		if (var->key)
		{
			free(var->key);
			var->key = NULL;
		}
		if (var->value)
		{
			free(var->value);
			var->value = NULL;
		}
		free(var);
		var = next;
	}
	ft_free_str_array(env->envp);
}

/*
	Libera los nodos del AST de manera recursiva, yendo hasta el ultimo
	left, luego hasta el ultimo right y ahi liberando cada uno hacia arriba.
*/

void	clean_ast(t_node *node)
{
	if (!node)
		return ;
	clean_ast(node->left);
	clean_ast(node->right);
	clean_node(&node);
}

/*
	Limpia todos los recursos del shell: historial, prompt y
	variables de entorno asociadas a 'data'.
*/

void	clean_all(t_shell *data)
{
	clean_prompt(&data->prompt);
	clean_env(&data->env, data->env.vars);
	clean_ast(data->ast_root);
	clean_fd(&data->exec);
	clean_extras(&data->extras);
}
