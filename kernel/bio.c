// Buffer cache.
//
// The buffer cache is a linked list of buf structures holding
// cached copies of disk block contents.  Caching disk blocks
// in memory reduces the number of disk reads and also provides
// a synchronization point for disk blocks used by multiple processes.
//
// Interface:
// * To get a buffer for a particular disk block, call bread.
// * After changing buffer data, call bwrite to write it to disk.
// * When done with the buffer, call brelse.
// * Do not use the buffer after calling brelse.
// * Only one process at a time can use a buffer,
//     so do not keep them longer than necessary.


#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "riscv.h"
#include "defs.h"
#include "fs.h"
#include "buf.h"

#define NBUCKET 13
#define BUCKET_HASH(x) ((x) % NBUCKET)

extern uint ticks;

struct {
  struct spinlock lock;
  struct buf buf[NBUF];

  struct spinlock bucketlock[NBUCKET];
  struct buf bucket[NBUCKET];
} bcache;

void
binit(void)
{
  struct buf *b;

  initlock(&bcache.lock, "bcache");

  // Init bucket list of buffers
  for (b = bcache.bucket; b < bcache.bucket + NBUCKET; b++) {
    b->prev = b;
    b->next = b;
    initsleeplock(&b->lock, "bcache.bucket");
  }

  // Create bucket list of buffers
  for(b = bcache.buf; b < bcache.buf+NBUF; b++){
    int bucketno = BUCKET_HASH(b->blockno);
    b->ticks = ticks;
    b->next = bcache.bucket[bucketno].next;
    b->prev = &bcache.bucket[bucketno];
    initsleeplock(&b->lock, "buffer");
    bcache.bucket[bucketno].next->prev = b;
    bcache.bucket[bucketno].next = b;
  }
}

// Look through buffer cache for block on device dev.
// If not found, allocate a buffer.
// In either case, return locked buffer.
static struct buf*
bget(uint dev, uint blockno)
{
  struct buf *b;
  struct buf *fb = 0;
  uint min_ticks = __INT32_MAX__;

  // if cached
  int bucketno = BUCKET_HASH(blockno);
  acquire(&bcache.bucketlock[bucketno]);
  for(b = bcache.bucket[bucketno].next; b != &bcache.bucket[bucketno]; b = b->next){
    if(b->dev == dev && b->blockno == blockno){
      b->refcnt++;
      release(&bcache.bucketlock[bucketno]);
      acquiresleep(&b->lock);
      return b;
    }
  }
  // if (fb) {
  //   fb->dev = dev;
  //   fb->blockno = blockno;
  //   fb->valid = 0;
  //   fb->refcnt = 1;
  //   fb->ticks = ticks;
  //   release(&bcache.bucketlock[bucketno]);
  //   acquiresleep(&fb->lock);
  //   return fb;
  // }
  // release(&bcache.bucketlock[bucketno]);

  // acquire(&bcache.lock);
  // if not cached, reusing other bucket
  for(int i = BUCKET_HASH(bucketno + 1); i != bucketno; i = BUCKET_HASH(i + 1)){
  {
    acquire(&bcache.bucketlock[i]);
    for(b = bcache.bucket[i].next; b != &bcache.bucket[i]; b = b->next){
      if(b->refcnt == 0 && b->ticks < min_ticks) {
        fb = b;
        min_ticks = fb->ticks;
      }
    }
    if (!fb) {
      release(&bcache.bucketlock[i]);
      continue;
    }
    b = fb;
    b->dev = dev;
    b->blockno = blockno;
    b->valid = 0;
    b->refcnt = 1;

    // remove b from i bucket
    b->prev->next = b->next;
    b->next->prev = b->prev;
    // insert b to blockno bucket
    b->next = bcache.bucket[bucketno].next;
    b->prev = &bcache.bucket[bucketno];
    bcache.bucket[bucketno].next->prev = b;
    bcache.bucket[bucketno].next = b;

    release(&bcache.bucketlock[i]);
    release(&bcache.bucketlock[bucketno]);
    // release(&bcache.lock);
    acquiresleep(&b->lock);
    return b;
    }
  }
  // release(&bcache.lock);

  panic("bget: no buffers");
}

// Return a locked buf with the contents of the indicated block.
struct buf*
bread(uint dev, uint blockno)
{
  struct buf *b;

  b = bget(dev, blockno);
  if(!b->valid) {
    virtio_disk_rw(b, 0);
    b->valid = 1;
  }
  return b;
}

// Write b's contents to disk.  Must be locked.
void
bwrite(struct buf *b)
{
  if(!holdingsleep(&b->lock))
    panic("bwrite");
  virtio_disk_rw(b, 1);
}

// Release a locked buffer.
// Move to the head of the most-recently-used list.
void
brelse(struct buf *b)
{
  if(!holdingsleep(&b->lock))
    panic("brelse");

  releasesleep(&b->lock);

  int bucketno = BUCKET_HASH(b->blockno);
  acquire(&bcache.bucketlock[bucketno]);
  b->refcnt--;
  if (b->refcnt == 0){
    b->ticks = ticks;
  }
  
  // if (b->refcnt == 0) {
  //   // no one is waiting for it.
  //   b->next->prev = b->prev;
  //   b->prev->next = b->next;
  //   b->next = bcache.bucket[bucketno].next;
  //   b->prev = &bcache.bucket[bucketno];
  //   bcache.bucket[bucketno].next->prev = b;
  //   bcache.bucket[bucketno].next = b;
  // }
  release(&bcache.bucketlock[bucketno]);
}

void
bpin(struct buf *b) {
  int bucketno = BUCKET_HASH(b->blockno);
  acquire(&bcache.bucketlock[bucketno]);
  b->refcnt++;
  release(&bcache.bucketlock[bucketno]);
}

void
bunpin(struct buf *b) {
  int bucketno = BUCKET_HASH(b->blockno);
  acquire(&bcache.bucketlock[bucketno]);
  b->refcnt--;
  release(&bcache.bucketlock[bucketno]);
}


