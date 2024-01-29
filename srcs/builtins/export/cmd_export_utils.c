#include "../../../include/builtin.h"
#include "../libft/libft.h"

/*
Error if the content is empty ou est un special char
Check that the 1rst character of the variable name is a letter (upper or lower)
Check that there's not a special character in the entire name (before the '=')
*/
int	error_exp(t_ms *ms, char *content)
{
	int	i;

	i = -1;
	if (!content[0])
		return (ft_putstr_fd("minishell: export: `': \
not a valid identifier\n", 2), 1);
	if (content[0] && content[0] == '-' && content[1])
		return (message_error_exp_2(ms, content));
	if (content[0] && (content[0] < 'A' || (content[0] > 'Z' && \
	content[0] < 'a') || \
	content[0] > 'z') && content[0] != '_' && content[0] != '!')
		return (message_error_exp_1(ms, content));
	while (content[++i] && content[i] != '=')
	{
		if (((content[i] < '0' || (content[i] > '9' && \
		content[i] < 'A') || (content[i] > 'Z' && content[i] < 'a') || \
		content[i] > 'z') && content[i] != '_' && content[i] != '+') || \
		(content[i] == '+' && content[i + 1] && \
		content[i + 1] != '=') || (content[i] == '+' && !content[i + 1]))
			return (message_error_exp_1(ms, content));
	}
	return (0);
}

/*
Check si le character avant '=' est un ' '
*/
int	error_exp_spaces(t_ms *ms, char *content)
{
	int	i;

	i = 0;
	while (content[i] && content[i] != '=')
		i++;
	if (i != 0 && content[i - 1] == ' ')
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(content, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		ms->exit_code = 1;
		return (1);
	}
	return (0);
}

/*
Cas ou il n'y a pas de '=', nom de variable seulement ajoutee a exp 
dans l'ordre ASCII
*/
int	has_equal(char *content)
{
	int	i;

	i = 0;
	while (content[i])
	{
		if (content[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

/*
Ajoute les quotes autour du contenu de la variable pour l'ajout a exp
*/
char	*add_qvar(char *content, int i)
{
	int		j;
	char	*cpy_content;

	j = 0;
	cpy_content = NULL;
	if (!has_equal(content))
		return (content);
	cpy_content = malloc(ft_strlen(content) + 3 * sizeof(char));
	if (!cpy_content)
		return (NULL);
	while (content[i] && content[i] != '=')
		cpy_content[j++] = content[i++];
	i++;
	cpy_content[j++] = '=';
	cpy_content[j++] = '\"';
	while (content[i])
		cpy_content[j++] = content[i++];
	cpy_content[j++] = '\"';
	cpy_content[j] = '\0';
	return (cpy_content);
}

void	add_qvar_lst(t_ms *ms, t_list *exp)
{
	char	*tmp_content;
	t_list	*tmp_exp;

	tmp_exp = exp;
	while (tmp_exp)
	{
		tmp_content = tmp_exp->content;
		tmp_exp->content = add_qvar(tmp_content, 0);// c'est verifie
		if (!tmp_exp->content)
		{	
			free(tmp_content);
			prefree_minishell(ms, NULL);
		}
		free(tmp_content);
		tmp_exp = tmp_exp->next;
	}
}
