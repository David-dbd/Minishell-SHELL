#include "../../../../inc/minishell.h"

int calculate_total_length(t_shell *data, char *str, char *key_to_find)
{
	t_var	*var;
	int	len_of_token;
	int	len_of_expansion;
	int	total_len;

	var = data->env.vars;
	len_of_token = 0;
	len_of_expansion = 0;
	len_of_token = ft_strlen(str);
	while (var != NULL)
	{
		if (ft_strcmp(var->key, key_to_find) == 0)
		{
			len_of_expansion = ft_strlen(var->value);
			break ;
		}
		var = var->next;
	}
	total_len = len_of_token + len_of_expansion;
	return (total_len);
}
