


// void *myalloc(int size);
// void myfree(void *p);

// void *heap = mmap(NULL, 1024, PROT_READ|PROT_WRITE,
//                   MAP_ANON|MAP_PRIVATE, -1, 0);



// // in my_malloc()



// struct block *cur;

// // ... All the machinations to allocate go here ...

// padded_block_size = PADDED_SIZE(sizeof(struct block));

// return PTR_OFFSET(cur, padded_block_size);


// padded_struct_block_size = PADDED_SIZE(sizeof(struct block));

// Assignment Description -------------------------- 
#include <stdio.h>
#include <sys/mman.h>

#define ALIGNMENT 16   // Must be power of 2
#define GET_PAD(x) ((ALIGNMENT - 1) - (((x) - 1) & (ALIGNMENT - 1)))
#define PADDED_SIZE(x) ((x) + GET_PAD(x))
#define PTR_OFFSET(p, offset) ((void*)((char *)(p) + (offset)))

struct block *head = NULL;  // Head of the list, empty

struct block {
    struct block *next;
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
        //printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
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

int is_large_enough(int size, struct block *node) {
	return size <= node->size;
}

int last_node(struct block *node) {
	return node->next == NULL;
}

struct block *get_sufficient_block(int size, struct block *node) {
	if (is_available(node) && is_large_enough(size, node)) {
		return node;
	} else if (last_node(node)) {
		return NULL;
	} else {
		return get_sufficient_block(size, node->next);
	}

}

void *myalloc(int size) {
	// void *heap = mmap(NULL, 1024, PROT_READ|PROT_WRITE,
    //               MAP_ANON|MAP_PRIVATE, -1, 0);

	if (head == NULL) {
	    head = mmap(NULL, 1024, PROT_READ|PROT_WRITE,
	                MAP_ANON|MAP_PRIVATE, -1, 0);
	    head->next = NULL;
	    head->size = 1024 - PADDED_SIZE(sizeof(struct block));
	    head->in_use = 0;
	}

	struct block *available_block = get_sufficient_block(size, head);

	if (available_block == NULL) {
		return NULL;
	} else {
		available_block->in_use = 1;
		int padded_block_size = PADDED_SIZE(sizeof(struct block));
		return PTR_OFFSET(available_block, padded_block_size);
	}

}


int main(void) {

	// struct block n3 = { NULL, 1000, 0 };
	// struct block n2 = { &n3, 5, 0 };
	// struct block n1 = { &n2, 10, 0 };

	// struct block *available = get_sufficient_block(20, &n1);
	// // printf("available? %d\n", is_available(&n1));
	// // printf("found? %d\n", available == NULL);
	// printf("found: %d\n", available->size);

	// void *p;

	// print_data();
	// p = myalloc(64);
	// print_data();

    void *p;

    print_data();
    p = myalloc(16);
    print_data();
    p = myalloc(16);
    printf("%p\n", p);

}


