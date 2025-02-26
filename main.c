/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmazari <dmazari@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:01:58 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/02/26 16:32:09 by dmazari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	init_pipex(t_cmd **cmds, char **args, int *i)
{
	*cmds = get_commands(args + 1);
	if (!*cmds)
	{
		write(2, "Error : creating command\n", 26);
		return (1);
	}
	*i = last_cmd(*cmds) + 1;
	return (0);
}

int	handle_single_cmd(t_cmd *cmds, char **env, char *infile, char *outfile)
{
	if (one_cmd(cmds, env, infile, outfile) == 1)
		return (1);
	return (0);
}

int	handle_first_cmd(t_cmd *cmds, char **env, int *fd, char *infile)
{
	if (pipe(fd) < 0)
	{
		write(2, "Error : creating pipe\n", 23);
		return (free_cmd_int(cmds));
	}
	if (cmd_infile(cmds->cmd, env, infile, fd) == 1)
		return (free_cmd_fd(cmds, fd, 1, NULL));
	return (0);
}

int	pipex(char **args, char **env, char *outfile, char *infile)
{
	t_cmd	*cmds;
	int		pipe_fd[2];
	int		prev_pipe[2];
	int		i;

	if (init_pipex(&cmds, args, &i) == 1)
		return (1);
	if (i == 1)
		return (handle_single_cmd(cmds, env, infile, outfile));
	if (handle_first_cmd(cmds, env, pipe_fd, infile) != 0)
		return (1);
	while (cmds->next && --i > 1)
	{
		cmds = cmds->next;
		prev_pipe[0] = pipe_fd[0];
		prev_pipe[1] = pipe_fd[1];
		if (pipe(pipe_fd) < 0)
			return (free_cmd_fd(cmds, prev_pipe, 1, "Error : pipe\n"));
		if (cmd_to_pipe(cmds->cmd, env, prev_pipe, pipe_fd) == 1)
			return (free_cmd_fd(cmds, pipe_fd, 1, "Error : command to pipe\n"));
	}
	cmds = cmds->next;
	if (cmd_outfile(cmds->cmd, env, outfile, pipe_fd) == 1)
		return (free_cmd_fd(cmds, pipe_fd, 1, "Error : command outfile\n"));
	return (free_cmd_fd(cmds, pipe_fd, 0, NULL));
}

int	main(int ac, char **av, char **env)
{
	int	save;

	if (ac > 3)
	{
		save = here_doc(av + 1);
		if (save > 0)
		{
			pipex((av + 2), env, av[ac - 1], ".temp_here_doc.txt");
			unlink(".temp_here_doc.txt");
		}
		else if (save == -1)
			write(2, "Error\n", 7);
		else
			pipex((av + 1), env, (av[ac - 1]), av[1]);
	}
}
