#include "../../include/exec.h"

/*
void	init_pipe(t_ms *ms, t_ast *node)
{
	node->pipe = ft_calloc(2, sizeof(int));
	if (!node)
	{
		ms->exit_code = 134;
		free_minishell(ms, 1);
	}
	node->pipe[0] = -1;
	node->pipe[1] = -1;
	if (pipe(node->pipe) == -1)
	{
		ms->exit_code = 134;//QUEL CODE ERROR?
		free_minishell(ms, 1);
	}
}

int	exec_pipeline(t_ast *node, t_ms *ms)
{
	if (!node)
		return (0);
	if (node->type == PIPE)
	{
		init_pipe(ms, node);
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
*/

int	create_pipeline(t_ast *node, t_ms *ms, t_list *pipeline)
{
	t_list	*new_cmd;
	t_ast	*node_content;

	if (node->type == PIPE)
	{
		if (create_pipeline(node->left, ms, pipeline))
			return (1);
		if (create_pipeline(node->right, ms, pipeline))
			return (1);
	}
	if (node->type == CMD)
	{
		node_content = ft_calloc(1, sizeof(t_ast));
		if (!node_content)
		{
			ms->exit_code = 255;
			return (ft_lstfree(pipeline), 1);
		}
		new_cmd = ft_lstnew(node_content);
		if (!new_cmd)
		{
			ms->exit_code = 255;
			return (free(node_content), ft_lstfree(pipeline), 1);
		}
		ft_lstadd_back(&pipeline, new_cmd);
	}
}

int	exec_pipeline(t_ast *node, t_ms *ms)
{
	t_list	*pipeline;

	pipeline = NULL;
	if (create_pipeline(node, ms, pipeline))
		return (1);
	if (ft_pipex(pipeline))
		return (ft_lstfree(pipeline), 1);
	ft_lstfree(pipeline);
	return (0);
}
