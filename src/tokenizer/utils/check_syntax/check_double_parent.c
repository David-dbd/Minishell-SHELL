/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_double_parent.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 18:14:29 by davdiaz-          #+#    #+#             */
/*   Updated: 2025/10/30 01:46:37 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

// static void	put_space_between(t_shell *d, char **str, char **tmp, char **space)
// {
// 	*space = ft_strjoin(*str, " ");
// 	free (*str);
// 	if (!*space)
// 		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
// 	*tmp = *space;
// }

// static char	*msg_construction(t_shell *d, t_token *tokens, int start, int len)
// {
// 	char	*str;
// 	char	*tmp;
// 	char	*space;

// 	tmp = ft_strdup("");
// 	if (!tmp)
// 		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
// 	while (start < len)
// 	{
// 		str = ft_strjoin(tmp, tokens[start].value);
// 		if (!str)
// 		{
// 			if (tmp)
// 				free (tmp);
// 			exit_error(d, ERR_MALLOC, EXIT_FAILURE);
// 		}
// 		free (tmp);
// 		tmp = NULL;
// 		if (start + 1 < len)
// 			put_space_between(d, &str, &tmp, &space);
// 		else
// 			tmp = str;
// 		start++;
// 	}
// 	return (str);
// }

// static int	ignore_till_paren_close(t_prompt *prompt, t_token *tokens, int i)
// {
// 	while (i + 1 < prompt->n_tokens && !(tokens[i].type == PAREN_CLOSE
// 			&& tokens[i + 1].type == PAREN_CLOSE))
// 		i++;
// 	return (i);
// }

// int	check_double_parent(t_shell *data, t_token *tokens, t_prompt *p)
// {
// 	char	*str;
// 	int		i;
// 	int		start;

// 	i = 0;
// 	while (i < p->n_tokens)
// 	{
// 		if (tokens[i].type == PAREN_OPEN && tokens[i + 1].type == PAREN_OPEN)
// 		{
// 			i += 2;
// 			start = i;
// 			i = ignore_till_paren_close(p, tokens, i);
// 			if (i + 1 < p->n_tokens && tokens[i].type == PAREN_CLOSE
// 				&& tokens[i + 1].type == PAREN_CLOSE)
// 			{
// 				str = msg_construction(data, tokens, start, i);
// 				if (!str)
// 					exit_error(data, ERR_MALLOC, EXIT_FAILURE);
// 				syntax_error(data, ERR_DOUBLE_PARENT, EXIT_USE, str, str);
// 				free (str);
// 				return (SYNTAX_ERROR);
// 			}
// 		}
// 		i++;
// 	}
// 	return (SUCCESS);
// }


// Verifica si es contexto aritmético (sin pipes ni comandos)
static int	is_arithmetic_context(t_token *tokens, int i, int n_tokens)
{
	i += 2;
	while (i < n_tokens && tokens[i].type != PAREN_CLOSE)
	{
		if (tokens[i].type == PIPE || is_cmd_builtin_type(tokens[i].type))
			return (0);
		i++;
	}
	if (i + 1 < n_tokens && tokens[i].type == PAREN_CLOSE
		&& tokens[i + 1].type == PAREN_CLOSE)
		return (1);
	return (0);
}

// Encuentra el final de la expresión aritmética ))
static int	find_arithmetic_end(t_token *tokens, int start, int n_tokens)
{
	int	i;
	int	depth;

	i = start + 2;
	depth = 2;
	while (i < n_tokens && depth > 0)
	{
		if (tokens[i].type == PAREN_OPEN)
			depth++;
		else if (tokens[i].type == PAREN_CLOSE)
			depth--;
		i++;
	}
	return (i - 1);
}

// Construye la expresión aritmética como string
static char	*extract_arithmetic_expr(t_shell *d, t_token *tokens, int start, int end)
{
	char	*expr;
	char	*tmp;
	char	*new;
	int		i;

	expr = ft_strdup("");
	if (!expr)
		exit_error(d, ERR_MALLOC, EXIT_FAILURE);
	i = start + 2;
	while (i < end - 1)
	{
		tmp = expr;
		new = ft_strjoin(tmp, tokens[i].value);
		free(tmp);
		if (!new)
			exit_error(d, ERR_MALLOC, EXIT_FAILURE);
		expr = new;
		if (i + 1 < end - 1)
		{
			tmp = expr;
			expr = ft_strjoin(tmp, " ");
			free(tmp);
			if (!expr)
				exit_error(d, ERR_MALLOC, EXIT_FAILURE);
		}
		i++;
	}
	return (expr);
}

// Evalúa expresión aritmética simple (solo números y operadores básicos)
static long	evaluate_arithmetic(char *expr)
{
	long	result;
	long	num;
	char	op;
	char	*ptr;

	result = 0;
	num = 0;
	op = '+';
	ptr = expr;

	while (*ptr)
	{
		while (*ptr == ' ')
			ptr++;
		if (*ptr >= '0' && *ptr <= '9')
		{
			num = 0;
			while (*ptr >= '0' && *ptr <= '9')
			{
				num = num * 10 + (*ptr - '0');
				ptr++;
			}
			if (op == '+')
				result += num;
			else if (op == '-')
				result -= num;
			else if (op == '*')
				result *= num;
			else if (op == '/' && num != 0)
				result /= num;
			else if (op == '%' && num != 0)
				result %= num;
		}
		else if (*ptr == '+' || *ptr == '-' || *ptr == '*'
			|| *ptr == '/' || *ptr == '%')
		{
			op = *ptr;
			ptr++;
		}
		else
			ptr++;
	}
	return (result);
}

// Ejecuta la expresión aritmética y actualiza exit_code
static int	execute_arithmetic(t_shell *data, t_token *tokens, int start, int end)
{
	char	*expr;
	long	result;

	expr = extract_arithmetic_expr(data, tokens, start, end);
	if (!expr)
		return (SYNTAX_ERROR);

	result = evaluate_arithmetic(expr);
	free(expr);

	// En bash: (( )) retorna 0 si resultado != 0, y 1 si resultado == 0
	if (result != 0)
		data->exit_code = 0;
	else
		data->exit_code = 1;

	return (2);
}

int	check_double_parent(t_shell *data, t_token *tokens, t_prompt *p)
{
	int		i;
	int		end;

	i = 0;
	while (i < p->n_tokens)
	{
		if (tokens[i].type == PAREN_OPEN && i + 1 < p->n_tokens
			&& tokens[i + 1].type == PAREN_OPEN)
		{
			if (is_arithmetic_context(tokens, i, p->n_tokens))
			{
				// Es aritmética - ejecutarla
				end = find_arithmetic_end(tokens, i, p->n_tokens);
				return (execute_arithmetic(data, tokens, i, end));
			}
			// Si no es aritmética, son subshells anidados - permitir
		}
		i++;
	}
	return (SUCCESS);
}
