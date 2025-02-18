/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:01:58 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/02/18 18:46:17 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	init_pipex(t_cmd **cmds, char **args, int *i)
{
	*cmds = get_commands(args + 1);
	if (!*cmds)
		return (1);
	*i = last_cmd(*cmds) + 1;
	return (0);
}

int	handle_single_cmd(t_cmd *cmds, char **env, char **args, char *outfile)
{
	if (one_cmd(cmds, env, args[0], outfile) == 1)
		return (free_cmd_int(cmds));
	return (0);
}

int	handle_first_cmd(t_cmd *cmds, char **env, int *fd, char *infile)
{
	if (pipe(fd) < 0)
		return (free_cmd_int(cmds));
	if (cmd_infile(cmds->cmd, env, infile, fd) == 1)
		return (free_cmd_fd(cmds, fd, 1));
	return (0);
}

int	pipex(char **args, char **env, char *outfile)
{
	t_cmd	*cmds;
	int		pipe_fd[2];
	int		prev_pipe[2];
	int		i;

	if (init_pipex(&cmds, args, &i) == 1)
		return (1);
	if (i == 1)
		return (handle_single_cmd(cmds, env, args, outfile));
	if (handle_first_cmd(cmds, env, pipe_fd, args[0]) != 0)
		return (1);
	while (cmds->next && --i > 1)
	{
		cmds = cmds->next;
		prev_pipe[0] = pipe_fd[0];
		prev_pipe[1] = pipe_fd[1];
		if (pipe(pipe_fd) < 0)
			return (free_cmd_fd(cmds, prev_pipe, 1));
		if (cmd_to_pipe(cmds->cmd, env, prev_pipe, pipe_fd) == 1)
			return (free_cmd_fd(cmds, pipe_fd, 1));
	}
	cmds = cmds->next;
	if (cmd_outfile(cmds->cmd, env, outfile, pipe_fd) == 1)
		return (free_cmd_fd(cmds, pipe_fd, 1));
	return (free_cmd_fd(cmds, pipe_fd, 0));
}

int	main(int ac, char **av, char **env)
{
	int	i;

	if (ac > 2)
	{
		i = 0;
		while (av[i])
			i++;
		if (pipex((av + 1), env, (av[i - 1])) == 1)
			write(2, "Error\n", 7);
	}
}
