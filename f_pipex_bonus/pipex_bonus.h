/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:02:18 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/04 17:59:19 by dmazari          ###   ########.fr       */
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

typedef struct s_cmd
{
	char			**cmd;
	struct s_cmd	*next;
	struct s_cmd	*prev;
	int				place;
	int				pipe_in[2];
	int				pipe_out[2];
}	t_cmd;

/***free_functions.c*/
void	*free_strs(char **strs);
void	*free_cmd(t_cmd *cmd);
int		free_cmd_int(t_cmd *cmd);
int		free_ptr(void *ptr, int i, char *error);
int		free_cmd_fd(t_cmd *cmd, int pipe_fd[2], int i, char *error);

/**utils.c*/
int		ft_strclen(char *str, int c);
char	*ft_strcat(char *str, char *s);
int		count_cmd(char **args);
int		last_cmd(t_cmd *cmd);
char	*ft_strdup(char *str);

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
t_cmd	*get_commands(char **args);

/**switch_fd.c*/
int		open_switch_stdin(char *file);
int		open_switch_stdout(char *file);
int		open_switch_in_out(char *infile, char *outfile);
void	switch_fd_in_out(int fd_a, int fd_b);

/**executing.c*/
void	child_process_first(char **cmd_a, char **env, 
			char *infile, int pipe_fd[2]);
void	child_process_last(char **cmd_b, char **env, 
			char *outfile, int pipe_fd[2]);
void	child_process_pipe(char **cmd, char **env, int prev[2], int next[2]);
int		cmd_infile(char **cmd_a, char **env, char *infile, int pipe_fd[2]);
int		cmd_outfile(char **cmd_b, char **env, char *outfile, int pipe_fd[2]);
int		cmd_to_pipe(char **cmd, char **env, int prev_pipe[2], int next_pipe[2]);
int		one_cmd(t_cmd *cmd, char **env, char *infile, char *outfile);

/**cmd_path.c*/
int		index_path_cmd(char *cmd, char **path);
char	*get_path_cmd(char *cmd, char **path);

/**here_doc.c */
int		is_here_doc(char *str);
int		is_limiter(char *str, char *limiter);
int		here_doc(char **args);

/**main.c*/
int		process_middle_cmds(t_cmd **cmds, char **env, int *i, int pipe_fd[2]);
void	wait_all_children(void);

/**str_error.c */
void	*error_malloc(void);

#endif