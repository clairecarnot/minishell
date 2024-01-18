#include "../../include/exec.h"
#include "../../include/signals.h"

void	update_hdlst(t_ms *ms, char *name)
{
	t_list	*new;

	if (!ms->hdlst)
	{
		ms->hdlst = ft_calloc(1, sizeof(t_list));
		if (!ms->hdlst)
		{
			free(name);
			free_minishell(ms, 255);// a verifier
		}
	}
	new = ft_lstnew(name);
	if (!new)
	{
		free(name);
		ft_lstfree(&ms->hdlst);
		free_minishell(ms, 255);// a verifier
	}
	ft_lstadd_back(&ms->hdlst, new);
}

char	random_char(t_ms *ms)
{
	char	c;
	int		fd;
	int		buf;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
	{
		perror("Error opening /dev/urandom");
		free_minishell(ms, 1);// a verifier
	}
	buf = read(fd, &c, sizeof(c));
	if (buf != sizeof(c))
	{
		perror("Error reading from /dev/urandom");
		close_if(&fd);
		free_minishell(ms, 1);// a verifier
	}
	close_if(&fd);
	if (c < 0)
		return (-c);
	return (c);
}

char	*generate_hdname(t_ms *ms)
{
	int		i;
	char	*name;

	i = 0;
	name = ft_calloc(11 + 6, sizeof(char));
	if (!name)
		free_minishell(ms, 255);// a verifier
	ft_strlcpy(name, "/tmp/.", 7);
	while (i < 10)
	{
		name[6 + i] = (random_char(ms) + '0') % 26 + 97;
		i++;
	}
	name[6 + i] = '\0';
	update_hdlst(ms, name);
	return (name);
}
