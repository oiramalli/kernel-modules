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
	total_ram = info.totalram * factorBytes;
	free_ram = info.freeram * factorBytes;
	used_ram = ((total_ram - free_ram)* 100)/total_ram ;

	//se escribe el archivo
	seq_printf(m, "Cantidad total de memoria: %ld bytes\nCantidad de memoria disponible: %ld bytes\nMemoria utilizada: %d por ciento\n", total_ram, free_ram, used_ram);
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

static int memo_init(void){
	//Mostrar carnets
	printk(KERN_ALERT "Hola mundo, somos el grupo 18 y este es el monitor de memoria.\n");
	
	//Create proc file	
	proc_create("mem_grupo18", 0, NULL, &memo_proc_fops);

	return 0;
}

static void memo_exit(void){
	//Remover proc entry
	remove_proc_entry("mem_grupo18", NULL);
	//Mostrar nombre del curso
	printk(KERN_ALERT "Sayonara mundo, somos el grupo 18 y este fue el monitor de memoria.\n");
}

module_init(memo_init);
module_exit(memo_exit);

//-------------- metadata -------------------------
MODULE_AUTHOR("Mario Alvarado");
MODULE_DESCRIPTION("[SO2] Modulo de memoria");
MODULE_LICENSE("GPL");
