#include "../../include/exec.h"

int	exec_pipeline(t_ast *node, t_ms *ms)
{
	if (!node)
		return (0);
	if (node->type == PIPE)
	{
		if (exec_pipeline(node->left, ms))
			return (1);
		if (exec_pipeline(node->right, ms))
			return (1);
	}
	else if (node ->type == CMD)
	{
	}
	return (0);
}
