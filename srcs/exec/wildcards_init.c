#include "../../include/exec.h"

t_wildcard	*init_wildc(t_wil *wil)
{
	t_wildcard	*wildc;

	wildc = ft_calloc(sizeof(t_wildcard), 1);
	if (!wildc)
		return (NULL);
	wildc->i = 0;
	wildc->size = 0;
	wildc->index = 0;
	wildc->tmp_args = NULL;
	wildc->wil = wil;
	wildc->tab_w = NULL;
	wildc->added_args = 0;
	wildc->new_args = 0;
	wildc->j = -1;
	return (wildc);
}

void	re_init_wildc(t_wildcard *wildc, t_cmd *cmd)
{
	wildc->i = wildc->index;
	wildc->size = ft_strlen(cmd->args[wildc->index]);
	wildc->tmp_args = NULL;
}
