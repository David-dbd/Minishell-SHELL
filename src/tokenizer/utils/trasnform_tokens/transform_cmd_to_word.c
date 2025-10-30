/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_cmd_to_word.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarrid <migarrid@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 15:39:12 by migarrid          #+#    #+#             */
/*   Updated: 2025/10/30 01:07:48 by migarrid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../inc/minishell.h"

/*
	Estas dos funciones se encargan de evaluar tokens->WORD para verificar
	si en realidad son COMMAND o BUILT_IN.

	Logica:
	Asumiendo que la INITAL_PHASE funciono correctamente, si un 'ls' es un WORD
	es porque es el argumento de algún comando o un built-in que se encuentra
	atras y esto esta BIEN.
	Sin embargo, aún existe la posibilidad de que al no haber podido evaluar los
	tokens con información más precisa ya que no existian ASIGNATIONS en esa
	fase, cabe la posibilidad de que 'ls' sea una palabra simplemente porque
	una asignacion era anteriormenet catalogada como WORD, de modo que
	automaticamente ls también lo es.
	Es por esto que hay que revisar ese caso; Cuando un comando o built-in
	estan catalogados como WORD pero deberian de ser COMMAND o BUILT_IN.

	Es por esta razón que se verifica si el anterior token es una ASIGNATION
	(Esto ya nos dice que efectivamente 'ls' puede estar mal catalogado)
	y si al ir hacia atras, si encontramos un delimitador antes que un export
	(si te preguntas porque un export lo dejare al final del comentario), es
	porque efectivamente ls esta mal catalogado, de modo que pasa a ser COMMAND.
	Lo mismo sucede con cualquier built-in.

	Si por el contrario, encontramos un BUILT_IN export antes, significa que ls
	esta bien catalogado ya que efectivamente es un argumento de export, con lo
	cual no se cambia su tipo.

	Por qué export?

	Esto se debe a que si un WORD tiene a su lado izquierdo (i - 1) un
	ASIGNATION y este token no es WORD, es porque no es el argumento de ningún
	comando o built-in, de modo que solo nos deja con 2 posbles casos:

	1. Es una asignación propuesta por un export, es decir, es un argumento de
	un export.
	2. Es una asignación sin un export.

	Si es el primer caso, como ya había dicho antes, no pasa nada.
	si es el segundo caso, hay que cambiar WORD -> COMMAND/BUILT_IN ya que esa
	ASIGNATION es temporal y por ende no afectara al proceso padre.

	Para el padre, un prompt como: VAR=1 ls a=1 -> Empieza en realidad en ls
	ya que VAR=1 es una variable temporal que será heredada al hijo (ls).
*/

static int	verify_till_valid_token(t_token *tokens, int i)
{
	while (i >= 0 && tokens[i].type)
	{
		if (is_delimiter_type(tokens[i].type))
			return (SUCCESS);
		if (tokens[i].type == BUILT_IN
			&& ft_strcmp(tokens[i].value, BUILTIN_EXPORT) == 0)
			return (FALSE);
		if (tokens[i].type == COMMAND)
			return (FALSE);
		if (i == 0 && tokens[i].type == ASIGNATION)
			return (SUCCESS);
		i--;
	}
	return (FALSE);
}

static void	check_again_more_tokens(t_token *tokens, t_token *token)
{
	int	i;

	i = token->id;
	if (i >= 1 && (ft_strcmp(token->value, BUILTIN_EXPORT) == 0
			|| ft_strcmp(token->value, BUILTIN_ECHO) == 0
			|| ft_strcmp(token->value, BUILTIN_ENV) == 0
			|| ft_strcmp(token->value, BUILTIN_EXIT) == 0
			|| ft_strcmp(token->value, BUILTIN_UNSET) == 0
			|| ft_strcmp(token->value, BUILTIN_CD) == 0
			|| ft_strcmp(token->value, BUILTIN_PWD) == 0)
		&& tokens[i - 1].type == ASIGNATION
		&& verify_till_valid_token(tokens, i))
		token->type = BUILT_IN;
	else if (i >= 1 && tokens[i - 1].type == ASIGNATION
		&& verify_till_valid_token(tokens, i))
		token->type = COMMAND;
}

/*
	Decide si un token de tipo comando debe transformarse en WORD,
	para que los argumentos pegados a comandos se interpreten correctamente.
	Esto ocurre cuando:
	- El comando está al inicio (i == 1) y el anterior es un comando válido.
	- El token anterior es comando y no está precedido por redirección.
	- El token anterior es una comilla y antes había un comando.
	- Casos más complejos con comillas dobles/NO_SPACE antes de un comando.
*/

static int	before_is_cmd_word_redir(t_token *tokens, int i)
{
	if ((i >= 4 && tokens[i - 1].type == WORD
			&& tokens[i - 2].type == NO_SPACE
			&& tokens[i - 3].type == EXPANSION
			&& is_redir_type(tokens[i - 4].type)))
		return (0);
	else if ((i >= 2 && tokens[i - 1].type == BACKGROUND
			&& (tokens[i - 2].type == REDIR_OUTPUT
				|| tokens[i - 2].type == REDIR_APPEND)))
		return (1);
	else if (i >= 2 && tokens[i - 1].type == WORD
		&& !is_redir_type(tokens[i - 2].type))
		return (1);
	else if (i >= 2 && tokens[i - 1].type == EXPANSION
		&& !is_redir_type(tokens[i - 2].type))
		return (1);
	else if (i >= 1 && is_cmd_builtin_type(tokens[i - 1].type))
		return (1);
	else if (i >= 1 && is_redir_type(tokens[i - 1].type))
		return (1);
	else if (i == 1 && tokens[i - 1].type == EXPANSION)
		return (1);
	return (0);
}

/*
	INTIAL_PAHSE:
	Revisa todos los tokens del prompt y ajusta tipos:
	- Si cumple condiciones de transformación, convierte COMMAND en WORD.
	- Si hay NO_SPACE cerca, también convierte el token en WORD.
	Esto asegura que los argumentos de comandos se tokenicen correctamente.

	FINAL_PAHSE:
	Revisa tokens que esten desactualizados ya que ahroa cuenta con nueva
	clase de tokens ASIGNATION que afecta a otros.
*/

void	transform_cmd_to_word(t_shell *data, t_token *tokens, int phase)
{
	int	i;

	i = 0;
	while (i < data->prompt.n_tokens)
	{
		if (phase == INITIAL_PHASE)
		{
			if (i == 0 || (i >= 1 && is_delimiter_type(tokens[i - 1].type)))
			{
				if (tokens[i].type == WORD)
					tokens[i].type = COMMAND;
			}
			else if (before_is_cmd_word_redir(tokens, i))
			{
				if (is_primitive_cmd_type(tokens[i].type))
					tokens[i].type = WORD;
			}
		}
		else if (phase == FINAL_PHASE && tokens[i].type == WORD)
			check_again_more_tokens(tokens, &tokens[i]);
		i++;
	}
}
