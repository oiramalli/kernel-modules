#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>        // pr_info
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
#include <linux/mm_types.h>

static int cpu_g18_main(struct seq_file *m, void *v)
{
    struct task_struct *procesos;
    size_t total_de_procesos = 0;
    size_t procesos_en_ejecucion = 0;
    size_t procesos_suspendidos = 0;
    size_t procesos_detenidos = 0;
    size_t procesos_zombies = 0;
    size_t memoria = 0;

    seq_printf(m, "{\n");
    seq_printf(m, "\"procs\" :[\n");
    char *coma = "";
    for_each_process(procesos)
    {
        long int ram = procesos->mm && procesos->mm->total_vm ? procesos->mm->total_vm : 0;
        memoria += ram;
        //char *estado = procesos->state == -1 ? "Inejecutable" : procesos->state == 0 ? "Ejecutable" : "Detenido";
        char *estado;
        if (procesos->state == -1)
        {
            estado = "Inejecutable";
        }
        else if (procesos->state == 0)
        {
            estado = "Ejecutando";
            ++procesos_en_ejecucion;
        }
        else if (procesos->exit_state == 16)
        {
            estado = "Zombie";
            ++procesos_zombies;
        }
        else if (procesos->exit_state == 32)
        {
            estado = "Suspendido";
            ++procesos_suspendidos;
        }
        else
        {
            estado = "Detenido";
            ++procesos_detenidos;
        }
        seq_printf(m, "%s{\"Pid\": %d,\n\"Nombre\": \"%s\",\n\"Usuario\": %d,\n\"Estado\": \"%s\",\n\"MemoriaUtilizada\": %ld,\n\"PorcentajeMemoriaUtilizada\": 0.0\n}", coma, procesos->pid, procesos->comm, procesos->cred->euid, estado, ram);
        coma = ",\n";
        ++total_de_procesos;
    }

    seq_printf(m, "],\n");
    seq_printf(m, "\"memoria\": %zu,\n", memoria);
    seq_printf(m, "\"total\": %zu,\n", total_de_procesos);
    seq_printf(m, "\"ejecucion\": %zu,\n", procesos_en_ejecucion); 
    seq_printf(m, "\"suspendidos\": %zu,\n", procesos_suspendidos);
    seq_printf(m, "\"detenidos\": %zu,\n", procesos_detenidos); 
    seq_printf(m, "\"zombies\": %zu\n", procesos_zombies);
    seq_printf(m, "}\n");
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

ssize_t write_proc(struct file *filp, const char *buf, size_t count, loff_t *offp)
{
    return 0;
}

int open_proc(struct inode *inode, struct file *file)
{
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

static int __init cpu_grupo18_init(void)
{
    printk(KERN_INFO "Hola mundo, somos el grupo 18 y este es el monitor de CPU.\n");

    struct proc_dir_entry *entry;
    entry = proc_create("cpu_grupo18", 0777, NULL, &proc_fops);

    if (!entry)
    {
        return -1;
    }
    return 0;
}

static void __exit cpu_grupo18_exit(void)
{
    printk(KERN_INFO "Sayonara mundo, somos el grupo 18 y este fue el monitor de CPU.\n");
    remove_proc_entry("cpu_grupo18", NULL);
}

module_init(cpu_grupo18_init);
module_exit(cpu_grupo18_exit);

MODULE_LICENSE("MIT");
MODULE_AUTHOR("Grupo 18");
MODULE_DESCRIPTION("[SO2] Modulo de cpu");
MODULE_VERSION("0.01");