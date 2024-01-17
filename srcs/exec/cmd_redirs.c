#include "../../include/exec.h"
#include "../../include/signals.h"

/*
int	create_outfiles(t_ms *ms, t_redirs *redirs)
{
	int	fd;

	if (access(redirs->filename, F_OK) == -1)
		fd = open(redirs->filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	else
		fd = open(redirs->filename, O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (fd < 0)
	{
		ms->exit_code = 1;
		return (1);
	}
	return (0);
}*/

char	*handle_dless(t_ms *ms, t_redirs *redirs, char *limiter)
{
	int	nb_line;
	int	fd;
	char *line;
	char	*hdname;
	int		limlen;
	int		linelen;

	nb_line = 0;
	limlen = ft_strlen(redirs->filename);
	hdname = generate_hdname(ms); //deja protege
	// hdname = "name";
	// (void)cmd;
//	dprintf(2, "hdname = %s\n", hdname);
//	fd = open(hdname, O_CREAT |  O_RDWR, 0666);
	fd = open(hdname, O_CREAT |  O_WRONLY, 0666);
	if (fd < 0)
	{
		perror("minishell: heredoc");
		ms->exit_code = 1;
		//return (1);
		return (NULL);
	}
	dup2(ms->in, STDIN_FILENO); //A PROTEGER// pour pouvoir ecrire dans le bon terminal
				    // dprintf(2, "%d\n", fd);
	hd_signals();
	while (1)
	{
//		ft_putstr_fd("> ", 1);
//		line = get_next_line(0, 0);
		line = readline("> ");
		if (g_exit_code == 2)
		{
			open("/dev/stdout", O_RDONLY); //A PROTEGER
			close_if(&fd);
			return (NULL);
			//return (1);
		}
		if (!line)
		{
			// perror("minishell: readline");
			ms->exit_code = 0;
			close_if(&fd);
			//unlink("/tmp/here_doc");
			ft_putstr_fd("minishell: warning: here-document at line ", 2);
			ft_putnbr_fd(nb_line, 2);
			ft_putstr_fd(" delimited by end-of-file (wanted `lim')\n", 2);
			//ajouter les lignes lues precedemment
			//+ ne pas afficher le message minishell: lim: No such file or directory
			unlink("/tmp/here_doc");
			return (NULL);
		}
		// dprintf(2, "lim = %s\n", redirs->filename);
		//		dprintf(2, "line0 = %s\n", line);
		linelen = ft_strlen(line);
		if (ft_strncmp(line, redirs->filename, limlen) == 0 && (linelen == limlen))
		{
			//			dprintf(2, "line = %s\n", line);
			free(line);
//			line = get_next_line(0, 1);
			// close_if(&fd);
			break ;
		}
//		dprintf(2, "line = %s\n", line);
		redirs->filename = expand_redir(ms, redirs->filename); // REPRENDRE ICI
		ft_putstr_fd(line, fd); //?
		ft_putstr_fd("\n", fd);
		free(line);
		nb_line++;
	}
	ft_putstr_fd("\0", fd);
	/*
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2 failed");
		ms->exit_code = 1;
		return (1);
	}
	*/
	close_if(&fd);// a proteger
      // dprintf(2, "fin handle less\n");
      // dprintf(2, "%d\n", STDIN_FILENO);
	preprompt_signals(); // A MODIFIER SI CHILD
	free(limiter);
	return (ft_strdup(hdname));
}

int	handle_less(t_ms *ms, t_redirs *redirs)
{
	int	fd;
	t_list	*rtmp_d;
	t_list	*rtmp_c;

	if (redirs->dol)
	{
		rtmp_d = redirs->dol->d;
		rtmp_c = redirs->dol->c;
	}
	if (redirs->type == LESS && redirs->dol)
	{
		redirs->filename = expand_redir(ms, redirs->filename, redirs->dol);
		redirs->dol->d = rtmp_d;
		redirs->dol->c = rtmp_c;
	}
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
	t_list	*rtmp_d;
	t_list	*rtmp_c;

	if (redirs->dol)
	{
		rtmp_d = redirs->dol->d;
		rtmp_c = redirs->dol->c;
	}
	if (redirs->type == LESS && redirs->dol)
	{
		redirs->filename = expand_redir(ms, redirs->filename, redirs->dol);
		redirs->dol->d = rtmp_d;
		redirs->dol->c = rtmp_c;
	}
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
//	dprintf(2, "fin handle great\n");
	// dprintf(2, "%d\n", STDIN_FILENO);
	return (0);
}

int	handle_dgreat(t_ms *ms, t_redirs *redirs)
{
	int	fd;
	t_list	*rtmp_d;                                             
	t_list	*rtmp_c;

	if (redirs->dol)
	{
		rtmp_d = redirs->dol->d;
		rtmp_c = redirs->dol->c;
	}
	if (redirs->type == LESS && redirs->dol)
	{
		redirs->filename = expand_redir(ms, redirs->filename, redirs->dol);
		redirs->dol->d = rtmp_d;
		redirs->dol->c = rtmp_c;
	}
	if (access(redirs->filename, F_OK) == -1)
		fd = open(redirs->filename, O_CREAT | O_WRONLY | O_APPEND, 0666);
	else
		fd = open(redirs->filename, O_CREAT | O_RDWR | O_APPEND, 0666);
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
	// dprintf(2, "cmd redirs\n");
	t_redirs	*tmp;

	cmd->redir = 1;
	/*
	tmp = node->redirs;
	while (tmp)
	{
		if (tmp->type == DLESS)
		{
			tmp->filename = handle_dless(ms, tmp, cmd, tmp->filename);
			if (!tmp->filename)
				return (1);
		}
		tmp = tmp->next_redir;
	}
	*/
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
			if (handle_less(ms, tmp) == 1)
//			if (handle_dless(ms, tmp, cmd) == 1)
				return (cmd->invalid_io = tmp->filename, 1);
//				return (1);
		}
		tmp = tmp->next_redir;
	}
	cmd->valid_redir = 1;
	// dprintf(2, "fin cmd_redir\n");
	return (0);
}
