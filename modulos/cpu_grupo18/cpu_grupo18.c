#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>		// pr_info
#include <linux/sched/signal.h> // for_each_process
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/sysinfo.h>
#include <linux/mm.h>
#include <linux/swap.h>
#include <linux/user_namespace.h>
#include <linux/cred.h>



static int cpu_g18_main(struct seq_file *m, void *v)
{
	struct task_struct *procesos;
	size_t process_counter = 0;
	for_each_process(procesos)
	{
		char *estado = procesos->state == -1 ? "Inejecutable" : procesos->state == 0 ? "Ejecutable" : "Detenido";
		seq_printf(m, "PID: %d\n\tNombre: %s\n\tUsuario: %d\n\tEstado: %s(%ld)\n", procesos->pid, procesos->comm, procesos->cred->euid,  estado, procesos->state);
		++process_counter;
	}
	seq_printf(m, "Número de procesos: %zu\n", process_counter);
	return 0;
}

/*
// Método auxiliar para la lectura
static int abrir_proceso_cpu(struct inode *inode, struct file *file)
{
	return single_open(file, cpu_g18_main, NULL);
}
*/
/*
// Estructura para llevar el control del asunto.
static struct file_operations opciones_de_archivo_procedimiento_cpu = {
	.owner = THIS_MODULE,
	.open = abrir_proceso_cpu,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};
*/
/*
int init_module(void)
{
	//printk(KERN_INFO "Hola mundo, somos el grupo 18 y este es el monitor de CPU.\n");;
	//proc_create("cpu_grupo18", 0, NULL, &opciones_de_archivo_procedimiento_cpu);
	//return 0;
}
*/
/*
void cleanup_module(void)
{
	//remove_proc_entry("cpu_grupo18", NULL);
	//printk(KERN_INFO "Sayonara mundo, somos el grupo 18 y este fue el monitor de CPU.\n");
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/*
static int open_action(struct seq_file *m, void *v) {
    struct task_struct *task;

    seq_printf(m, "PID;NOMBRE_PROCESO\n");

    for_each_process(task) {
        if(task->mm) {
            seq_printf(m,"%d;%s\n", task->pid, task->comm);
        }
    }

    return 0;
}
*/


ssize_t write_proc(struct file *filp, const char *buf, size_t count, loff_t *offp) {
    return 0;
}


int open_proc(struct inode *inode, struct file *file) {
    //return single_open(file, open_action, NULL);
	return single_open(file, cpu_g18_main, NULL);
}

static struct file_operations proc_fops = {
    read : seq_read,
    write : write_proc,
    open : open_proc,
    release : single_release,
    llseek : seq_lseek
};

static int __init cpu_grupo18_init(void) {
    printk(KERN_INFO "Hola mundo, somos el grupo 18 y este es el monitor de CPU.\n");

    struct proc_dir_entry *entry;
    entry =  proc_create("cpu_grupo18", 0777, NULL, &proc_fops);

    if (!entry) {
        return -1;
    }
    return 0;
}

static void __exit cpu_grupo18_exit(void) {
    printk(KERN_INFO "Sayonara mundo, somos el grupo 18 y este fue el monitor de CPU.\n");
    remove_proc_entry("cpu_grupo18", NULL);
}



module_init(cpu_grupo18_init);
module_exit(cpu_grupo18_exit);



MODULE_LICENSE("MIT");
MODULE_AUTHOR("Grupo 18");
MODULE_DESCRIPTION("Un monitor de CPU sencillo.");
MODULE_VERSION("0.01");