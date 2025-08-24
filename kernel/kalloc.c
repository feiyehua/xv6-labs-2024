// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;

  acquire(&kmem.lock);
  r->next = kmem.freelist;
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}

// Allocate one 2097152-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *
kalloc_superpage(void)
{
  struct run *pre_init = kmem.freelist;
  struct run *init = kmem.freelist;
  struct run *cur = init;
  int num = 1;
  acquire(&kmem.lock);
  while (num < (1<<9))
  {
    if (!cur)
    {
      goto bad;
    }
    // The current and the next page is contiguous
    // And properly aligned
    if ((uint64)((void *)cur - (void *)cur->next) == 4096 && !(((uint64)init + PGSIZE) & ((1 << 21) - 1)))
    {
      num++;
    }
    else
    {
      num = 1;
      init = cur->next;
      pre_init = cur;
    }
    cur = cur->next;
  }
  pre_init->next = cur->next;
bad:
  release(&kmem.lock);

  if (cur)
    memset((char *)cur, 5, PGSIZE * (1<<9)); // fill with junk
  return (void *)cur;
}

// Free the superpage of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void kfree_superpage(void *pa)
{
  struct run *r;

  if (((uint64)pa % SUPER_PGSIZE) != 0 || (char *)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree_superpage");

  // Fill with junk to catch dangling refs.
  memset(pa, 1, SUPER_PGSIZE);

  r = (struct run *)pa;

  acquire(&kmem.lock);
  for (int i = 0; i < (1<<9);i++)
  {
    r->next = kmem.freelist;
    kmem.freelist = r;
    r = (struct run *)((uint64)r + PGSIZE);
  }
  release(&kmem.lock);
}