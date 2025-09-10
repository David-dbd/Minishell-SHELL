#include "../../inc/minishell.h"

int send_tokens_for_expansion(t_shell *data, t_token *tokens, int phase)
{
	int i;

	i = 0;
	while (i < data->prompt.n_alloc_tokens)
	{
		if (tokens[i].type == EXPANSION)
		{
			if (expansion(data, &tokens[i], &data->env, phase) == ERROR)
				return (ERROR);
		}
		i++;
	}
	return (SUCCESS);
}
