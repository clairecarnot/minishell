/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_utils4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 17:26:32 by mapoirie          #+#    #+#             */
/*   Updated: 2024/02/01 17:26:33 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/builtin.h"
#include "../libft/libft.h"

int	message_error_exp_1(t_ms *ms, char *content)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(content, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	ms->exit_code = 1;
	return (1);
}

int	message_error_exp_2(t_ms *ms, char *content)
{
	ft_putstr_fd("minishell: export: ", 2);
	ft_putchar_fd(content[0], 2);
	ft_putchar_fd(content[1], 2);
	ft_putstr_fd(": invalid option\n", 2);
	ms->exit_code = 2;
	return (2);
}
