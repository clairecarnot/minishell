#include "../../../include/builtins.h"
#include "../libft/libft.h"

int	exec_pwd(t_ms *ms, t_cmd *cmd)
{
	char	path[1024];

	if (getcwd(path, sizeof(path)) != NULL)
		printf("%s\n", path);
	else
	{
		perror("getcwd() error");
		ms->exit_code = 255;
		free_cmd(cmd);
		free_minishell(ms, 1);
	}
	return (0);
}
