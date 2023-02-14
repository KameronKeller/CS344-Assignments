#include "llist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void node_printer(struct node *n, char *name) {
	printf("\t------\n");
	printf("\t%s address: %p\n", name, n);
	printf("\t%s value: %d\n", name, n->value);
	if (n->next == NULL) {
		printf("\t%s next: NULL\n", name);
	} else {
		printf("\t%s next: %d\n", name, n->next->value);
		printf("\t%s next address: %p\n", name, n->next);
	}
}

void llist_insert_head(struct node **head, struct node *n) {
	printf("insert head %d\n", n->value);
	if (*head == NULL) {
		*head = n;
	} else {
		// node_printer(*head, "head");
		struct node *temp = malloc(sizeof(struct node));
		temp->value = (*head)->value;
		temp->next = (*head)->next;
		(*head)->value = n->value;
		(*head)->next = n;
		n->value = temp->value;
		n->next = temp->next;
		node_free(temp);
		// temp->value = (*head)->value;
		// temp->next = (*head)->next;
		// printf("\ttemp = %d\n", temp->value);
		// (*head)->value = n->value;
		// (*head)->next = n;
		// printf("\thead is now: %d\n", (*head)->value);
		// printf("\thead points to NULL? %d\n", (*head)->next == NULL);
		// n->value = temp->value;
		// n->next = temp->next;
		// (*head)->next = n;
		// printf("\ttemp is : %d\n", temp->value);
		// printf("\tn is : %d\n", n->value);

		// node_printer(*head, "head");
		// node_printer(n, "n");
		// node_printer(temp, "temp");

		// node_free(temp);
	}

}

struct node *llist_delete_head(struct node **head) {
	if (*head == NULL) {
		return NULL;
	} else {
		struct node *temp = malloc(sizeof(struct node));
		struct node *next = (*head)->next;
		temp = *head;
		*head = next;
		return temp;
	}
}

void llist_insert_tail(struct node **head, struct node *n) {
	struct node *current = *head;
	if (*head == NULL) {
		*head = n;
	} else if (current->next == NULL) {
		current->next = n;
	} else {
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = n;
	}
}


void llist_print(struct node *head) {
	if (head == NULL) {
		printf("[empty]\n");
	} else {
		struct node *current = head;
		printf("%d", head->value);
		while (current->next != NULL) {
			current = current->next;
			printf(" -> %d", current->value);
		}
		printf("\n");
	}
}

void llist_free(struct node **head) {
	while (*head != NULL) {
		struct node *free_node = llist_delete_head(&*head);
		node_free(free_node);
	}


	// struct node *current = *head;
	// struct node *next = (*head)->next;
	// while (next != NULL) {
	// 	struct node *next = current->next;
	// 	node_free(current);
	// }
}


struct node *node_alloc(int value) {
	struct node *new_node = malloc(sizeof(struct node));

	if (new_node != NULL) {
		new_node->value = value;
		new_node->next = NULL;
	}

	// struct node new_node = {value, NULL};
	// return &(struct node){value, NULL};
	return new_node;
}

void node_free(struct node *n) {
	free(n);
}


int is_ih(char *arg) {
	return strcmp(arg, "ih") == 0 ? 1 : 0; // return true (1) if string matches
}

int is_it(char *arg) {
	return strcmp(arg, "it") == 0 ? 1 : 0; // return true (1) if string matches
}

int is_dh(char *arg) {
	return strcmp(arg, "dh") == 0 ? 1 : 0; // return true (1) if string matches
}

int is_f(char *arg) {
	return strcmp(arg, "f") == 0 ? 1 : 0; // return true (1) if string matches
}

int is_p(char *arg) {
	return strcmp(arg, "p") == 0 ? 1 : 0; // return true (1) if string matches
}


int main(int argc, char *argv[]) {
	const char *USAGE_STATEMENT = "Usage: [ih integer_value] [it integer_value] [dh] [f] [p]";

	if (argc < 2) {
		printf("%s\n", USAGE_STATEMENT);
		exit(1);
	}

	// struct node *head = node_alloc(1);
	// struct node *next_node = node_alloc(2);
	// head->next = next_node;

	struct node *head = NULL;


	for (int i = 1; i < argc; i++) {

		if (is_ih(argv[i])) {
			char *new_head_string_value = argv[i + 1];
			int new_head_value = atoi(new_head_string_value);

			struct node *n = node_alloc(new_head_value);
			llist_insert_head(&head, n);
			i++; // ih it takes two arguments, so increment to skip value arg on next loop
		} else if (is_it(argv[i])) {
			char *new_tail_string_value = argv[i + 1];
			int new_tail_value = atoi(new_tail_string_value);
			struct node *n = node_alloc(new_tail_value);

			llist_insert_tail(&head, n);

			i++; // it takes two arguments, so increment to skip arg on next loop
		} else if (is_dh(argv[i])) {
			struct node *deleted_head = llist_delete_head(&head);
		} else if (is_f(argv[i])) {
			llist_free(&head);
		} else if (is_p(argv[i])) {
			llist_print(head);
		} else {
			printf("%s\n", USAGE_STATEMENT);
			exit(1);
		}

	}




	return 0;
}
