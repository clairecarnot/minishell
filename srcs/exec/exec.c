#include "../../include/exec.h"

int	execute(t_ast *node, t_ms *ms)
{
	//init pipes?
	if (ms->root->type == CMD)
	{
		if (exec_cmd(node, ms) == 0)
			return (0);
	}
	if (ms->root->type == AND_IF)
	{
		if (exec_andif(node, ms) == 0)
			return (0);
	}
	if (ms->root->type == OR_IF)
	{
		if (exec_orif(node, ms) == 0)
			return (0);
	}
	if (ms->root->type == PIPE);
	{
		if (exec_pipeline(node, ms) == 0)
			return (0);
	}
}

int	pre_exec(t_ms *ms)
{
	//handle save STDIN STDOUT
	//handle signals
	execute(ms->root, ms);
	//clear exec
	return (0);
}
