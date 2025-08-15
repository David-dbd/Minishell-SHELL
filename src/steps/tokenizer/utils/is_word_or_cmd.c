#include "../../../../inc/minishell.h"

//HAY UNA FUNCION PARA FREE ARR
static void ft_free_split(char **path_arr)
{
    int i;

    i = 0;
    while (path_arr[i] != NULL)
    {
        free (path_arr[i]);
        i++;
    }
    free (path_arr);
    path_arr = NULL;
}

static void set_free(char **path_arr, char **path_slash, char **path)
{
    ft_free_split(path_arr);//change this for libft function
    if (*path_slash)
        free (*path_slash);
    *path_slash = NULL;
    if (*path)
        free (*path);
    *path = NULL;
}

static int check_access(char **path_arr, char *path_slash, char *path)
{
    int validate;

    validate = access(path, F_OK);
    if (validate == 0)
    {
        set_free(path_arr, &path_slash, &path);
        return (1);
    }
    return (0);
}

static int find_binary(char **path_arr, char *path_slash, char *path, char *word)
{
    int i;

    i = 0;
    while (path_arr[i] != NULL)
    {
        path_slash = ft_strjoin(path_arr[i], "/");
        if (!path_slash && i == 0) //liberar path y path_slash y path_arr si i > 0
            return (-1);
        else if (!path_slash && i > 0)
        {
            set_free(path_arr, &path_slash, &path);
            return (-1);
        }
        path = ft_strjoin(path_slash, word);
        if (!path)
        {
            set_free(path_arr, &path_slash, &path);
            return (-1);
        }
        if (check_access(path_arr, path_slash, path) == 1)
            return (1);
        i++;
    }
    set_free(path_arr, &path_slash, &path);
    return (0);
}

static int compare_built_ins(t_prompt *prompt, t_token *tokens, char *str)
{
    
    char *built_in[] = {"cd", "echo", "export", "unset", "env", "exit", "pwd", NULL};
    int i;

    i = 0;
    while (built_in[i] != NULL)
    {
        if (ft_strcmp(built_in[i], str) == 0)
        {
            tokens->type = COMMAND;
            prompt->n_cmds++;
            return (1);
        }
        i++;
    }
    return (0);
}

int is_word_or_cmd(t_shell *data, t_prompt *prompt, t_token *tokens, char *str)
{
    char **path_arr;
    char *path_slash;
    char *path;
    int validate;

    path_slash = NULL;
    path = getenv("PATH");
    if (compare_built_ins(prompt, tokens, str) == 1)
        return (0);
    path_arr = ft_split(path, ':');
    if (!path_arr)
        exit_error(data, ERR_MALLOC, EXIT_FAILURE);
    validate = find_binary(path_arr, path_slash, path, str);
    if (validate == 1)
    {
        tokens->type = COMMAND;
        prompt->n_cmds++;
        return (0);
    }
    else if (validate == -1)
        exit_error(data, ERR_MALLOC, EXIT_FAILURE);
    return (0);
}
