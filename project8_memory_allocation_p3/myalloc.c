/*
Author: Kameron Keller
Course: CS 344 OS I, Winter 2023
Assignment: Project 8
*/

#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

#define ALIGNMENT 16   // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x) - 1) & (ALIGNMENT - 1)))
#define PADDED_SIZE(x) ((x) + GET_PAD(x))
#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))

struct block *head = NULL;  // Head of the list, empty

struct block {
	struct block *next;
	struct block *prev;
	int size;     // Bytes
	int in_use;   // Boolean
};

void print_data(void) {
	struct block *b = head;

	if (b == NULL) {
		printf("[empty]\n");
		return;
	}

	while (b != NULL) {
		// Uncomment the following line if you want to see the pointer values
		// printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
		printf("[%d,%s]", b->size, b->in_use? "used": "free");
		if (b->next != NULL) {
			printf(" -> ");
		}

		b = b->next;
	}

	printf("\n");
}

int is_available(struct block *node) {
	return node->in_use ? 0 : 1;
}

int is_large_enough(int padded_requested_space, struct block *node) {
	return padded_requested_space <= node->size;
}

int is_splittable(struct block *node, int padded_requested_space) {
	int available_space = node->size;
	int padded_struct_block_size = PADDED_SIZE(sizeof(struct block));

	// required space = padded requested space + header of current node + header of future free node
	int required_space = padded_requested_space + (padded_struct_block_size * 2);

	return available_space >= required_space ? 1 : 0;
}

int last_node(struct block *node) {
	return node->next == NULL;
}

void mark_in_use(struct block **node) {
	(*node)->in_use = 1;
}

void mark_not_in_use(struct block **node) {
	(*node)->in_use = 0;
}

void *pointer_offset(struct block *node) {
	int padded_block_size = PADDED_SIZE(sizeof(struct block));
	return PTR_OFFSET(node, padded_block_size);
}

void split_space(struct block **node, int padded_requested_space) {
	int remaining_space = (*node)->size - padded_requested_space - PADDED_SIZE(sizeof(struct block));

	struct block *new_block = pointer_offset(*node) + padded_requested_space;
	new_block->next = NULL;
	new_block->prev = *node;
	new_block->size = remaining_space;
	new_block->in_use = 0;

	(*node)->next = new_block;
	(*node)->size = padded_requested_space;
	(*node)->in_use = 1;
}

struct block *get_sufficient_block(int padded_requested_space, struct block *node) {
	if (is_available(node) && is_large_enough(padded_requested_space, node)) {
		if (is_splittable(node, padded_requested_space)) {
			split_space(&node, padded_requested_space);
		}
		return node;
	} else if (last_node(node)) {
		return NULL;
	} else {
		return get_sufficient_block(padded_requested_space, node->next);
	}
}

void setup_initial_memory(int num_bytes, struct block **head) {
	*head = mmap(NULL, num_bytes, PROT_READ|PROT_WRITE,
				MAP_ANON|MAP_PRIVATE, -1, 0);
	(*head)->next = NULL;
	(*head)->prev = NULL;
	(*head)->size = num_bytes - PADDED_SIZE(sizeof(struct block));
	(*head)->in_use = 0;
}

void *myalloc(int requested_space) {
	int padded_requested_space = PADDED_SIZE(requested_space);
	if (head == NULL) {
		setup_initial_memory(1024, &head);
	}

	struct block *available_block = get_sufficient_block(padded_requested_space, head);

	if (available_block == NULL) { // if no available block
		return NULL;
	} else {
		mark_in_use(&available_block);
		return pointer_offset(available_block);
	}
}

void merge_blocks(struct block **current_block) {
	if (*current_block != NULL & (*current_block)->in_use == 0){
		struct block *next = (*current_block)->next;
		if (next != NULL & next->in_use == 0) {
			(*current_block)->size = (*current_block)->size + next->size + PADDED_SIZE(sizeof(struct block));
			if (next->next != NULL){
				(*current_block)->next = next->next;
				next->next->prev = (*current_block);
			} else {
				(*current_block)->next = NULL;
			}
		}

	}
}

void myfree(void *p) {
	void *header_address = p - PADDED_SIZE(sizeof(struct block));

	struct block *freed_block = (struct block *) header_address;
	mark_not_in_use(&freed_block);
	merge_blocks(&freed_block);
	if (freed_block->prev != NULL) {
		merge_blocks(&freed_block->prev);
	}
}


int main(void) {

	// ====== Example 1 ======
	void *p;

	p = myalloc(10); print_data();

	myfree(p); print_data();

	// ====== Example 2 ======
	head = NULL;

	p = NULL;
	void *q;

	p = myalloc(10); print_data();
	q = myalloc(20); print_data();

	myfree(p); print_data();
	myfree(q); print_data();

	// ====== Example 3 ======
	head = NULL;

	p = NULL; 
	q = NULL;

	p = myalloc(10); print_data();
	q = myalloc(20); print_data();

	myfree(q); print_data();
	myfree(p); print_data();

	// ====== Example 4 ======
	head = NULL;

	void *r, *s;
	p = NULL; 
	q = NULL;

	p = myalloc(10); print_data();
	q = myalloc(20); print_data();
	r = myalloc(30); print_data();
	s = myalloc(40); print_data();

	myfree(q); print_data();
	myfree(p); print_data();
	myfree(s); print_data();
	myfree(r); print_data();


}


