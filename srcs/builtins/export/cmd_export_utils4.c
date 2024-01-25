#include "../../../include/builtins.h"
#include "../libft/libft.h"

int	message_error_exp_1(char *content)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(content, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

int	message_error_exp_2(char *content)
{
	ft_putstr_fd("minishell: export: ", 2);
	ft_putchar_fd(content[0], 2);
	ft_putchar_fd(content[1], 2);
	ft_putstr_fd(": invalid option\n", 2);
	return (1);
}

int	message_error_exp_3(char *content, int i)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(content + i, 2);
	return (ft_putstr_fd(": event not found\n", 2), 1);
}
