#include "../../include/env.h"

void	add_pwd(t_ms *ms)
{
	char	path[1024];
	char	*pwd;
	char	*wd;
	t_list	*new_var1;

	wd = getcwd(path, sizeof(path));
	if (!wd)
		free_minishell(ms, 255);
	pwd = ft_strjoin("PWD=", wd);
	if (!pwd)
		free_minishell(ms, 255);
	new_var1 = ft_lstnew(pwd);
	if (!new_var1)
	{
		free(pwd);
		free_minishell(ms, 255);
	}
	ft_lstadd_back(&ms->env, new_var1);
}

void	add_shlvl(t_ms *ms)
{
	char	*shlvl;
	t_list	*new_var2;

	shlvl = ft_strdup("SHLVL=0");
	if (!shlvl)
		free_minishell(ms, 255);
	new_var2 = ft_lstnew(shlvl);
	if (!new_var2)
	{
		free(shlvl);
		free_minishell(ms, 255);
	}
	ft_lstadd_back(&ms->env, new_var2);
}

void	add_underscore(t_ms *ms)
{
	char	*underscore;
	t_list	*new_var3;

	underscore = ft_strdup("_=/usr/bin/");
	if (!underscore)
		free_minishell(ms, 255);
	new_var3 = ft_lstnew(underscore);
	if (!new_var3)
	{
		free(underscore);
		free_minishell(ms, 255);
	}
	ft_lstadd_back(&ms->env, new_var3);
}

int	find_in_lst(t_list *env, char *var)
{
	t_list	*tmp_env;

	tmp_env = env;
	while (tmp_env)
	{
		if (!ft_strncmp(tmp_env->content, var, ft_strlen(var)))
			return (1);
		tmp_env = tmp_env->next;
	}
	return (0);
}

void	check_noenv(t_ms *ms)
{
	if (!find_in_lst(ms->env, "PWD="))
		add_pwd(ms);
	if (!find_in_lst(ms->env, "SHLVL="))
		add_shlvl(ms);
	if (!find_in_lst(ms->env, "_="))
		add_underscore(ms);
}
