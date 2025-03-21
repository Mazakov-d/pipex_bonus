/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 13:02:19 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 22:41:44 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	here_doc_error(int i)
{
	perror("Error: Cannot create temporary file for here_doc");
	return (i);
}

int	is_here_doc(char *str)
{
	if (!str)
		return (0);
	if (ft_strlen(str) == 8)
	{
		if (str[0] == 'h' && str[1] == 'e' && str[2] == 'r' && str[3] == 'e'
			&& str[4] == '_' && str[5] == 'd' && str[6] == 'o' && str[7] == 'c')
			return (1);
	}
	return (0);
}

int	is_limiter(char *str, char *limiter)
{
	int	i;

	i = 0;
	while (str && str[i] && limiter && limiter[i] && str[i] == limiter[i])
		i++;
	if (limiter[i] == '\0' && str[i] == '\n')
		return (0);
	return (1);
}

int	setup_here_doc(char **args)
{
	char	*line;
	int		fd;

	fd = open(".temp_here_doc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (here_doc_error(-1));
	while (1)
	{
		write(STDOUT_FILENO, "heredoc> ", 9);
		line = get_next_line(STDIN_FILENO);
		if (!line)
		{
			close(fd);
			return (here_doc_error(-1));
		}
		if (is_limiter(line, args[1]) == 0)
			break;
		write(fd, line, ft_strlen(line));
		free(line);
	}
	if (line)
		free(line);
	close(fd);
	return (1);
}
