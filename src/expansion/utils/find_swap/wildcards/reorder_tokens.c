#include "../../../../../inc/minishell.h"

/*
	Funcion para eliminar los tokens tmp que se han creado hasta el moemento.

	Si i = 5; y falla malloc, entonces se liberan los tmp[5] que si fueron
	allocated para que no hayan leaks
*/

void	free_tmp_tokens(t_token *tmp, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (tmp[i].value)
		{
			free(tmp[i].value);
			tmp[i].value = NULL;
		}
		i++;
	}
	free(tmp);
}

int copy_token_members(t_token *tmp_index, t_token *src)
{
	tmp_index->type = src->type;
	tmp_index->id = src->id;
	tmp_index->hash = src->hash;
	tmp_index->double_quoted = src->double_quoted;
	tmp_index->single_quoted = src->single_quoted;
	if (is_alloc_type(src->type))
	{
		tmp_index->value = ft_strdup(src->value);
		if (!tmp_index->value)
			return (ERROR);
	}
	return (SUCCESS);
}


/*
	FunciÃ³n para copiar en el nuevo arr tmp los tokens en el rango desde 0
	hasta el Ãºltimo indice (sin copiar el ultimo, es decir, hasta el penultimo)

	Ejemplo:
	prompt->tokens = a b c d e
	nuevo_arr = a b c d
*/

static int	copy_orig_tokens(t_shell *data, t_token *tmp, int ignore)
{
	int	i;
	int	tmp_i;

	i = 0;
	tmp_i = 0;
	while (i < ignore)
	{
		if (copy_token_members(&tmp[tmp_i], &data->prompt.tokens[i]) == ERROR)
		{
			free_tmp_tokens(tmp, tmp_i);
			return (ERROR);
		}
		else
			//printf("copy_orig_tokens token[i]: %d\n\n", data->prompt.tokens[i].type);
		tmp_i++;
		i++;
	}
	return (tmp_i);
}

/*
	Funcion para generar hashes validos, ya que al expandir un wildcard hay
	altas posibilidades de que los haya colisión entre los hashes de los tokens
	y los hashes que se le asignan a los nuevos tokens insertados a partir
	del wildcard.

	Ejemplo:

	Tokens: 1 2 3 4 5 6 ->El 3 es el wildcard a expandir

	new_tokens: 1 2 new_token new_token new_token 4 5 6 -> colisión, ya que
	los cada new_sigue el orden, es decir: 1 2 3 4 5 4 5 6. ERRROR
	deberia de ser 1 2 3 4 5 6 7 8
*/

static int generate_valid_hash(t_shell *data, int n_dirs, int n_tokens, int starting_hash)
{
	int	i;
	int	collision;
	int	hash_generator;

	hash_generator = starting_hash + data->prompt.n_tokens + n_dirs;
	while (1)
	{
		collision = FALSE;
		i = 0;
		while (i < n_tokens)
		{
			if (data->prompt.tokens[i].hash == hash_generator)
			{
				collision = TRUE;
				hash_generator++;
				break;
			}
			i++;
		}
		if (collision == FALSE)
			return (hash_generator);
	}
}

static int	count_dirs(char **dirs)
{
	int	i;

	i = 0;
	while (dirs[i] != NULL)
		i++;
	return (i);
}

/*
	FunciÃ³n para copiar los tokens que se crearon a partir del wildcard
	(que estan de Ãºltimos en prompt->tokens) desde donde estaba el Ãºltimo
	token anterior.

	Ejemplo:
	prompt->tokens = a b c d e
	new_tokens = f g h i
	old_arr = a b c d e f g h i
	se copian los new = a b c d f g h i
*/

static int	copy_new_tokens(t_shell *data, t_token *tmp, int *i, char **dirs)
{
	int	j;

	j = 0;
	while (dirs[j] != NULL)// copio los nuevos tokens del wildcard
	{
		tmp[*i].type = WORD;
		tmp[*i].double_quoted = FALSE;
		tmp[*i].single_quoted = FALSE;
		if (j == 0)
			tmp[*i].id = *i;//reemplaza el hash del wildcard por si llegase a transformarse en cmd
		else
			tmp[*i].hash = generate_valid_hash(data, count_dirs(dirs),
				data->prompt.n_tokens, 0);//needs to generate a new hash because of collisions
		tmp[*i].value = ft_strdup(dirs[j]);
		if (!tmp[*i].value)
		{
			free_tmp_tokens(tmp, *i);
			return (ERROR);
		}
		(*i)++;
		j++;
	}
	return (SUCCESS);
}

/*
	FunciÃ³n para copiar el Ãºltimo token del old_arr al Ãºltimo index del new_arr
	despuÃ©s de haber traido los new_tokens.

	Ejemplo:
	prompt->tokens = a b c d e
	new_tokens = f g h i
	juntos = a b c d e f g h i
	se copian los new = a b c d f g h i e
*/

static int	copy_last_token(t_shell *data, t_token *tmp, int *i, int j)
{
	if (j < data->prompt.n_tokens)
	{
		while (j < data->prompt.n_tokens)
		{
			if (copy_token_members(&tmp[*i], &data->prompt.tokens[j]) == ERROR)
			{
				free_tmp_tokens(tmp, *i);
				return (ERROR);
			}
			(*i)++;
			j++;
		}
	}
	return (SUCCESS);
}

void	free_tokens(t_prompt *prompt)
{
	int i;

	if (!prompt || !prompt->tokens)
		return ;
	i = 0;
	while (i < prompt->n_tokens)
	{
		if (is_alloc_type(prompt->tokens[i].type) && prompt->tokens[i].value)
		{
			free (prompt->tokens[i].value);
			prompt->tokens[i].value = NULL;
		}
		i++;
	}
	free (prompt->tokens);
	prompt->tokens = NULL;
	prompt->n_tokens = 0;
	prompt->n_alloc_tokens = 0;
}

/*
	FunciÃ³n para reordenar los tokens de modo que los nuevos tokens que
	salieron a partir de la expansiÃ³n del wildcard (y que estan de ultimo
	en el arr) puedan cambiar posiciÃ³n con el token que se encuentra actualmente
	de Ãºltimo.

	La logica es copiar los tokens en un nuevo arr, teniendo en cuenta el orden.
	Luego se elimina el old_arr y se actualizan ptrs y otros datos
*/

int	reorder_tokens(t_shell *d, t_token *orig_token, int n_dirs, char **dirs)
{
	int		i;
	int		j;
	int		orig_size;
	int		wildcard;
	t_token	*tmp;

	orig_size = d->prompt.n_tokens;
	wildcard = orig_token->id;
	tmp = ft_calloc((d->prompt.n_alloc_tokens + n_dirs), sizeof(t_token)); // + n_dirs porque puede que al allocate x bites, puede que no alcance porque el wildcard son 50 items, de modo que se perderan algunos
	if (!tmp)
		return (ERROR);
	i = copy_orig_tokens(d, tmp, orig_token->id);
	if (i == ERROR)
		return (ERROR);
	if (copy_new_tokens(d, tmp, &i, dirs) == ERROR)
		return (ERROR);
	if (copy_last_token(d, tmp, &i, wildcard + 1) == ERROR)
		return (ERROR);
	// print_tokens_debug(&d->prompt);
	j = d->prompt.n_alloc_tokens;
	free_tokens(&d->prompt);
	d->prompt.tokens = tmp;
	d->prompt.n_tokens = i;
	d->prompt.n_alloc_tokens = j + n_dirs;
	return (SUCCESS);
}
