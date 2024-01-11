#include "../../include/exec.h"
#include "../../include/signals.h"

void	update_hdlst(t_ms *ms, char *name, t_cmd *cmd)
{
	t_list	*new;

	if (!ms->hdlst)
	{
		ms->hdlst = ft_calloc(1, sizeof(t_list));
		if (!ms->hdlst)
		{
			free(name);
			free_cmd(cmd);
			free_minishell(ms, 1);
		}
	}
	new = ft_lstnew(name);
	if (!new)
	{
		free(name);
		ft_lstfree(&ms->hdlst);
		free_cmd(cmd);
		free_minishell(ms, 1);		
	}
	ft_lstadd_back(&ms->hdlst, new);
}

char	random_char(t_ms *ms, t_cmd *cmd)
{
    int		fd;
    char	c;
	int		buf;

    fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
	{
        perror("Error opening /dev/urandom");
        free_cmd(cmd);
        free_minishell(ms, 1);// a verifier
    }
    buf = read(fd, &c, sizeof(c));
    if (buf != sizeof(c))
	{
        perror("Error reading from /dev/urandom");
        close(fd);
		free_cmd(cmd);
        free_minishell(ms, 1);// a verifier
    }
    close(fd);
	if (c < 0)
		return (-c);
	// dprintf(2, "c = %d\n", c);
    return (c);
}


char	*generate_hdname(t_ms *ms, t_cmd *cmd)
{
    int		i;
	char	*name; // +1 pour le caractère de fin de chaîne

	i = 0;
	name = ft_calloc(11, sizeof(char));
	if (!name)
	{
		free_cmd(cmd);
		free_minishell(ms, 1);// a verifier
		return (NULL);
	}
	while (i < 10)
	{
		name[i] = (random_char(ms, cmd) + '0') % 26 + 97;
		i++;
	}
	name[i] = '\0';
	update_hdlst(ms, name, cmd);
    return (name);
}
