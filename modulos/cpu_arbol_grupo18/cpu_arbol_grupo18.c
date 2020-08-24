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

static int cpu_arbol_g18_main(struct seq_file *m, void *v)
{

	struct task_struct *task;
	struct task_struct *task_child;
	struct list_head *list;
	char *coma = "";
	seq_printf(m, "[\n");
	for_each_process(task)
	{
		seq_printf(m, "%s{\n", coma);
		seq_printf(m, "\"id\": %d,\n\"nombre\": \"%s\",\n\"hijos\":[\n", task->pid, task->comm);
		char *coma2 = "";
		list_for_each(list, &task->children)
		{
			task_child = list_entry(list, struct task_struct, sibling);
			seq_printf(m, "%s{\"id\": %d,\n \"nombre\": \"%s\",\n\"hijos\":[]}", coma2, task_child->pid, task_child->comm);
			coma2 = ",\n";
		}
		seq_printf(m, "]\n}");
		coma = ",\n";
	}
	seq_printf(m, "]\n");
	return 0;
}

static int cpu_arbol_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, cpu_arbol_g18_main, NULL);
}

//operaciones a realizar
static struct file_operations cpu_arbol_proc_fops = {
	.owner = THIS_MODULE,
	.open = cpu_arbol_proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int cpu_arbol_grupo18_init(void)
{
	//Mostrar carnets
	printk(KERN_ALERT "Hola mundo, somos el grupo 18 y este es el monitor de cpu de arbol.\n");

	//Create proc file
	proc_create("cpu_arbol_grupo18", 0777, NULL, &cpu_arbol_proc_fops);

	return 0;
}

static void cpu_arbol_grupo18_exit(void)
{
	//Remover proc entry
	remove_proc_entry("cpu_arbol_grupo18", NULL);
	//Mostrar nombre del curso
	printk(KERN_ALERT "Sayonara mundo, somos el grupo 18 y este fue el monitor de cpu de arbol.\n");
}

module_init(cpu_arbol_grupo18_init);
module_exit(cpu_arbol_grupo18_exit);

//-------------- metadata -------------------------
MODULE_LICENSE("MIT");
MODULE_AUTHOR("Grupo 18");
MODULE_DESCRIPTION("[SO2] Modulo de cpu para arbol");
MODULE_LICENSE("GPL");
