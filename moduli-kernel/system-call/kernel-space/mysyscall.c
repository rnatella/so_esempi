#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>

asmlinkage long sys_mysyscall(pid_t pid, char * buffer, unsigned int buf_size) {

        struct task_struct * mytask;
        char * nome_processo;

        printk(KERN_DEBUG "System call trova-processo\n");

        mytask = find_task_by_vpid(pid);	 // ricerca del process control block, in base al PID

        if(!mytask) {
                printk(KERN_DEBUG "Processo non trovato: %d\n", pid);	      // find_task_by_vpid è fallita
                return -ESRCH;
        }

        nome_processo = mytask->comm;
        printk(KERN_DEBUG "Processo trovato: %d = %s\n", pid, nome_processo);

        if( copy_to_user( buffer, nome_processo, (buf_size<TASK_COMM_LEN ? buf_size : TASK_COMM_LEN)) ) {

                printk(KERN_DEBUG "Errore di accesso alla memoria: %p\n", buffer);	// copy_to_user è fallita
                return -EFAULT;
        }

        return 0;
}

