#include "../../include/exec.h"

int	execute(t_ms *ms)
{
	if (!ms->root)
		return (1); //inutile ?
	if (ms->root->type == CMD)
		return (exec_cmd(ms));
	if (ms->root->type == PIPE);
		return (exec_pipe(ms));
	if (ms->root->type == AND_IF)
		return (exec_andif(ms));
	if (ms->root->type == OR_IF)
		return (exec_orif(ms));
}

int	is_builtin(t_list *args)
{
	char	*cmd;

	cmd = (char*)args->content;
	if (ft_strncmp("echo", cmd, 4) == 0)//option -n ?
		return (exec_echo(ms));
	if (ft_strncmp("cd", cmd, 2) == 0)//rel or abs path only
		return (exec_cd(ms));
	if (ft_strncmp("pwd", cmd, 3) == 0)
		return (exec_pwd(ms));
	if (ft_strncmp("export", cmd, 6) == 0)
		return (exec_export(ms));
	if (ft_strncmp("unset", cmd, 5) == 0)
		return (exec_unset(ms));
	if (ft_strncmp("env", cmd, 3) == 0)
		return (exec_env(ms));
	if (ft_strncmp("exit", cmd, 6) == 0)
		return (exec_exit(ms));
	return (0);
}

/* si node->type == CMD
  renvoie l'exit code
*/
int	exec_cmd(t_ms *ms)
{
	int	ret;

	ret = CMD_NOT_FOUND;
	ret = is_builtin(ms->root->args);
	if (ret != CMD_NOT_FOUND)
		return (ret);
	else
	{
		fork();
		child-> exec;
		parent-> waipid;
	}
}

/* si node->type == PIPE
  renvoie 1 ou 0 si bien execute ?
*/

/* si node->type == && ou ||
  renvoie 1 ou 0 si bien execute ?
*/
