#include ".h"

/*Here we simply set the behaviour for when we get either SIGINT or SIGQUIT. 
We use the rl functions from the gnu library to manage the line creation.
We also set the global variable to CLEAN. We'll check on every key point
if the global variable global_flag has been set to CLEAn in order to of course clean*/

void signal_handler(int sig)
{
	if (signal == SIGINT)
	{
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
		global_flag = CLEAN;
	}
    else if (signal == SIGQUIT) //do nothing
        continue ;
	return ;
}