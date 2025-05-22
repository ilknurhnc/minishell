/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbayram <hbayram@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 13:11:21 by hbayram           #+#    #+#             */
/*   Updated: 2025/05/22 16:35:00 by hbayram          ###   ########.fr       */
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
// void set_heredoc()
// {

// }

// void set_redirect()
// {
	
// }



// void set_argv(t_exec *temp, int i)
// {
// 	t_exec *current;
//     t_executor **node;
//     int j;

// 	j = 0;
//     current = temp;
//     while (current)
//     {
//         while (current && current->rank == 4)
//         {
//             node[i]->argv[j] = ft_strdup(current->content);
//             j++;
//             current = current->next;
//         }
// 		// if (current && current->rank == 3)
//         // 	set_heredoc();
// 		// if (current && current->rank != 3 && current->rank != 1)
// 		// 	set_redirect();
		
//     }    
// }

void set_argv(t_executor **node, t_exec *temp, int i)
{
    t_exec *current = temp;
    int j = 0;

    node[i]->argv = malloc(sizeof(char *) * 100); 
    if (!node[i]->argv)
        return; // malloc hatası
    while (current)
    {
        if (current->content && current->rank == 4)
        {
            node[i]->argv[j] = ft_strdup(current->content);
            j++;
        }
        // diğer durumlar (redirect, heredoc vb.) buraya...
        current = current->next;
    }
    node[i]->argv[j] = NULL; // argv'nin sonunu null ile işaretle
}

// void	redirect_handle(t_executor *node)
// {
//     int fd;

//     // Dosya yönlendirme işlemleri sadece child process'te yapılmalı
//     if (node->infile) // Girdi dosyası varsa
//     {
//         fd = open(node->infile, O_RDONLY);
//         if (fd < 0)
//         {
//             perror("minishell: open infile");
//             return;
//         }
//         dup2(fd, STDIN_FILENO); // Girdi dosyasını stdin'e yönlendir
//         close(fd);
//     }
    
//     if (node->outfile) // Çıktı dosyası varsa
//     {
//         fd = open(node->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//         if (fd < 0)
//         {
//             perror("minishell: open outfile");
//             return;
//         }
//         dup2(fd, STDOUT_FILENO); // Çıktı dosyasını stdout'a yönlendir
//         close(fd);
//     }
    
//     if (node->append_outfile) // Çıktıya ekleme dosyası varsa
//     {
//         fd = open(node->append_outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
//         if (fd < 0)
//         {
//             perror("minishell: open append outfile");
//             return;
//         }
//         dup2(fd, STDOUT_FILENO); // Çıktıya eklemeyi stdout'a yönlendir
//         close(fd);
//     }
    
//     if (node->heredoc) // Heredoc işlemi varsa
//     {
//         // Heredoc işlemine göre özel bir yönlendirme yapabilirsin.
//         // Burada sadece placeholder olarak bırakıyorum:
//         // heredoc_handle(node->heredoc);
//     }
// }

void run_execve(t_executor *node, int input_fd, int output_fd)
{
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
    //redirect_handle(node);
    execve("/bin/ls", node->argv, node->program->env_str);
    perror("execve failed");
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
        pid = fork();
        if (pid == -1)
        {
            perror("fork failed");
            exit(1);
        }
        if (pid == 0) // Child process
        {
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
	i = program->exec->pipe + 2;
	node = malloc(sizeof(t_executor *) * (i));
	if (!node)
		return ; // exit 
	while (count < i)
	{
		node[count] = malloc(sizeof(t_executor));
		if (!node[count])
		{
			while (--count > 0)
				free(node[count]);
			free(node);
		}
        set_argv(node, current, count);
		node[count]->infile = NULL;
		node[count]->outfile = NULL;
		node[count]->heredoc = NULL;
		node[count]->append_outfile = NULL;
		node[count]->pipe = program->exec->pipe;
		if (count > 0)
		    node[count - 1]->next = node[count];
		current = current->next;
        count++;
	}
	//print_exec_list(program->exec->next);
    main_execute(node[0]);
}

// void main_execute(t_executor *exec)
// {
//     int pipefds[2];    // Pipe'dan veri geçirecek dosya descriptorları
//     pid_t pid;         // Child process ID
//     int prev_fd = 0;   // Önceki komutun çıktısını tutacak dosya descriptoru (ilk başta 0, stdin)
//     t_executor *current = exec;

//     // Komutlar arasında dolaşalım
//     while (current)
//     {
//         // Eğer bir sonraki komut varsa, pipe aç
//         if (current->next)
//         {
//             if (pipe(pipefds) == -1)
//             {
//                 perror("pipe failed");
//                 exit(1);
//             }
//         }
//         pid = fork(); // Her komut için child process oluşturuluyor
//         if (pid == -1)
//         {
//             perror("fork failed");
//             exit(1);
//         }
//         if (pid == 0) // Child process
//         {
//             // Yönlendirmeleri uygula (input, output yönlendirmeleri)
//             redirect_handle(current);
//             // Eğer önceki komutun çıktısı bir pipe'a yönlendirilmişse
//             if (prev_fd != 0)
//             {
//                 dup2(prev_fd, STDIN_FILENO); // Önceki komutun çıktısını stdin'e yönlendir
//                 close(prev_fd);
//             }
//             // Eğer bu komut bir pipe'a veri gönderecekse
//             if (current->next != NULL)
//             {
//                 close(pipefds[0]); // Okuma ucu kapatılıyor (bu komutun stdout'u pipe'a yazacak)
//                 dup2(pipefds[1], STDOUT_FILENO); // stdout'u pipe'ın yazma ucuna yönlendir
//             }
//             // Eğer bu komut son komutsa, standart çıktıyı ekran veya dosyaya yönlendir
//             if (current->next == NULL)
//             {
//                 close(pipefds[1]); // Yazma ucu kapatılır
//             }
//             // Komutu execve ile çalıştır
//             execve(current->argv[0], current->argv, NULL);
//             // Eğer execve başarısız olursa
//             perror("execve failed");
//             exit(1);
//         }
//         else // Parent process
//         {
//             // Önceki pipe'ı kapatıyoruz çünkü sadece bu komutun stdout'u ile ilgileniyoruz
//             if (prev_fd != 0)
//                 close(prev_fd);
//             // Eğer son bir komut çalışıyorsa, son pipe'ı kapatıyoruz
//             if (current->next != NULL)
//                 close(pipefds[1]);
//             prev_fd = pipefds[0]; // Sonraki komutun girişine bu pipe'ı yönlendir
//             current = current->next; // Sonraki komut için ilerle
//         }
//     }
//     // Parent process, child'ları bekler
//     while (wait(NULL) > 0)
//         ;
// }
