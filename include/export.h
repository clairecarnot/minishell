/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapoirie <mapoirie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 16:41:09 by mapoirie          #+#    #+#             */
/*   Updated: 2023/11/20 10:35:26 by mapoirie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPORT_H
# define EXPORT_H

# include "lexer.h"
# include "parser.h"
# include "struct.h"
# include "export.h"

//----------------------- export.c -----------------------
int		check_error_exp(char *content);
void	print_lst_exp(t_list *exp);
int		exec_export(t_ms *ms);


#endif