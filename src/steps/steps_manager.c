
#include ".h"
/*Here we simply manage all the key functions/steps of the program.
We first tokenize, then we'll do the expantionthe we create 
the tree, in this particular case, will use the ast (syntaxis tree). 
To conlcude we use execute_recursive to go through all the 
nodes by suing recursion and execute the commands*/

int steps_manager(t_shell *data, char *input);
{
	if (!tokenizer(data, input))
		return (error(data));
	if (!expanstion(data->tokens, data->envp))
		return (error(data))
	if (!ast(data, data->tokens))
		return (error(data));
	if (!execute_recursive(data, data.ast))
		return (error(data));
	clean_data(data);
}