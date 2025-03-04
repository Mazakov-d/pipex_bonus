/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:15:23 by dmazari           #+#    #+#             */
/*   Updated: 2025/03/04 15:01:52 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>

typedef struct s_pipe_data
{
	int		pipe_fd[2];
	char	*infile;
	char	*outfile;
}	t_pipe_data;

char	*str_dup_c(char *str, char c_limit, char c_join);
int		count_str_c_limit(char *str, char c_limit);
char	**split_c(char *str, char c_limit, char c_join);

void	*free_strs(char **strs);
int		ft_strlen(char *str);
char	*ft_strcat(char *str_a, char *str_b);

int		index_path_cmd(char *cmd, char **path);
char	**get_path_env(char **env);
char	*get_path_cmd(char *cmd, char **path);
char	**cmd(char *cmd, int a, int b);
int		cmd_a_infile(char **cmd_a, char **env, char *infile, int pipe_fd[2]);
int		cmd_b_outfile(char **cmd_b, char **env, char *outfile, int pipe_fd[2]);

void	child_process_first(char **cmd, char **env, t_pipe_data data);
void	child_process_second(char **cmd, char **env, t_pipe_data data);

int		error_pipe(int i);
int		error_create_cmd(int pipe_fd[2], int i);
int		error_fork(int i, char *path_cmd, int pipe_fd[2]);

int		free_close_fd(char *str, int pipe_fd[2], int i);
int		open_switch_stdin(char *file);
int		open_switch_stdout(char *file);

#endif