#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE3(mysyscall, pid_t, pid, char __user *, buffer, size_t, buf_size)
{

        struct task_struct * mytask;
        char nome_processo[TASK_COMM_LEN];
	size_t dim;

        printk(KERN_DEBUG "System call trova-processo (%d, %p, %ld)\n", pid, buffer, buf_size);

        mytask = find_task_by_vpid(pid);	 // ricerca del process control block, in base al PID

        if(!mytask) {
                printk(KERN_DEBUG "Processo non trovato: %d\n", pid);	      // find_task_by_vpid è fallita
                return -ESRCH;
        }

	
	__get_task_comm(nome_processo, TASK_COMM_LEN, mytask);

        printk(KERN_DEBUG "Processo trovato: %d = %s\n", pid, nome_processo);


	dim = (buf_size < TASK_COMM_LEN) ? buf_size : TASK_COMM_LEN;

        if( copy_to_user( buffer, nome_processo, dim) ) {

                printk(KERN_DEBUG "Errore di accesso alla memoria: %p\n", buffer);	// copy_to_user è fallita
                return -EFAULT;
        }

        return 0;
}

