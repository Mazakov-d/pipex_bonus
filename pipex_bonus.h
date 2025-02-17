/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:02:18 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/02/16 14:34:09 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/errno.h>
#include <stdio.h>

typedef struct s_cmd
{
	char	**cmd;
}	t_cmd;

//free_functions.c
int		fd_is_valid(int fd);
void	*free_strs(char **strs);
int		free_cmds_fd(t_cmd *cmds, int pipe_fd[2], int i);
int		free_ptr(void *ptr, int i);

//utils.c
int		ft_strclen(char *str, int c);
char	*ft_strndup(char *str, int n);
char	*ft_strcat(char *str, char *s);
int		count_cmd(char **args);

//path.c
char	*str_dup_c(char *str, char c_limit, char c_join);
int		count_str_c_limit(char *str, char c_limit);
int		find_path_string(char **env);
char	**split_c(char *str, char c_limit, char c_join);
char	**get_path_env(char **env);

//parsing.c
int		is_flag(char *str);
int		count_flags(char **args, int i);
char	**command_flags(char **args, int *i, int j, int k);
t_cmd	*get_commands(char **args);

//switch_fd.c
int		open_switch_stdin(char *file);
int		open_switch_stdout(char *file);
void	switch_fd_in_out(int fd_a, int fd_b);

//executing.c
int		cmd_infile(char **cmd_a, char **env, char *infile, int pipe_fd[2]);
int		cmd_outfile(char **cmd_b, char **env, char *outfile, int pipe_fd[2]);
int		cmd_to_pipe(char **cmd, char **env, int pipe_fd[2]);

//cmd_path.c
int		index_path_cmd(char *cmd, char **path);
char	*get_path_cmd(char *cmd, char **path);



#endif