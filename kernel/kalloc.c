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

struct  {
  struct spinlock lock;
  int count[(PHYSTOP - KERNBASE) / PGSIZE];
} pageref;

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

int
kgetpagerefcount(uint64 pa)
{
  acquire(&pageref.lock);
  int count = pageref.count[(pa -  KERNBASE) / PGSIZE];
  release(&pageref.lock);
  return count;
}

void
kaddpagerefcount(uint64 pa)
{
  acquire(&pageref.lock);
  pageref.count[(pa -  KERNBASE) / PGSIZE]++;
  release(&pageref.lock);
}

void
kinit()
{
  initlock(&kmem.lock, "kmem");
  initlock(&pageref.lock, "pageref");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start);
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE){
    // page refrence count need init set to 1
    acquire(&pageref.lock);
    pageref.count[((uint64)p - KERNBASE) / PGSIZE] = 1;
    release(&pageref.lock);
    kfree(p);
  }
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

  // ref count is equal 0 than free physical page 
  acquire(&pageref.lock);
  if (--pageref.count[((uint64)pa - KERNBASE) / PGSIZE] > 0) {
    release(&pageref.lock);
    return;
  }
  release(&pageref.lock);

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
  if(r){
    acquire(&pageref.lock);
    pageref.count[((uint64)r - KERNBASE) / PGSIZE] = 1;
    release(&pageref.lock);
    kmem.freelist = r->next;
  }
  release(&kmem.lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
