
#include "../../../../inc/minishell.h"


static int check_case_1(t_token *token)
{
	if (token[token->id - 1].type && token[token->id + 1].type)
	{
		if (((token[token->id - 1].type == AND
			|| token[token->id - 1].type == OR
			|| (token[token->id - 1].type == BUILT_IN)
			&& ft_strcmp(token[token->id - 1].value, BUILTIN_EXPORT) == 0) //importante manerjarlo en el arbol si es false o true
			&& (token[token->id + 1].type == AND
				|| token[token->id + 1].type == OR))
			|| (token[token->id + 1].type == BUILT_IN)
			&& ft_strcmp(token[token->id + 1].value, BUILTIN_EXPORT) == 0)
		return (1);
	}
	return (0); //comillas hace que no se tenga en cuenta "hola""var=hola"
}

static int check_case_2(t_token *token)
{
	if (token[token->id - 1].type && !token[token->id + 1].type)
	{
		if (token[token->id - 1].type == AND
			|| token[token->id - 1].type == OR
			|| (token[token->id - 1].type == BUILT_IN
			&& ft_strcmp(token[token->id - 1].value, BUILTIN_EXPORT) == 0))
		return (1);
	}
	return (0); //if not, then it needs to remain as WORD
}

static int check_case_3(t_token *token) //Aqui solo va a entrar si es local, porque si fuese por export, si existiria i - 1
{
	if (!token[token->id - 1].type && token[token->id + 1].type)
	{
		if (token[token->id + 1].type == AND || token[token->id + 1].type == OR
			|| token[token->id + 1].type == ASIGNATION
			|| (token[token->id + 1].type == BUILT_IN
				&& ft_strcmp(token[token->id + 1].value, BUILTIN_EXPORT) == 0))
		return (1);
		else if (token[token->id + 1].type == WORD
			|| token[token->id + 1].type == COMMAND
			|| (token[token->id + 1].type == BUILT_IN
			&& ft_strcmp(token[token->id + 1].value, BUILTIN_EXPORT) != 0))
			return (IGNORE); //Yep, that's right, it needs to ignore this token.
	}
	return (0);
}

int check_externs_syntax(t_shell *data, t_token *token, int *result)
{
	if (token->id > 0 && token->id < data->prompt.n_tokens)
	{
		if (check_case_1(token))
			*result = check_asignation_syntax(token);
		else if (check_case_2(token))
			*result = check_asignation_syntax(token);
		else if (check_case_3(token))
			*result = check_asignation_syntax(token);
	}
	else
		*result = check_asignation_syntax(token); //en caso de que sea el primero y no haya ningun otro token
	if (check_case_3(token) == IGNORE)
		*result = IGNORE;
	return (*result);
}
