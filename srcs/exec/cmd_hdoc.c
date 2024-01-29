#include "../../include/exec.h"

void	hdoc_ctrl_d(t_ms *ms, int nb_line)
{
	ms->exit_code = 0;
	ft_putstr_fd("minishell: warning: here-document at line ", 2);
	ft_putnbr_fd(nb_line, 2);
	ft_putstr_fd(" delimited by end-of-file (wanted `lim')\n", 2);
}

int	hdoc_sigint(int fd)
{
	if (open("/dev/stdout", O_RDONLY) == -1)
		return (close_if(&fd), perror("open stdout failed"), 1);
	return (close_if(&fd), 0);
}

int	hdoc_create(t_ms *ms, int fd, char *filename, int limlen)
{
	char	*line;
	int		nb_line;

	nb_line = 0;
	while (1)
	{
		line = readline("> ");
		if (g_exit_code == 2)
			return (free_if(line), hdoc_sigint(fd));
		if (!line)
		{
			hdoc_ctrl_d(ms, nb_line);
			break ;
		}
		if (ft_strncmp(line, filename, limlen) == 0
			&& ((int) ft_strlen(line) == limlen))
			break ;
		line = expand_hdoc(ms, line);
		if (ms->exit_code == 255)
			return (free_if(line), close_if(&fd), 255);
		(ft_putstr_fd(line, fd), ft_putstr_fd("\n", fd), free(line));
		nb_line++;
	}
	return (free(line), ft_putstr_fd("\0", fd), close_if(&fd), 0);
}

char	*handle_dless(t_ms *ms, t_redirs *redirs, char *limiter)
{
	char	*hdname;
	int		fd;
	int		limlen;
	int		exit_code;

	exit_code = 0;
	limlen = ft_strlen(redirs->filename);
	hdname = generate_hdname(ms);
	fd = open(hdname, O_CREAT | O_WRONLY, 0666);
	if (fd < 0)
		return (perror("minishell: heredoc"), ms->exit_code = 1,
			free(limiter), NULL);
	if (dup2(ms->in, STDIN_FILENO) == -1)
		return (perror("dup2 failed"), ms->exit_code = 1, close_if(&fd),
			free(limiter), NULL);
	hd_signals();
	exit_code = hdoc_create(ms, fd, redirs->filename, limlen);
	if (g_exit_code == 2 && exit_code == 0)
		return (free(limiter), NULL);
	else if ((g_exit_code == 2 && exit_code == 1) || exit_code == 255)
		(free_minishell(ms, exit_code));
	(preprompt_signals(), free(limiter));
	return (ft_strdup(hdname));
}

int	open_heredocs(t_ms *ms, t_ast *node)
{
	t_redirs	*tmp;

	if (!node)
		return (0);
	tmp = node->redirs;
	if (open_heredocs(ms, node->left) == 1)
		return (1);
	while (tmp)
	{
		if (tmp->type == DLESS)
		{
			tmp->filename = handle_dless(ms, tmp, tmp->filename);
			if (!tmp->filename)
				return (1);
		}
		tmp = tmp->next_redir;
	}
	if (open_heredocs(ms, node->right) == 1)
		return (1);
	return (0);
}
