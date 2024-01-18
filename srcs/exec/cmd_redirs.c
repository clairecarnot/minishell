#include "../../include/exec.h"
#include "../../include/signals.h"

char	*handle_dless(t_ms *ms, t_redirs *redirs, char *limiter)
{
	char	*line;
	char	*hdname;
	int		nb_line;
	int		fd;
	int		limlen;

	nb_line = 0;
	limlen = ft_strlen(redirs->filename);
	hdname = generate_hdname(ms); //deja protege
	fd = open(hdname, O_CREAT | O_WRONLY, 0666);
	if (fd < 0)
	{
		perror("minishell: heredoc");
		ms->exit_code = 1;
		return (NULL);
	}
	if (dup2(ms->in, STDIN_FILENO) == -1)
		(perror("dup2 failed"), free_minishell(ms, 1)); //a verifier
	hd_signals();
	while (1)
	{
		line = readline("> ");
		if (g_exit_code == 2)
		{
			if (open("/dev/stdout", O_RDONLY) == -1)
				(perror("open failed"), free_minishell(ms, 1));
			close_if(&fd);
			return (NULL);
		}
		if (!line) //A voir avec Maelle
		{
			ms->exit_code = 0;
			close_if(&fd);
			ft_putstr_fd("minishell: warning: here-document at line ", 2);
			ft_putnbr_fd(nb_line, 2);
			ft_putstr_fd(" delimited by end-of-file (wanted `lim')\n", 2);
			//ajouter les lignes lues precedemment
			//+ ne pas afficher le message minishell: lim: No such file or directory
			//unlink("/tmp/here_doc"); => ne correspond a rien
			return (NULL);
		}
		if (ft_strncmp(line, redirs->filename, limlen) == 0
			&& ((int) ft_strlen(line) == limlen))
			break ;
		line = expand_hdoc(ms, line);
		(ft_putstr_fd(line, fd), ft_putstr_fd("\n", fd), free(line));
		nb_line++;
	}
	free(line);
	ft_putstr_fd("\0", fd);
	close_if(&fd);// a proteger
	(preprompt_signals(), free(limiter));
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
