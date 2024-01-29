#include "../../include/exec.h"

void	print_msg_redir(char *invalid_io)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(invalid_io, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

int	handle_less(t_ms *ms, t_redirs *redirs)
{
	int		fd;
	t_list	*rtmp_d;
	t_list	*rtmp_c;

	if (redirs->dol)
		save_ptrs(&redirs->dol->d, &redirs->dol->c, &rtmp_d, &rtmp_c);
	if (redirs->type == LESS && redirs->dol)
	{
		redirs->filename = expand_redir(ms, redirs->filename, redirs->dol);
		redirs->dol->d = rtmp_d;
		redirs->dol->c = rtmp_c;
		if (ms->exit_code == 255)
			return (1);
	}
	fd = open(redirs->filename, O_RDONLY, 0666);
	if (fd < 0)
		return (ms->exit_code = 1, print_msg_redir(redirs->filename), 1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (close_if(&fd), perror("minishell: dup2 failed"),
			ms->exit_code = 1, 1);
	return (close_if(&fd), 0);
}

int	handle_great(t_ms *ms, t_redirs *redirs)
{
	int		fd;
	t_list	*rtmp_d;
	t_list	*rtmp_c;

	if (redirs->dol)
		save_ptrs(&redirs->dol->d, &redirs->dol->c, &rtmp_d, &rtmp_c);
	if (redirs->type == GREAT && redirs->dol)
	{
		redirs->filename = expand_redir(ms, redirs->filename, redirs->dol);
		redirs->dol->d = rtmp_d;
		redirs->dol->c = rtmp_c;
		if (ms->exit_code == 255)
			return (1);
	}
	fd = open(redirs->filename, O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (fd < 0)
		return (ms->exit_code = 1, print_msg_redir(redirs->filename), 1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (close_if(&fd), perror("minishell: dup2 failed"),
			ms->exit_code = 1, 1);
	return (close_if(&fd), 0);
}

int	handle_dgreat(t_ms *ms, t_redirs *redirs)
{
	int		fd;
	t_list	*rtmp_d;
	t_list	*rtmp_c;

	if (redirs->dol)
		save_ptrs(&redirs->dol->d, &redirs->dol->c, &rtmp_d, &rtmp_c);
	if (redirs->type == DGREAT && redirs->dol)
	{
		redirs->filename = expand_redir(ms, redirs->filename, redirs->dol);
		redirs->dol->d = rtmp_d;
		redirs->dol->c = rtmp_c;
		if (ms->exit_code == 255)
			return (1);
	}
	fd = open(redirs->filename, O_CREAT | O_RDWR | O_APPEND, 0666);
	if (fd < 0)
		return (ms->exit_code = 1, print_msg_redir(redirs->filename), 1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (close_if(&fd), perror("minishell: dup2 failed"),
			ms->exit_code = 1, 1);
	return (close_if(&fd), 0);
}

int	cmd_redirs(t_ms *ms, t_ast *node, t_cmd *cmd)
{
	t_redirs	*tmp;

	cmd->redir = 1;
	tmp = node->redirs;
	while (tmp)
	{
		if (tmp->type == LESS || tmp->type == DLESS)
		{
			if (handle_less(ms, tmp) == 1)
				return (cmd->invalid_io = tmp->filename, 1);
		}
		else if (tmp->type == GREAT)
		{
			if (handle_great(ms, tmp) == 1)
				return (cmd->invalid_io = tmp->filename, 1);
		}
		else if (tmp->type == DGREAT)
		{
			if (handle_dgreat(ms, tmp) == 1)
				return (cmd->invalid_io = tmp->filename, 1);
		}
		tmp = tmp->next_redir;
	}
	cmd->valid_redir = 1;
	return (0);
}
