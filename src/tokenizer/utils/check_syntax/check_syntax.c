/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 19:06:35 by migarrid          #+#    #+#             */
/*   Updated: 2025/08/17 20:59:32 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Comprueba que un paréntesis de apertura tenga un comando antes de cerrarse.
	Si no encuentra un cierre válido, lanza un error de sintaxis.
*/

int	check_open_parent(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	int	content_flag;

	content_flag = 0;
	if (tokens[i].type == PAREN_OPEN)
	{
		//prompt->n_parentesis++;
		//printf("entra en open\n");
		if (i > 0 && tokens[i - 1].type && tokens[i - 1].type == PAREN_CLOSE)
		{
			//printf("fallo en open\n");
			syntax_error(data, ERR_SYNTAX, EXIT_USE, "(");
			return (SYNTAX_ERROR);
		}
		while (i < prompt->n_tokens)
		{
			//printf("entra en loop\n");
			if (tokens[i].type != PAREN_OPEN && tokens[i].type != PAREN_CLOSE)
			{
				//printf("content flag en open\n");
				content_flag++;
			}
			if (tokens[i].type == PAREN_CLOSE && i > 1 && content_flag >= 1)
			{
				//printf("Tot be con open\n");
				prompt->n_parentesis++;
				return (SUCCESS);
			}
			i++;
		}
		// printf("fallo en open\n");
		syntax_error(data, ERR_SYNTAX, EXIT_USE, "(");
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}

/*
	Comprueba que un paréntesis de cierre tenga un comando antes de la apertura.
	Si no encuentra un paréntesis abierto válido, lanza error de sintaxis.
*/

int	check_close_parent(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	int	content_flag;
	int	error_flag;
	int	j;

	content_flag = 0;
	error_flag = FALSE;
	j = i;
	if (tokens[i].type == PAREN_CLOSE)
	{
		//prompt->n_parentesis++;
		if (tokens[i - 1].type && (tokens[i - 1].type == PIPE || tokens[i - 1].type == OR || tokens[i - 1].type == AND) && content_flag == 0)
		{
			//printf("error_flag activated\n");
			error_flag = TRUE;
		}
		while (i >= 0)
		{
			if (tokens[i].type != PAREN_OPEN && tokens[i].type != PAREN_CLOSE)
				content_flag++;
			if (tokens[i].type == PAREN_OPEN && i < j && content_flag == 1 && error_flag == TRUE) //Si es un comando que tiene "|" justo antes de ")" y nada mas: (|)
			{
				// printf("Tot be con close\n");
				//printf("content flag: %d \n", content_flag);
				prompt->n_parentesis++;
				return (SUCCESS);
			}
			else if (tokens[i].type == PAREN_OPEN && i < j && content_flag >= 1 && error_flag == FALSE) //Si es un comando que no tiene un "|" justo antes del ")": (ls)
			{
				// printf("Tot be con close\n");
				//printf("content flag: %d \n", content_flag);
				prompt->n_parentesis++;
				return (SUCCESS);
			}
			else if (tokens[i].type == PAREN_OPEN && i < j && content_flag > 1 && error_flag == TRUE) //Si lo que esta antes de ")" es un "|" y hay mas contenido: (ls |)
			{
				// printf("fallo en  close\n");
				//printf("content flag: %d \n", content_flag);
				syntax_error(data, ERR_SYNTAX, EXIT_USE, ")");
				return (SYNTAX_ERROR);
			}
			i--;
		}
		// printf("fallo en ')' \n");
		syntax_error(data, ERR_SYNTAX, EXIT_USE, ")");
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}

/*
	Comprueba que el token '|' tenga un comando válido a izquierda y derecha.
	En caso contrario, lanza un error de sintaxis.
*/

int	check_pipe(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	// heardoc:    cmd | nothing if (tokens[i - 1].type && !tokens[i - 1].type)
	// command not found: cmd | cmd success;      cmd | word fails;   word | cmd fails and success;  word | word fails;
	// syntax error near unexpected token `|':    nothing | nothing fails;   nothing | cmd fails;
	if (tokens[i].type == PIPE)
	{
		if ((i > 0 && i < prompt->n_tokens)
			&& tokens[i + 1].type && tokens[i - 1].type
			&& ((tokens[i + 1].type == COMMAND
					|| tokens[i + 1].type == WORD
					|| tokens[i + 1].type == BUILT_IN)
				&& (tokens[i - 1].type == COMMAND
					|| tokens[i - 1].type == WORD
					|| tokens[i - 1].type == BUILT_IN)))
		{
			prompt->n_pipes++;
			return (SUCCESS);
		}
		//printf("entra en PIPE\n");
		//if ((i > 0 && i < prompt->n_tokens) && (tokens[i + 1].type && tokens[i - 1].type))
		//{
		//	if ()
		//	prompt->n_pipes++;
		//	return (0);
		//}
		// printf("error PIPE\n");
		syntax_error(data, ERR_SYNTAX, EXIT_USE, tokens[i].value);
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}

/*
	Comprueba que los operadores '||' y '&&' tengan un comando válido a ambos
	lados. Si no se cumple, lanza un error de sintaxis.
*/

int	check_or_and(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	// heardoc:    cmd && nothing if (tokens[i - 1].type && !tokens[i - 1].type)
	// command not found: cmd && cmd success;      cmd && word fails;   word && cmd fails and success;  word && word fails;
	// syntax error near unexpected token `&&':    nothing && nothing fails;   nothing && cmd fails;
	if (tokens[i].type == OR || tokens[i].type == AND)
	{
		//printf("entra a && ||\n");
		if ((i > 0 && i < prompt->n_tokens) && (tokens[i + 1].type && tokens[i - 1].type))
		{
			if (tokens[i].type == AND)
				prompt->n_and++;
			else if (tokens[i].type == OR)
				prompt->n_or++;
			return (SUCCESS);
		}
		// printf("error && ||\n");
		syntax_error(data, ERR_SYNTAX, EXIT_USE, tokens[i].value);
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}

/*
	Verifica que paréntesis, comillas y caracteres de escape estén en número
	par y correctamente emparejados.
*/

int	check_quotes(t_shell *data, t_prompt *prompt, t_token *tokens, int i)
{
	if (tokens[i].type == DOUBLE_QUOTE)
	{
		while (i < prompt->n_tokens)
		{
			if (tokens[i].type == DOUBLE_QUOTE && i >= 1)
			{	//printf("Tot be con double quote\n");
				prompt->n_double_quotes++;
				return (SUCCESS);
			}
			i++;
		}
		// printf("Falla en doublequote\n");
		syntax_error(data, ERR_SYNTAX, EXIT_USE, "\"");
		return (SYNTAX_ERROR);
	}
	else if (tokens[i].type == SINGLE_QUOTE)
	{
		while (i < prompt->n_tokens)
		{
			if (tokens[i].type == SINGLE_QUOTE && i >= 1)
			{	//printf("Tot be con single quote\n");
				prompt->n_single_quotes++;
				return (SUCCESS);
			}
			i++;
		}
		// printf("Falla en single\n");
		syntax_error(data, ERR_SYNTAX, EXIT_USE, "\'");//HEARDOC
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}
int	valid_pair_operands(t_shell *data, t_prompt *prompt, t_token *tokens)
{
	int i;
	int j;
	int k;
	int balance;

	i = 0;
	j = 0;
	k = 0;
	balance = 0;
	while (i < prompt->n_tokens)
	{
		if (tokens[i].type == PAREN_OPEN)
			balance++;
		else if (tokens[i].type == PAREN_CLOSE)
			balance--;
		if (balance < 0)
		{
			syntax_error(data, ERR_SYNTAX, EXIT_USE, ")");// cierre sin apertura
			return (SYNTAX_ERROR);
		}
		i++;
	}
	if (balance > 0)
	{
		syntax_error(data, ERR_SYNTAX, EXIT_USE, "(");// apertura sin cierre
		return (SYNTAX_ERROR);
	}
	balance = 0;
	while (j < prompt->n_tokens)
	{
		if (tokens[j].type == DOUBLE_QUOTE)
			balance++;
		else if (tokens[j].type == DOUBLE_QUOTE)
			balance--;
		if (balance < 0)
		{
			syntax_error(data, ERR_SYNTAX, EXIT_USE, "\"");// cierre sin apertura
			return (SYNTAX_ERROR);
		}
		j++;
	}
	if (balance > 0)
	{
		syntax_error(data, ERR_SYNTAX, EXIT_USE, "\"");// apertura sin cierre
		return (SYNTAX_ERROR);
	}
	balance = 0;
	while (k < prompt->n_tokens)
	{
		if (tokens[k].type == SINGLE_QUOTE)
			balance++;
		else if (tokens[k].type == SINGLE_QUOTE)
			balance--;
		if (balance < 0)
		{
			syntax_error(data, ERR_SYNTAX, EXIT_USE, "\'");// cierre sin apertura
			return (SYNTAX_ERROR);
		}
		k++;
	}
	if (balance > 0)
	{
		syntax_error(data, ERR_SYNTAX, EXIT_USE, "\'");// apertura sin cierre
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}
