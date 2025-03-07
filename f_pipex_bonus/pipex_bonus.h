/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:02:18 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 21:18:40 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/errno.h>
# include <sys/wait.h>
# include <stdio.h>
# include "get_next_line.h"

typedef struct s_data
{
	char			**cmd;
	struct s_data	*next;
	struct s_data	*prev;
	int				place;
	int				**pipe_fd;
	int				fd_in;
	int				fd_out;
}	t_data;

/***free_functions.c*/
void	*free_strs(char **strs);
void	*free_data(t_data *data);
int		free_data_int(t_data *data);
int		free_ptr(void *ptr, int i, char *error);
int		free_data_fd(t_data *data, int i, char *error);

/**utils.c*/
int		ft_strclen(char *str, int c);
char	*ft_strcat(char *str, char *s);
int		count_data(char **args);
int		last_data(t_data *cmd);

/**path.c*/
char	*str_dup_c(char *str, char c_limit, char c_join);
int		count_str_c_limit(char *str, char c_limit);
int		find_path_string(char **env);
char	**split_c(char *str, char c_limit, char c_join);
char	**get_path_env(char **env);

/**parsing.c*/
int		is_flag(char *str);
int		count_flags(char **args, int i);
char	**command_flags(char **args, int *i, int j, int k);
t_data	*new_cmd(t_data *actu, int i);
t_data	*get_commands(char **args);

/**fd_managements.c*/
void	switch_fd_in_out(int fd_a, int fd_b);
int		open_in_out(t_data *data, char **args, int ac);

/**cmd_path.c*/
int		count_flags(char **args, int i);
int		index_path_cmd(char *cmd, char **path);
char	*get_path_cmd(char *cmd, char **path);

/**here_doc.c */
int		here_doc_error(int i);
int		is_here_doc(char *str);
int		is_limiter(char *str, char *limiter);
int		here_doc(char **args);

/**main.c*/
int		pipex(char **args, char **env, int ac);


#endif