/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_managements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 20:48:27 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 21:36:13 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	switch_fd_in_out(int fd_in, int fd_out)
{
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
}

int	open_in_out(t_data *data, char **args, int ac)
{
	int		fd;
	char	*infile;

	if (is_here_doc(args[1]))
		infile = ".temp_here_doc.txt";
	else
		infile = args[1];
	fd = open(infile, O_RDONLY);
	if (fd < 0)
		write(2, "Error : Can't read infile\n", 27);
	else
		data->fd_in = fd;
	fd = open(args[ac - 1], O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd < 0)
		return (free_data_fd(data, 1, "Error : Can't open outfile\n"));
	else
		data->fd_out = fd;
	return (0);
}
