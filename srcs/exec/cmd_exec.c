/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 17:12:31 by ccarnot           #+#    #+#             */
/*   Updated: 2023/12/18 18:09:18 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/exec.h"

/*int	tab_size(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
		i++;
	return (i);
}

char	**resize_tab(char **tab, int i)
{
	char	**new;
	int		size;
	int		j;

	new = NULL;
	size = tab_size(tab) - 1;
	new = ft_calloc(size + 1, sizeof(char *));
	if (!new)
		return (free_tab(tab), NULL);
//	j = 0;
//	while (j++ <= size) //CHECKER SI ON PEUT S'EN PASSER AVEC LE CALLOC
//		new[j] = 0;
	j = 0;
	while (j < i)
	{
		new[j] = ft_strdup(tab[j]);
		if (!new[j])
			return (free_tab(new), free_tab(tab), NULL);
		j++;
	}
	while (tab[j + 1])
	{
		new[j] = ft_strdup(tab[j + 1]);
		if (!new[j])
			return (free_tab(new), free_tab(tab), NULL);
		j++;
	}
	new[j] = 0; //IDEM
	return (free_tab(tab), new);
}

char	**post_expand_adj(t_ms *ms, char **args, char **tmp)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (args[i + j] && tmp[i])
	{
		if (ft_strlen(args[i + j]) == 0 && ft_strlen(tmp[i]) > 0)
		{
			args = resize_tab(args, i + j);
			if (!args)
			{
				ms->exit_code = 134;
				return (NULL);
			}
			j--;
		}
		i++;
	}
	return (args);
}
*/

t_cmd	*node_to_cmd(t_ms *ms, t_ast *node, char **env)
{
	t_cmd	*cmd;
	t_list	*tmp_d;
	t_list	*tmp_c;

	if (node->dol)
	{
		if (!node->dol->d)
			dprintf(2, "dol->d n'existe pas\n");
//		dprintf(2, "dol = %d\n", node->dol->d->n);
		tmp_d = node->dol->d;
		tmp_c = node->dol->c;
	}
	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->env = NULL;
	cmd->bin_paths = NULL;
	cmd->abs_or_rel = 0;
	cmd->valid_path = 0;
	cmd->args = NULL;
	//	cmd->tmp_args = lst_to_tab(node->args);
	cmd->args = lst_to_tab(node->args);
	//	if (!cmd->tmp_args)
	//		return (free_cmd(cmd), NULL);
	//	cmd->args = tab_cpy(ms, cmd->tmp_args);
	if (!cmd->args)
		return (free_cmd(cmd), NULL);
	if (node->dol)
	{
		if (cmd_expand(ms, cmd->args, node->dol) == 1)
		{
			node->dol->d = tmp_d;
			node->dol->c = tmp_c;
			return (free_cmd(cmd), NULL); // A CHECKER
		}
		node->dol->d = tmp_d;
		node->dol->c = tmp_c;
		//		cmd->args = post_expand_adj(ms, cmd->args, cmd->tmp_args);
		//		if (!cmd->args)
		//			return (free_cmd(cmd), NULL); // A CHECKER
	}
	int	i;
	i = 0;
	while (cmd->args && cmd->args[i] && ft_strlen(cmd->args[i]) == 0)
	{
		free(cmd->args[i]);
		i++;
		if (!cmd->args[i])
		{
			cmd->args = NULL;
			break ;
		}
		cmd->args = &cmd->args[i];
	}
		if (!cmd->args || !cmd->args[0])
			return (cmd);
	//	dprintf(2, "arg[0] exists\n");
	//	dprintf(2, "%s\n", cmd->args[0]);
	if (cmd->args[0][0] == '/' || cmd->args[0][0] == '.')
		abs_rel_path(cmd);
	else
	{
		cmd->builtin = builtin_type_is(cmd->args[0]);
		if (cmd->builtin != NOBUILT)
		{
			cmd->valid_path = 1;
			return (cmd);
		}
		cmd->bin_paths = get_bin_paths(env);
		build_path(cmd);
	}
	return (cmd);
}

int	exec_builtin(t_ms *ms, t_cmd *cmd)
{
	if (cmd->builtin == ECHO)
		return (exec_echo(ms, cmd));
	if (cmd->builtin == CD)
		return (0); //A remplacer par ligne ci-dessous
//		return (exec_cd);
	if (cmd->builtin == PWD)
		return (0); //A remplacer par ligne ci-dessous
//		return (exec_pwd);
	if (cmd->builtin == EXPORT)
		return (0); //A remplacer par ligne ci-dessous
//		return (exec_export);
	if (cmd->builtin == UNSET)
		return (0); //A remplacer par ligne ci-dessous
//		return (exec_unset);
	if (cmd->builtin == ENV)
		return (0); //A remplacer par ligne ci-dessous
//		return (exec_env);
	if (cmd->builtin == EXIT)
		return (exec_exit(ms, cmd));
	return (1);
}

int	do_cmd(t_cmd *cmd, t_ms *ms, char **env)
{
	int		pid;
	t_list	*new_pid;

	pid = fork();
	if (pid == -1)
		return (1);
	else if (pid == 0)
	{
		dprintf(2, "%s\n", cmd->args[0]);
		if (!cmd->valid_path)
		{
			if (cmd->abs_or_rel) //printf mais sur sortie d'erreur ?
				(ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd->args[0], 2), ft_putstr_fd(": No such file or directory\n", 2));
			else
				(ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd->args[0], 2), ft_putstr_fd(": command not found\n", 2));
			free_cmd(cmd);
			exit(127); //A CHECKER
		}
		execve(cmd->args[0], cmd->args, env);
		dprintf(2, "execve fails\n");
		free_cmd(cmd);
		exit(errno); //free minishell ?
	}
	else
	{
		new_pid = ft_lstnew(&pid);
		if (!new_pid)
		{
			ms->exit_code = 134;
			return (1);
		}
		ft_lstadd_back(&ms->pidlst, new_pid);
	}
	return (0);
}

int	exec_cmd(t_ast *node, t_ms *ms)
{
	t_cmd	*cmd;
	char	**env;
	// t_list	*tmp;
	int	exit_code;

	exit_code = 0;
	env = lst_to_tab(ms->env);
	if (!env)
		return (1);
	cmd = node_to_cmd(ms, node, env);
	if (!cmd)
		return (free_tab(env), 1);
	cmd->env = env;
	if (cmd->builtin != NOBUILT)
		exit_code = exec_builtin(ms, cmd);
	else
		exit_code = do_cmd(cmd, ms, env);
	if (exit_code == 1)
		return (free(cmd), 1);//on n'attend pas les children
	// tmp = ms->pidlst;
	// while (tmp)
	// {
	// 	waitpid(tmp->n, NULL, 0);
	// 	tmp = tmp->next;
	// }
	free_cmd(cmd);
//	free_tab(env);
	return (0);
}
