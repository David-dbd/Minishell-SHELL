#include ".h"

char *recieve_input(char **input)
{
    *input = readline("Minishell>>  "); //Si falla o no falla, retorna y eso se intercepta en el main.
    if (*input == NULL) //si fallo readline hay que revisar si errno esta definido por el sistema. Si fue CTR + D, errno no estara establecido, por lo que entrara y no pasara nada.
		exit_error(&data); //Sempre libera memoria!
    return (*input);
}