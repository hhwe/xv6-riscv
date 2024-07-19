### 6.1. Comment out the calls to acquire and release in kalloc (kernel/kalloc.c:69). This seems like it should cause problems for kernel code that calls kalloc; what symptoms do you expect to see? When you run xv6, do you see these symptoms? How about when running usertests? If you don’t see a problem, why not? See if you can provoke a problem by inserting dummy loops into the critical section of kalloc.

代码如下:

```c
// 未加锁版本kalloc
void *
kalloc(void)
{
  struct run *r;

  // acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  // release(&kmem.lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}

// 汇编代码
0000000080000afa <kalloc>:
    80000afa:	1101                	addi	sp,sp,-32
    80000afc:	ec06                	sd	ra,24(sp)
    80000afe:	e822                	sd	s0,16(sp)
    80000b00:	e426                	sd	s1,8(sp)
    80000b02:	1000                	addi	s0,sp,32
    80000b04:	00010497          	auipc	s1,0x10
    80000b08:	0544b483          	ld	s1,84(s1) # 80010b58 <kmem+0x18>
    80000b0c:	cc89                	beqz	s1,80000b26 <kalloc+0x2c>
    80000b0e:	609c                	ld	a5,0(s1)
    80000b10:	00010717          	auipc	a4,0x10
    80000b14:	04f73423          	sd	a5,72(a4) # 80010b58 <kmem+0x18>
    80000b18:	6605                	lui	a2,0x1
    80000b1a:	4595                	li	a1,5
    80000b1c:	8526                	mv	a0,s1
    80000b1e:	00000097          	auipc	ra,0x0
    80000b22:	1ea080e7          	jalr	490(ra) # 80000d08 <memset>
    80000b26:	8526                	mv	a0,s1
    80000b28:	60e2                	ld	ra,24(sp)
    80000b2a:	6442                	ld	s0,16(sp)
    80000b2c:	64a2                	ld	s1,8(sp)
    80000b2e:	6105                	addi	sp,sp,32
    80000b30:	8082                	ret
```

1. 两个cpu T1\T2同时执行到这个函数 13行(`r = kmem.freelist;`), 此时, 两个cpu的 `r` 都执行一块未分配物理内存
2. 如果一个cpu T1执行比较块, T1执行到了19行(`memset((char*)r, 5, PGSIZE);`), 将 `r` 执行内存刷成0x0505050505050505
3. 第二个cpu T2, 开始执行14行, 此时 `r` 内存是非法地址 0x0505050505050505, 不再kernel的内存映射内, pagetable 找不到
4. 发生异常, 进入trap, os panic.

执行结果:

``` sh
test reparent2: scause 0x000000000000000d
sepc=0x0000000080000b0e stval=0x0505050505050505
panic: kerneltrap
```


## lock


``` c
struct semaphore
{
  struct spinlock lock;
  int count;
};

void V(struct semaphore *s)
{
  acquire(&s->lock);
  s->count++;
  release(&s->lock);
}

void P(struct semaphore *s)
{
  while (s->count == 0)
    ; // 循环浪费cpu
  acquire(&s->lock);
  s->count--;
  release(&s->lock);
}

void V(struct semaphore *s)
{
  acquire(&s->lock);
  s->count++;
  wakeup(s);
  release(&s->lock);
}

void P(struct semaphore *s)
{
  while (s->count == 0)
    sleep(s); // 虚假唤醒
  acquire(&s->lock);
  s->count--;
  release(&s->lock);
}

void P(struct semaphore *s)
{
  acquire(&s->lock);
  while (s->count == 0)
    sleep(s); // 死锁
  s->count--;
  release(&s->lock);
}

void P(struct semaphore *s)
{
  acquire(&s->lock);
  while (s->count == 0)
    sleep(s, &s->lock); // 线程切换前释放锁,切换后获取锁,避免死锁
  s->count--;
  release(&s->lock);
}

void P(struct semaphore *s)
{
  while (1) {
    acquire(&lock);
    if (s->count == 0) {
      sleep(s);
    } else {
      break;
    }
    release(&lock);
  }
  acquire(&s->lock);
  s->count--;
  release(&s->lock);
}
```

