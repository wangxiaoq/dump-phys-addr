#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <asm/pgtable.h>
#include <asm-generic/memory_model.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/migrate.h>
#include <linux/hugetlb.h>

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Wang Xiaoqiang <wang_xiaoq@126.com>");

static char* task_name = "NULL";
module_param(task_name, charp, S_IRUGO);

static struct task_struct *name_task_struct(char *name)
{
    struct task_struct *task;

    read_lock(&tasklist_lock);
    for_each_process(task)
        if (!strcmp(task->comm, name))
            return task;
    read_unlock(&tasklist_lock);

    return NULL;
}

struct page *virt2page(struct mm_struct *mm, unsigned long addr)
{
    pgd_t *pgd;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    struct page *page = NULL;;

    pgd = pgd_offset(mm, addr);
    if (pgd_none(*pgd) || pgd_bad(*pgd))
        return NULL;

    pud = pud_offset(pgd, addr);
    if (pud_none(*pud) || pud_bad(*pud))
        return NULL;

    pmd = pmd_offset(pud, addr);
    if (pmd_none(*pmd) || pmd_bad(*pmd))
        return NULL;

    if (!(pte = pte_offset_map(pmd, addr)))
        return NULL;
    if (pte_none(*pte))
        goto out;

    if (!pte_present(*pte))
        goto out;

    if (!(page = pte_page(*pte)))
        goto out;

out:
    pte_unmap(pte);

    return page;
}

static void dump_phy_page_address(struct task_struct *task)
{
    struct vm_area_struct *vma = NULL;
    unsigned long vpage;
    unsigned long pfn;

    if (task->mm && task->mm->mmap) {
        for (vma = task->mm->mmap; vma; vma = vma->vm_next)
            for (vpage = vma->vm_start; vpage < vma->vm_end; vpage += PAGE_SIZE) {
                struct page *p = virt2page(task->mm, vpage);
                if (p) {
                    pfn = page_to_pfn(p);
                    pr_info("linear address: 0x%lx, physical address: 0x%lx\n", vpage, (pfn<<PAGE_SHIFT));
                } else
                    pr_info("linear address: 0x%lx, physical address: not alloc\n", vpage);
            }
    }
}

static int __init change_pgtab_init(void)
{
	struct task_struct *t = name_task_struct(task_name);
	if (!t)
		return -EINVAL;
	dump_phy_page_address(t);
    pr_info("module insert\n");

    return 0;
}

static void __exit change_pgtab_exit(void)
{
    pr_info("module exit\n");
    return ;
}

module_init(change_pgtab_init);
module_exit(change_pgtab_exit);
