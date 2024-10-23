#include "freeList.h"
#include <sys/mman.h>

const int MAGIC_NUMBER = 1234567;

freeList::freeList(long int *inRAM, int size)
{
  {
    // create a header and an item in the list that occupies all of ram

    head = inRAM;
    head[0] = size - 2;
    head[1] = NULL;
  }
}

long int *
freeList::reserve_space(int size) // malloc
{
  long int *sptr = NULL;

  long int *temp_head = (long int *)head;
  while (temp_head != NULL)
  {
    // temporarily store the data from the space we are freeing as we are about to overwrite it with the allocation header
    int curr_size = temp_head[0];
    long int pointer = temp_head[1];

    if (curr_size >= size + 2)
    {

      // overwrite the current data to be
      temp_head[1] = MAGIC_NUMBER;
      temp_head[0] = size;
      if (curr_size > size + 2)
      {

        // go to the new memory location and
        long int *new_free_block = temp_head + size + 2;
        new_free_block[0] = curr_size - size - 2;
        new_free_block[1] = pointer;

        head = new_free_block;
      }
      else
      {
        // if the size perfectly fits than we can just remove the entry and point
        // head to the next entry
        head = (long int *)pointer;
      }
      return temp_head + 2; // sptr
    }
    temp_head = (long int *)temp_head[1];
  }
  return sptr;
}

void freeList::free_space(long int *location)
{
  long int *temp_head = (long int *)head;
  long int *new_head = location - 2; // compute the header location from sptr
  long int *prev_head = NULL;

  while (temp_head != NULL && temp_head < new_head)
  {
    prev_head = temp_head;
    temp_head = (long int *)temp_head[1];
  }

  // set the pointer of our header to point to head and set head to our new entry
  new_head[1] = (long int)temp_head;

  if (prev_head == NULL)
  {
    head = new_head;
  }
  else
  {
    prev_head[1] = (long int)new_head; 
  }
}

void freeList::coalesce_forward()
{
  long int *temp_head = (long int *)head;
  while (temp_head != NULL)
  {
    long int *next_head = (long int *)temp_head[1];
    printf("computed pointer %p\n", temp_head + temp_head[0] + 2);
    printf("target %p\n", next_head);
    if (next_head != 0 && (temp_head + temp_head[0] + 2 == next_head))
    {
      temp_head[0] = temp_head[0] + next_head[0] + 2;
      temp_head[1] = next_head[1];
    }
    else
    {
      temp_head = (long int *)temp_head[1];
    }
  }
}

// print() is given to you. It traverses the free list and prints out what is there.
void freeList::print()
{
  cout << "printing" << "\n";
  long int *printHead = head;

  while (printHead != NULL)
  {
    cout << "at " << printHead << "(" << dec << printHead[0] << " : 0x" << hex << printHead[1] << ")\n";
    printHead = (long int *)printHead[1];
  }
}
/*
#include "freeList.h"
#include <sys/mman.h>

freeList::freeList(long int *inRAM, int size)
{
  // create a header and an item in the list that occupies all of ram

  head = (node_t *)mmap(NULL, size, PROT_READ | PROT_WRITE,
                        MAP_ANON | MAP_PRIVATE, -1, 0);
  head->size = size - sizeof(node_t);
  head->next = NULL;
  inRAM[0] =
}

long int *
freeList::reserve_space(int size) // malloc
{
  long int *loc = NULL;
  // start at the head of the list and iterate over it until we find an entry that
  // has a size that is less than the parameter size
  // subtract the size of the allocated space from the entry and update the entry
  // add the allocated header to RAM and
  // and then return the start of that piece of memory
  node_t *temp_head = (node_t *)head;
  while (temp_head != NULL)
  {
    int total_size = temp_head->size + 8;
    if (total_size == size) {
      // delete the node if the first fit is the correct size
    } else if (total_size > size) {
      temp_head->size = temp_head->size - size;
    }
    temp_head = (node_t *)temp_head->next;
  }
  return loc;
}

void freeList::free_space(long int *location) // free()
{
  // iterate through the list, checking whether the next entry has a location of more than the currenct loc
  // create a new entry with the
}

void freeList::coalesce_forward()
{
  // start at the beginning of the list
  // iterate over the list, if the next entry in the list is contiguous, merge the two
  // iterate
}

// print() is given to you. It traverses the free list and prints out what is there.
void freeList::print()
{
  node_t *printHead = (node_t *)head;

  while (printHead != NULL)
  {
    cout << "at " << printHead << "(" << dec << printHead->size << " : 0x" << hex << printHead->next << ")\n";
    printHead = (node_t *)printHead->next;
  }
}
*/