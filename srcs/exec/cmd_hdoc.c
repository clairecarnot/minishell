#include "../../include/exec.h"
#include "../../include/signals.h"

int	hdoc_ctrl_d(t_ms *ms, int fd, int nb_line) // A VOIR AVEC MAELLE
{
	ms->exit_code = 0;
	ft_putstr_fd("minishell: warning: here-document at line ", 2);
	ft_putnbr_fd(nb_line, 2);
	ft_putstr_fd(" delimited by end-of-file (wanted `lim')\n", 2);
	//ajouter les lignes lues precedemment
	//+ ne pas afficher minishell: lim: No such file or directory
	return (close_if(&fd), 1); //check error code
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
		if (!line) //A VOIR AVEC MAELLE
			return (free_if(line), hdoc_ctrl_d(ms, fd, nb_line));
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
	hdname = generate_hdname(ms); //deja protege
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
//		(free(limiter), free_minishell(ms, exit_code));
	(preprompt_signals(), free(limiter));
	return (ft_strdup(hdname));
}
