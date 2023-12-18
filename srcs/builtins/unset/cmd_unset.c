#include "../../../include/builtins.h"
#include "../libft/libft.h"

void	del_var_exp(t_ms *ms, char *content)
{
	t_list	*cur;
	t_list	*prev;

	cur = ms->exp;
	prev = NULL;
	while (cur && (ft_strncmp(cur->content, content, slen_equal(cur->content)) \
	|| ft_strncmp(cur->content, content, ft_strlen(content))))
	{
		prev = cur;
		cur = cur->next;
	}
	if (cur == NULL)
		return ;
	if (prev != NULL)
		prev->next = cur->next;
	else
		ms->exp = cur->next;
	free(cur->content);
	free(cur);
}

void	del_var_env(t_ms *ms, char *content)
{
	t_list	*cur;
	t_list	*prev;

	cur = ms->env;
	prev = NULL;
	while (cur && (ft_strncmp(cur->content, content, slen_equal(cur->content)) \
	|| ft_strncmp(cur->content, content, ft_strlen(content))))
	{
		prev = cur;
		cur = cur->next;
	}
	if (cur == NULL)
		return ;
	if (prev != NULL)
		prev->next = cur->next;
	else
		ms->env = cur->next;
	free(cur->content);
	free(cur);
}

int	exec_unset(t_ms *ms, t_ast *node)
{
	t_list	*unset_arg;

	unset_arg = node->args;
	if (ft_strncmp(unset_arg->content, "unset", 6) == 0)
	{
		if (!unset_arg->next)
			return (0);
		else
		{
			unset_arg = unset_arg->next;
			while (unset_arg)
			{

				del_var_env(ms, unset_arg->content);
				del_var_exp(ms, unset_arg->content);
				if (!unset_arg->next)
					return (0);
				else
					unset_arg = unset_arg->next;
			}
		}
	}
	return (0);
}
