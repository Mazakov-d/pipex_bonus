/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dorianmazari <dorianmazari@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 13:02:19 by dorianmazar       #+#    #+#             */
/*   Updated: 2025/03/07 21:55:55 by dorianmazar      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

/**
 * @brief Print an error message for here_doc failures
 * 
 * @param i Value to return
 * @return int Return value
 */
int	here_doc_error(int i)
{
	perror("Error: Cannot create temporary file for here_doc");
	return (i);
}

/**
 * @brief Check if a string is "here_doc"
 * 
 * @param str String to check
 * @return int 1 if string is "here_doc", 0 otherwise
 */
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

/**
 * @brief Check if a line matches the limiter
 * 
 * @param str Line to check
 * @param limiter Limiter string
 * @return int 0 if line matches limiter, 1 otherwise
 */
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

/**
 * @brief Set up here_doc input
 * 
 * @param args Command line arguments (args[1] is the limiter)
 * @return int 1 on success, -1 on failure
 */
int	setup_here_doc(char **args)
{
	char	*line;
	int		fd;

	// Create temporary file for here_doc
	fd = open(".temp_here_doc.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (here_doc_error(-1));
	
	// Read lines until limiter is found
	while (1)
	{
		write(STDOUT_FILENO, "heredoc> ", 9);
		line = get_next_line(STDIN_FILENO);
		
		if (!line)
		{
			close(fd);
			return (here_doc_error(-1));
		}
		
		// Check if line matches limiter
		if (is_limiter(line, args[1]) == 0)
			break;
		
		// Write line to temporary file
		write(fd, line, ft_strlen(line));
		free(line);
	}
	
	if (line)
		free(line);
	
	close(fd);
	return (1);
}