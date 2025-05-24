/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihancer <ihancer@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 13:11:21 by hbayram           #+#    #+#             */
/*   Updated: 2025/05/24 15:58:07 by ihancer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
    //printf("set_argv çağrıldı, i=%d, start rank=%d, content='%s'\n", i, start->rank, start->content ? start->content : "NULL");
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
    if (j == 0)
        printf("WARNING: argv boş kaldi. i = %d\n", i);
    else
    {
        printf("Node[%d] argv: ", i);
        for (int x = 0; x < j; x++)
            printf("%s ", node[i]->argv[x]);
        printf("\n");
    }
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

char *find_command_path(char *command)
{
    char *path_env = getenv("PATH");
    char *path_copy;
    char *dir;
    char *full_path;

    if (!path_env)
        return NULL;

    path_copy = strdup(path_env);
    if (!path_copy)
        return NULL;

    dir = strtok(path_copy, ":");
    while (dir)
    {
        full_path = join_path(dir, command);
        if (!full_path)
        {
            free(path_copy);
            return NULL;
        }
        if (access(full_path, X_OK) == 0) // çalıştırılabilir mi?
        {
            free(path_copy);
            return full_path;
        }
        free(full_path);
        dir = strtok(NULL, ":");
    }
    free(path_copy);
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

    // Komutun tam yolunu bul
    cmd_path = find_command_path(node->argv[0]);
    if (!cmd_path)
    {
        perror("Komut bulunamadi");
        exit(127);
    }
    execve(cmd_path, node->argv, node->program->env_str);
    perror("execve failed");
    free(cmd_path); // sadece execve başarısız olursa buraya gelir
    exit(1);
}

void main_execute(t_executor *exec)
{
    int pipefds[2];
    pid_t pid;
    int prev_fd;
    t_executor *current;

    current = exec;
    prev_fd = STDIN_FILENO;
    while (current)
    {
        redirect_handle(current);
        int output_fd = STDOUT_FILENO;
        if (current->next)
        {
            if (pipe(pipefds) == -1)
            {
                perror("pipe failed");
                exit(1);
            }
            output_fd = pipefds[1]; // stdout bu pipe'a yazılacak
        }
        printf("Child process: executing command: ");
        for (int k = 0; current->argv[k]; k++)
            printf("%s ", current->argv[k]);
        printf("\n");
        pid = fork();
        if (pid == -1)
        {
            perror("fork failed");
            exit(1);
        }
        if (pid == 0) // Child process
        {
            redirect_handle(current);
            if (current->next)
                close(pipefds[0]); // Bu child, pipe'ın sadece yazma ucunu kullanır
            run_execve(current, prev_fd, output_fd);
        }
        else // Parent process
        {
            if (prev_fd != STDIN_FILENO)
                close(prev_fd); // Önceki pipe'dan gelen okuma ucunu kapat
            if (current->next)
                close(pipefds[1]); // Bu parent, pipe'ın yazma ucunu kapatır
            if (current->next)
                prev_fd = pipefds[0]; // Pipe'ın okuma ucunu kaydet
            else
                prev_fd = STDIN_FILENO; // Artık gerek yok, stdin gibi davran
            current = current->next;
        }
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
			free(node);
		}
		node[count]->infile = NULL;
		node[count]->outfile = NULL;
		node[count]->heredoc_file = NULL;
		node[count]->append = NULL;
		node[count]->pipe = program->exec->pipe;
        node[count]->program = program;
        current = set_argv(node, current, count);
		if (count > 0)
		    node[count - 1]->next = node[count];
        node[count]->next = NULL;
        count++;
	}
    node[count] = NULL;
    program->executer = node;
	//print_exec_list(program->exec->next);
    main_execute(node[0]);
}
