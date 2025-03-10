/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:02:18 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 22:49:55 by dorianmazar      ###   ########.fr       */
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
	char			**args;
	struct s_cmd	*next;
	struct s_cmd	*prev;
	int				position;
}	t_cmd;

typedef struct s_fd
{
	int				**pipes;
	int				in_file;
	int				out_file;
	int				pipe_count;
}	t_fd;

typedef struct s_data
{
	t_cmd			*cmd_list;
	t_fd			fd;
	char			**env;
	int				cmd_count;
	int				here_doc;
}	t_data;

/* String utils */
int		ft_strlen(char *str);
int		ft_strclen(char *str, int c);
char	*ft_strcat(char *str, char *s);
char	*ft_strndup(char *str, int n);
char	*ft_strdup(char *str);

/* Path utils */
char	*str_dup_c(char *str, char c_limit, char c_join);
int		count_str_c_limit(char *str, char c_limit);
int		find_path_in_env(char **env);
char	**split_c(char *str, char c_limit, char c_join);
char	**get_path_from_env(char **env);

/* Command parsing */
int		is_flag(char *str);
int		count_flags(char **args, int i);
int		is_grep(char *str, int *k);
char	**parse_cmd_args(char **args, int *i, int j, int k);
t_cmd	*cmd_new(t_cmd *current, int position);
t_cmd	*parse_commands(char **args, int cmd_count);

/* File descriptor management */
int		init_pipes(t_data *data, int ac, char **av);
int		open_input_file(t_data *data, char *filename);
int		open_output_file(t_data *data, char *filename, int append_mode);
int		setup_files(t_data *data, char **args, int ac);
void	redirect_io(int fd_in, int fd_out);

/* Command execution */
char	*get_cmd_path(char *cmd, char **path_dirs);
void	setup_cmd_pipes(t_data *data, t_cmd *cmd);
void	child_process(t_data *data, t_cmd *cmd, char **path_dirs);
int		wait_for_children(int cmd_count, pid_t *pids);
int		run_pipeline(t_data *data);

/* Here_doc handling */
int		here_doc_error(int i);
int		is_here_doc(char *str);
int		is_limiter(char *str, char *limiter);
int		setup_here_doc(char **args);

/* Memory management */
void	*free_str_array(char **strs);
void	*free_cmd_list(t_cmd *cmd_list);
int		free_data_struct(t_data *data);
int		free_ptr_with_error(void *ptr, int ret_val, char *error_msg);
int		clean_exit(t_data *data, int ret_val, char *error_msg);
void	close_all_pipes(int **pipes, int pipe_count);
void	free_pipes(int **pipes, int pipe_count);

/* Error handling */
void	print_error(char *msg);
int		error_return(char *msg, int ret_val);
void	*error_malloc(void);

/* Main functions */
int		init_data(t_data *data, char **args, char **env, int ac);
int		pipex(char **args, char **env, int ac);

#endif