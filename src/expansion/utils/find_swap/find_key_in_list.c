
#include "../../../../inc/minishell.h"

int find_key_in_lst(t_shell *data, t_token *token, char *key_to_f)
{
	t_var	*var;

	var = data->env.vars;
	while (var != NULL)
	{
		if (ft_strcmp(var->key, key_to_f) == 0)
		{
			token->type = WORD;
			if (copy_value(data, &token->value, var->value, key_to_f) == ERROR)
			{
				exit_error(data, ERR_MALLOC, EXIT_USE);
				return (ERROR);
			}
			return (TRUE);
		}
		var = var->next;
	}
	return (FALSE);
}
