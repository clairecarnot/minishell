#include "../../include/exec.h"
#include "../../include/signals.h"

int	handle_dless(t_ms *ms, t_redirs *redirs, t_cmd *cmd)
{
	int	fd;
	char *line;
	char	*hdname;
	int		limlen;
	int		linelen;
	int tmp;

	tmp = 0;
	limlen = ft_strlen(redirs->filename);
	hdname = generate_hdname(ms, cmd);
	// hdname = "name";
	// (void)cmd;
//	dprintf(2, "hdname = %s\n", hdname);
	fd = open(hdname, O_CREAT |  O_RDWR, 0666);
	if (fd < 0)
	{
		perror("minishell: heredoc");
		ms->exit_code = 1;
		return (1);
	}
	dup2(ms->in, STDIN_FILENO); //A PROTEGER
	// dprintf(2, "%d\n", fd);
	hd_signals();
	while (1)
	{
		line = readline("> ");
		if (g_exit_code == 2)
		{
//			open("/dev/stdin", O_RDONLY); //ne marche pas
			open("/dev/stdout", O_RDONLY); //A PROTEGER
			close_if(&fd);
			return (1);
		}
		if (!line)
		{
			perror("minishell: readline");
			ms->exit_code = 1;
			close_if(&fd);
			//unlink("/tmp/here_doc");
			return (1);
		}
		// dprintf(2, "lim = %s\n", redirs->filename);
//		dprintf(2, "line0 = %s\n", line);
		linelen = ft_strlen(line);
		if (ft_strncmp(line, redirs->filename, limlen) == 0 && (linelen == limlen))
		{
//			dprintf(2, "line = %s\n", line);
			free(line);
			tmp = 1;
			// close_if(&fd);
			break ;
		}
		ft_putstr_fd(line, fd);
		if (tmp)
			ft_putstr_fd("\0", fd);
		else
			ft_putstr_fd("\n", fd);
		free(line);
	}

	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 failed");
		ms->exit_code = 1;
		return (1);
	}
	close_if(&fd);// a proteger
	// dprintf(2, "fin handle less\n");
	// dprintf(2, "%d\n", STDIN_FILENO);
	preprompt_signals();
	return (0);
}

int	handle_less(t_ms *ms, t_redirs *redirs)
{
//	dprintf(2, "handle less \n");
	int	fd;

	if (access(redirs->filename, F_OK) != -1)
	{
//		dprintf(2, "access1\n");
		if (access(redirs->filename, R_OK) == -1)
		{
//			dprintf(2, "access2\n");
//			dprintf(2, "can't be opened in read mode\n");
//			fd = open(redirs->filename,  O_RDONLY, 0666);
//			close_if(&fd);
			return (1);
		}
	}
//	while (access(redirs->filename, F_OK) != -1 && access(redirs->filename, R_OK) == -1)
//		sleep(1);
//	fd = open(redirs->filename,  O_RDONLY, S_IRUSR);
//	fd = open(redirs->filename,  O_RDWR, 0666);
	fd = open(redirs->filename,  O_RDONLY, 0666);
	if (fd < 0)
	{
//		ft_putstr_fd("minishell: ", 2);
//		ft_putstr_fd(redirs->filename, 2);
//		ft_putstr_fd(": ", 2);
//		perror("");
		ms->exit_code = 1;
		return (1);
	}
//	dprintf(2, "open less succeded \n");
	// dprintf(2, "%d\n", fd);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 failed");
		ms->exit_code = 1;
		return (1);
	}
	close_if(&fd);// a proteger
	// dprintf(2, "fin handle less\n");
	// dprintf(2, "%d\n", STDIN_FILENO);
	return (0);
}

int	handle_great(t_ms *ms, t_redirs *redirs)
{
//	dprintf(2, "handle great \n");
	int	fd;

	/*
	if (access(redirs->filename, F_OK) != -1)
	{
		dprintf(2, "access3\n");
		if (access(redirs->filename, W_OK) == -1)
		{
			dprintf(2, "access4\n");
//			fd = open(redirs->filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
//			close_if(&fd);
			return (1);
		}
	}
	*/
//	fd = open(redirs->filename, O_CREAT | O_WRONLY | O_TRUNC, S_IWUSR);
	if (access(redirs->filename, F_OK) == -1)
		fd = open(redirs->filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	else
		fd = open(redirs->filename, O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (fd < 0)
	{
//		ft_putstr_fd("minishell: ", 2);
//		ft_putstr_fd(redirs->filename, 2);
//		ft_putstr_fd(": ", 2);
//		perror("");
		ms->exit_code = 1;
		return (1);
	}
//	dprintf(2, "open great succeded \n");
	// dprintf(2, "%d\n", fd);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2 failed");
		ms->exit_code = 1;
		return (1);
	}
	close_if(&fd);// a proteger
	// dprintf(2, "fin handle great\n");
	// dprintf(2, "%d\n", STDIN_FILENO);
	return (0);
}

int	handle_dgreat(t_ms *ms, t_redirs *redirs)
{
	int	fd;

	fd = open(redirs->filename, O_CREAT | O_WRONLY | O_APPEND, 0666);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(redirs->filename, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		ms->exit_code = 1;
		return (1);
	}
	// dprintf(2, "%d\n", fd);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2 failed");
		ms->exit_code = 1;
		return (1);
	}
	close_if(&fd);// a proteger
	// dprintf(2, "fin handle dgreat\n");
	// dprintf(2, "%d\n", STDIN_FILENO);
	return (0);
}

int	cmd_redirs(t_ms *ms, t_ast *node, t_cmd *cmd)
{
	t_redirs	*tmp;

	cmd->redir = 1;
	tmp = node->redirs;
	while (tmp)
	{
		if (tmp->type == LESS)
		{
			if (handle_less(ms, tmp) == 1)
				return (cmd->invalid_io = tmp->filename, 1);
//				return (1);
		}
		else if (tmp->type == GREAT)
		{
			if (handle_great(ms, tmp) == 1)
				return (cmd->invalid_io = tmp->filename, 1);
//				return (1);
		}
		else if (tmp->type == DGREAT)
		{
			if (handle_dgreat(ms, tmp) == 1)
				return (cmd->invalid_io = tmp->filename, 1);
//				return (1);
		}
		else if (tmp->type == DLESS)
		{
			if (handle_dless(ms, tmp, cmd) == 1)
				return (cmd->invalid_io = tmp->filename, 1);
//				return (1);
		}
		tmp = tmp->next_redir;
	}
	cmd->valid_redir = 1;
	// dprintf(2, "fin cmd_redir\n");
	return (0);
}
