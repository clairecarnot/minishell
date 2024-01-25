#include "../../../include/builtins.h"
#include "../libft/libft.h"

int	exec_pwd(t_ms *ms, t_cmd *cmd)
{
	char	path[1024];

	(void)cmd;
	if (getcwd(path, sizeof(path)) != NULL)
		printf("%s\n", path);
	else
	{
		ms->exit_code = 1;
		ft_putstr_fd("pwd: error retrieving current directory: \
getcwd: cannot access parent directories: No such file or directory\n", 2);
		return (1);
	}
	ms->exit_code = 0;
	return (0);
}
