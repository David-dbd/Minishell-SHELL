#include "../../../../inc/minishell.h"

void transform_word_to_asignation(t_shell *data, t_token *tokens)
{
	int i;
	int	result;

	i = 0;
	while (i < data->prompt.n_tokens)
	{
		if (tokens[i].type != WORD)
		{
			i++;
			continue ;
		}
		result = check_externs_syntax(data, &tokens[i], &result);
		if (result == TRUE)
			tokens->type = ASIGNATION;
		else if (result == IGNORE)
			tokens[i].type = DELETE; //o simpelmete un bool que diga: state: ignore;
		i++;
	}
}
