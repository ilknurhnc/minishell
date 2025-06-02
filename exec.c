/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 13:11:21 by hbayram           #+#    #+#             */
/*   Updated: 2025/06/02 12:40:12 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_heredoc(t_executor *cmd)
{
	int pipefd[2];
	char *line;
	char *delimiter;

	if (!cmd->heredoc_delimiters || !cmd->heredoc_delimiters[0])
		return;
	delimiter = cmd->heredoc_delimiters[0];
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
	close(pipefd[1]);
	cmd->heredoc_file = pipefd[0]; // child process buradan okuyacak
}


void pipe_count(t_exec *node)
{
    t_exec *new;
    int i;

    new = node;
    i = 0;
    while (new)
    {
        if(new->rank == 1)
            i++;
        new = new->next;
    }
    node->pipe = i;
}

t_exec *set_argv(t_executor **node, t_exec *start, int i)
{
    t_exec *current = start;
    int j;
    int h_count;

    j = 0;
    h_count = 0;
    node[i]->argv = malloc(sizeof(char *) * 100);
    if (!node[i]->argv)
        return NULL;
    while (current)
    {
        if (current->rank == 1) // pipe
        {
            current = current->next;
            break;
        }
        if (current->rank == 4 && current->content)
        {
            node[i]->argv[j++] = ft_strdup(current->content);
        }
        if (current && current->rank == 3)
		{
			set_heredoc(current, node[i], h_count);
			h_count++;
            if (!node[i]->heredoc_delimiters)
            {
                node[i]->heredoc_delimiters = malloc(sizeof(char *) * 100);
                if (!node[i]->heredoc_delimiters)
                    return NULL;
            }
            node[i]->heredoc_delimiters[h_count] = NULL;
			current = current->next->next;
			continue ;
		}
		if (current && current->rank != 4 && current->rank != 1)
		{
			set_redirect(current, node[i]);
			current = current->next->next;
			continue ;
		}
        current = current->next;
    }
    node[i]->argv[j] = NULL;
    return current;
}

char *join_path(const char *dir, const char *cmd)
{
    int len = strlen(dir) + strlen(cmd) + 2; // '/' + '\0'
    char *full_path = malloc(len);
    if (!full_path)
        return NULL;
    strcpy(full_path, dir);
    if (dir[strlen(dir)-1] != '/')
        strcat(full_path, "/");
    strcat(full_path, cmd);
    return full_path;
}

char *get_path_from_env(t_env *env)
{
    while (env)
    {
        if (ft_strcmp(env->before_eq, "PATH") == 0)
        {
            if (env->after_eq && ft_strlen(env->after_eq) > 0)
                return env->after_eq;
            else
                return NULL; // PATH var ama boş
        }
        env = env->next;
    }
    return NULL; // PATH yok
}

char *get_next_path_dir(char *path_str, int *start_pos)
{
    int i = *start_pos;
    int start;
    int len;

    if (!path_str || path_str[i] == '\0')
        return NULL;

    start = i;
    while (path_str[i] != ':' && path_str[i] != '\0')
        i++;

    len = i - start;
    char *token = malloc(len + 1);
    if (!token)
        return NULL;
    strncpy(token, &path_str[start], len);
    token[len] = '\0';

    if (path_str[i] == ':')
        *start_pos = i + 1;  // sonraki aramaya hazırla
    else
        *start_pos = i;      // son token

    return token;
}


char *find_command_path(t_main *program, char *command)
{
    char *path_env = get_path_from_env(program->env);
    if (!path_env)
        return NULL;

    int pos = 0;
    char *dir;
    char *full_path;
    
    while ((dir = get_next_path_dir(path_env, &pos)) != NULL)
    {
        full_path = join_path(dir, command);
        free(dir);
        if (!full_path)
            return NULL;
        if (access(full_path, X_OK) == 0)
            return full_path;
        free(full_path);
    }
    return NULL;
}


void run_execve(t_executor *node, int input_fd, int output_fd)
{
    char *cmd_path;

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
    cmd_path = find_command_path(node->program, node->argv[0]);
    if (!cmd_path)
    {
        printf("%s: command not found\n", node->argv[0]);
        free_token(node->program);
        free_exec(node->program);
        free_executer(node->program);
        exit(127);
    }
    execve(cmd_path, node->argv, node->program->env_str);
    perror("execve failed");
    free(cmd_path);
    exit(1);
}

void main_execute(t_executor *exec)
{
    int pipefds[2];
    pid_t pid;
    int prev_fd = STDIN_FILENO;
    t_executor *current = exec;

    while (current)
    {
        int output_fd = STDOUT_FILENO;
        int is_builtin = is_builtin_command(current->argv[0]);

        if (current->heredoc_delimiters && current->heredoc_delimiters[0])
            handle_heredoc(current);
            
        if (current->next)
        {
            if (pipe(pipefds) == -1)
            {
                perror("pipe failed");
                exit(1);
            }
            output_fd = pipefds[1];
        }

        // BUILTIN ve PIPE yoksa (tek komutsa) PARENT içinde çalıştırılır
        if (is_builtin && !current->next && prev_fd == STDIN_FILENO && output_fd == STDOUT_FILENO)
        {   
            int saved_stdin = dup(STDIN_FILENO);
            int saved_stdout = dup(STDOUT_FILENO);

            redirect_handle(current);

        if (current->heredoc_file != -1)
        {
            dup2(current->heredoc_file, STDIN_FILENO);
            close(current->heredoc_file);
        }

        execute_builtin(current); 
            // stdout ve stdin'i eski haline getir
        dup2(saved_stdout, STDOUT_FILENO);
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdout);
        close(saved_stdin);
        }
        
        else
        {
            pid = fork();
            if (pid == -1)
            {
                perror("fork failed");
                exit(1);
            }

            if (pid == 0) // 🔸 CHILD PROCESS
            {
                if (current->next)
                    close(pipefds[0]);

                // heredoc varsa stdin'e yönlendir
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

                // pipe varsa stdout'u bağla
                if (output_fd != STDOUT_FILENO)
                {
                    dup2(output_fd, STDOUT_FILENO);
                    close(output_fd);
                }

                // redirect'leri uygula
                redirect_handle(current);
                if (current->error)
                {
                    fprintf(stderr, "minishell: %s\n", current->error);
                    exit(1);
                }

                // built-in ise çalıştır, değilse execve yap
                if (is_builtin)
                    exit(execute_builtin(current));
                else
                    run_execve(current, STDIN_FILENO, STDOUT_FILENO);
            }
        }
        
        if (prev_fd != STDIN_FILENO)
            close(prev_fd);
        if (current->heredoc_file != -1)
            close(current->heredoc_file);
        if (current->next)
            close(pipefds[1]);

        if (current->next)
            prev_fd = pipefds[0];
        else
            prev_fd = STDIN_FILENO;

        current = current->next;
    }
    
    while (wait(NULL) > 0)
        ;
}


void print_exec_list(t_exec *head)
{
    t_exec *current = head;
    int i = 0;

    printf("---- t_exec list start ----\n");
    while (current)
    {
        printf("Node[%d] rank=%d, content='%s'\n", i, current->rank, current->content ? current->content : "NULL");
        current = current->next;
        i++;
    }
    printf("---- t_exec list end ----\n");
}

void	prep_exec(t_main *program)
{
	t_exec *current;
	t_executor **node;
	int i;
	int count;

	current = program->exec;
	count = 0;
	setting_sign(program);
	pipe_count(program->exec);
	i = program->exec->pipe + 1;
	node = malloc(sizeof(t_executor *) * (i + 1));
	if (!node)
		return ; // exit 
	while (count < i)
	{
		node[count] = malloc(sizeof(t_executor));
		if (!node[count])
		{
			while (--count >= 0)
				free(node[count]);
			free_executer(program);
            return ;
		}
		node[count]->infile = NULL;
		node[count]->outfile = NULL;
		node[count]->heredoc_file = -1;
		node[count]->append = NULL;
		node[count]->pipe = program->exec->pipe;
        node[count]->heredoc_delimiters = NULL;
        node[count]->program = program;
        node[count]->error = NULL;
        current = set_argv(node, current, count);
		if (count > 0)
		    node[count - 1]->next = node[count];
        node[count]->next = NULL;
        count++;
	}
    node[count] = NULL;
    program->executer = node;
    program->control = 1;
    main_execute(node[0]);
}
