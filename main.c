/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 15:01:58 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/02/18 18:17:39 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	last_cmd(t_cmd *cmd)
{
	t_cmd	*save;
	int		i;

	save = cmd;
	i = 0;
	while (cmd->next)
		cmd = cmd->next;
	cmd = cmd->prev;
	i = cmd->place;
	return (i);
}

int	pipex(char **args, char **env, char *outfile)
{
	t_cmd	*cmds;
	t_cmd	*ptr;
	int		pipe_fd[2];
	int		i;

	cmds = get_commands(args + 1);
	if (!cmds)
		return (1);
	i = last_cmd(cmds) + 1;
	if (i == 1)
		return (one_cmd(cmds, env, args[0], outfile));
	if (pipe(pipe_fd) < 0)
		return (free_cmd_int(cmds));
	if (cmd_infile(cmds->cmd, env, args[0], pipe_fd) == 1)
		return (free_cmd_fd(cmds, pipe_fd, 1));
	while (cmds->next && --i > 1)
	{
		cmds = cmds->next;
		// int k = 0;
		// while (cmds->cmd[k])
		// {
		// 	printf("%s\n", cmds->cmd[k]);
		// 	k++;
		// }
		if (cmd_to_pipe(cmds->cmd, env, pipe_fd) == 1)
			return (free_cmd_fd(cmds, pipe_fd, 1));
	}
	cmds = cmds->next;
	if (cmd_outfile(cmds->cmd, env, outfile, pipe_fd) == 1)
		return (free_cmd_fd(cmds, pipe_fd, 1));
	return (free_cmd_fd(cmds, pipe_fd, 0));
}

int main(int ac, char **av, char **env)
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