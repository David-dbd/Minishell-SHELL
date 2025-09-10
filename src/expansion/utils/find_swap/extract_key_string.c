
#include "../../../../inc/minishell.h"

int extract_key_string(t_shell *data, t_token *token, char **key_to_find, int phase)
{
	int	j;
	int	k;
	int	found;
	int	match;

	j = 0;
	k = 0;
	found = FALSE;
	match = copy_key(token->value, key_to_find);
	if (!match)
		return (FAILURE);
	found = find_key_in_lst(data, token, *key_to_find);
	if (found == ERROR)
		return (ERROR);
	if (!found && phase == 2)
	{
		token->value = ft_strdup(""); //cambiar el value por una str vacia si no se haya CREO QU ESTO ESTA MAL porque reemplaza tooodo el valu por "" y solo necesito que reemplaze la expansion
		if (!token->value)
		{
			exit_error(data, ERR_MALLOC, EXIT_USE);
			return (ERROR);
		}
	}
	return (SUCCESS);
}
