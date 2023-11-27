#include "../../include/exec.h"

int	exec_andif(t_ast *node, t_ms *ms)
{
	if (execute(node->left, ms))
		return (1);
	if (execute(node->right, ms))
		return (1);
	return (0);
}

int	exec_orif(t_ast *node, t_ms *ms)
{
	if (execute(node->left, ms))
	{
		if (execute(node->right, ms))
			return (1);
	}
	return (0);
}

int	execute(t_ast *node, t_ms *ms)
{
	//init pipes?
	if (!node)
		return (0);
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
	if (ms->root->type == PIPE)
	{
		if (exec_pipeline(node, ms) == 0)
			return (0);
	}
	return (1);
}

int	pre_exec(t_ms *ms)
{
	int	exit_code;

	//handle save STDIN STDOUT
	//handle signals
	exit_code = execute(ms->root, ms);
	//clear exec
	return (exit_code);
}
