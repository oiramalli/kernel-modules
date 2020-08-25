#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sys.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>


static int mem_g18_main(struct seq_file *m, void *v) {
	//variables que contendran la información del estado de la memoria
	int factorBytes;
	long total_ram;
	long free_ram;
	int used_ram;
	struct sysinfo info;	

	//se obtiene la información del estado de la memoria
	si_meminfo(&info);

	//se rellenan las variables
	factorBytes = info.mem_unit;
	total_ram = info.totalram * factorBytes /1000;
	free_ram = info.freeram * factorBytes / 1000;
	used_ram = total_ram - free_ram;

	//se escribe el archivo
	seq_printf(m, "{\n\"TotalKb\": %ld,\n\"AvailableKb\": %ld,\n\"UsedKb\": %d\n}\n", total_ram, free_ram, used_ram);
	return 0;
}

static int mem_proc_open(struct inode *inode, struct  file *file) {
     return single_open(file, mem_g18_main, NULL);
}

//operaciones a realizar
static struct file_operations memo_proc_fops = {
     .owner = THIS_MODULE,
     .open = mem_proc_open,
     .read = seq_read,
     .llseek = seq_lseek,
     .release = single_release,
};

static int memoria_grupo18_init(void){
	//Mostrar carnets
	printk(KERN_ALERT "Hola mundo, somos el grupo 18 y este es el monitor de memoria.\n");
	
	//Create proc file	
	proc_create("mem_grupo18", 0777, NULL, &memo_proc_fops);

	return 0;
}

static void memoria_grupo18_exit(void){
	//Remover proc entry
	remove_proc_entry("mem_grupo18", NULL);
	//Mostrar nombre del curso
	printk(KERN_ALERT "Sayonara mundo, somos el grupo 18 y este fue el monitor de memoria.\n");
}

module_init(memoria_grupo18_init);
module_exit(memoria_grupo18_exit);

//-------------- metadata -------------------------
MODULE_LICENSE("MIT");
MODULE_AUTHOR("Grupo 18");
MODULE_DESCRIPTION("[SO2] Modulo de memoria");
MODULE_LICENSE("GPL");
