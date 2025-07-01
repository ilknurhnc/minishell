#include "../minishell.h"

void is_executable(char *cmd, t_main *program)
{
    char *executable;

    if (ft_strnstr(cmd, "./", 2) == NULL)
        return;
    executable = ft_strnstr(cmd, "./", 2);
    if (access(executable, F_OK) < 0)
    {
        printf("%s: No such file or directory\n", executable);
        set_exit_status_code(127);
        free_resources(program);
        exit(127);
        
    }
    if (access(executable, X_OK) < 0)
    {
        printf("%s: Permission denied\n", executable);
        set_exit_status_code(126);
        free_resources(program);
        exit(126);
    }
}

void	run_execve(t_executor *node, int input_fd, int output_fd)
{
	char	*cmd_path;

	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		dup2(output_fd, STDOUT_FILENO);
		close(output_fd);
	}
    is_executable(node->argv[0], node->program);
	cmd_path = find_command_path(node->program, node->argv[0]);
	if (!cmd_path)
	{
		printf("%s: command not found\n", node->argv[0]);
		free_resources(node->program);
		exit(127);
	}
	execve(cmd_path, node->argv, node->program->env_str);
	perror("execve failed");
	free_resources(node->program);
	free(cmd_path);
	exit(1);
}

void	single_built_in(t_executor *current)
{
	int	saved_stdin;
	int	saved_stdout;
	int builtin_status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	redirect_handle(current);
	if (current->heredoc_file != -1)
	{
		dup2(current->heredoc_file, STDIN_FILENO);
		close(current->heredoc_file);
	}
	builtin_status = execute_builtin(current);
	set_exit_status_code(builtin_status);  
	dup2(saved_stdout, STDOUT_FILENO);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdout);
	close(saved_stdin);
}
void manage_io(t_executor *current, int *pipefds, int output_fd, int prev_fd)
{
    if (current->next)
        close(pipefds[0]);

    if (current->heredoc_file != -1)
    {
        dup2(current->heredoc_file, STDIN_FILENO);
        close(current->heredoc_file);
    }
    else if (prev_fd != STDIN_FILENO)
    {
        dup2(prev_fd, STDIN_FILENO);
        close(prev_fd);
    }

    if (output_fd != STDOUT_FILENO)
    {
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }
	redirect_handle(current);
}

void child_process(t_executor *current, int *pipefds, int output_fd, int prev_fd)
{
    int code;

    manage_io(current, pipefds, output_fd, prev_fd);  // Yönlendirme işlemleri
    if (current->error || !current->argv[0])
    {
        if (current->error)
        {
            write(2, "minishell: ", 11);
            write(2, current->error, ft_strlen(current->error));
            write(2, "\n", 1);
        }
        set_exit_status_code(1);
        free_resources(current->program);
        exit(1);
    }
    if (is_builtin_command(current->argv[0]))
    {
        code = execute_builtin(current);
        free_resources(current->program);
        set_exit_status_code(1);
        exit(code);
    }
    else
        run_execve(current, STDIN_FILENO, STDOUT_FILENO);
}

void	ft_fork(t_executor *current, int *pipefds, int output_fd, int prev_fd)
{
	pid_t	pid;

	if (!current->error && is_builtin_command(current->argv[0]) && !current->next
		&& prev_fd == STDIN_FILENO && output_fd == STDOUT_FILENO)
		single_built_in(current);
	else
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork failed.");
			free_resources(current->program);
			exit(1);
		}
		if (pid == 0)
			child_process(current, pipefds, output_fd, prev_fd);
	}
	if (prev_fd != STDIN_FILENO)
		close(prev_fd);
	if (current->heredoc_file != -1)
		close(current->heredoc_file);
	if (current->next)
		close(pipefds[1]);
}


void setup_pipe_and_fork(t_executor *current, int *pipefds, int *output_fd, int prev_fd)
{
    if (current->next)
    {
        if (pipe(pipefds) == -1)
        {
            perror("pipe failed");
            free_resources(current->program);
            exit(1);
        }
        *output_fd = pipefds[1];
    }
    ft_fork(current, pipefds, *output_fd, prev_fd);
    if (prev_fd != STDIN_FILENO)
        close(prev_fd);
}

void wait_for_children(pid_t *last_pid, int *last_status)
{
    pid_t pid;
    int status;
    while ((pid = wait(&status)) > 0)
    {
        if (WIFEXITED(status))
            *last_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            *last_status = 128 + WTERMSIG(status);
        *last_pid = pid;
    }
}

void main_execute(t_executor *exec, int prev_fd)
{
    int pipefds[2];
    int output_fd;
    t_executor *current;
    pid_t last_pid = -1;
    int last_status = 0;

    current = exec;
    while (current)
    {
        output_fd = STDOUT_FILENO;
        if (current->heredoc_delimiters && current->heredoc_delimiters[0])
            handle_heredoc(current, current->program);
        setup_pipe_and_fork(current, pipefds, &output_fd, prev_fd);
        if (current->next)
            prev_fd = pipefds[0];
        else
            prev_fd = STDIN_FILENO;
        current = current->next;
    }
    wait_for_children(&last_pid, &last_status);
    if (last_pid != -1)
        set_exit_status_code(last_status);
}




// void	main_execute(t_executor *exec, int prev_fd)
// {
// 	int			pipefds[2];
// 	int			output_fd;
// 	t_executor	*current;
// 	int			status;
// 	pid_t		pid;
// 		pid_t		last_pid = -1;
// 	int			last_status = 0;
	
// 	current = exec;
// 	while (current)
// 	{
// 		output_fd = STDOUT_FILENO;
// 		if (current->heredoc_delimiters && current->heredoc_delimiters[0])
// 			handle_heredoc(current, current->program);
// 		if (current->next)
// 		{
// 			if (pipe(pipefds) == -1)
// 			{
// 				perror("pipe failed");
// 				free_resources(current->program);
// 				exit(1);
// 			}
// 			output_fd = pipefds[1];
// 		}
// 		ft_fork(current, pipefds, output_fd, prev_fd);
// 		if (prev_fd != STDIN_FILENO)
// 			close(prev_fd);
// 		if (current->next)
// 			prev_fd = pipefds[0];
// 		else
// 			prev_fd = STDIN_FILENO;
// 		current = current->next;
// 	}
// 	while ((pid = wait(&status)) > 0)
// 	{
// 		if (WIFEXITED(status))
// 			last_status = WEXITSTATUS(status);
// 		else if (WIFSIGNALED(status))
// 			last_status = 128 + WTERMSIG(status);
// 		last_pid = pid;
// 	}
// 	if (last_pid != -1)
// 		set_exit_status_code(last_status);
// }

// void	child_process(t_executor *current, int *pipefds, int output_fd,
// 		int prev_fd)
// {
// 	int code;
	
// 	if (current->next)
// 		close(pipefds[0]);
// 	if (current->heredoc_file != -1)
// 	{
// 		dup2(current->heredoc_file, STDIN_FILENO);
// 		close(current->heredoc_file);
// 	}
// 	else if (prev_fd != STDIN_FILENO)
// 	{
// 		dup2(prev_fd, STDIN_FILENO);
// 		close(prev_fd);
// 	}
// 	if (output_fd != STDOUT_FILENO)
// 	{
// 		dup2(output_fd, STDOUT_FILENO);
// 		close(output_fd);
// 	}
// 	redirect_handle(current);
// 	if (current->error || !current->argv[0])
// 	{
// 		if (current->error)
// 		{
// 			write(2, "minishell: ", 11);
// 			write(2, current->error, ft_strlen(current->error));
// 			write(2, "\n", 1);
// 		}
// 		set_exit_status_code(1);
// 		free_resources(current->program);
// 		exit(1);
// 	}
// 	if (is_builtin_command(current->argv[0]))
// 	{		
// 		code = execute_builtin(current);
// 		free_resources(current->program);
// 		set_exit_status_code(1);
// 		exit(code);
// 	}	
// 	else
// 		run_execve(current, STDIN_FILENO, STDOUT_FILENO);
// }


