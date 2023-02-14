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
	if (*head == NULL) {
		*head = n;
	} else {
		struct node *temp = malloc(sizeof(struct node));

		temp->value = (*head)->value;
		temp->next = (*head)->next;

		(*head)->value = n->value;
		(*head)->next = n;

		n->value = temp->value;
		n->next = temp->next;

		node_free(temp);
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
}


struct node *node_alloc(int value) {
	struct node *new_node = malloc(sizeof(struct node));

	if (new_node != NULL) {
		new_node->value = value;
		new_node->next = NULL;
	}

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

	struct node *head = NULL;

	for (int i = 1; i < argc; i++) {
		char *command = argv[i];

		if (is_ih(command) || is_it(command)) {

			int argument = atoi(argv[i+1]);
			struct node *n = node_alloc(argument);

			if (is_ih(command)) {
				llist_insert_head(&head, n);
			} else if (is_it(command)) {
				llist_insert_tail(&head, n);
			}
			i++; // fast-forward i to skip over the argument on the next iteration
		} else if (is_dh(command)) {
			struct node *deleted_head = llist_delete_head(&head);
		} else if (is_f(command)) {
			llist_free(&head);
		} else if (is_p(command)) {
			llist_print(head);
		} else {
			printf("%s\n", USAGE_STATEMENT);
			exit(1);

		}

	}

	return 0;

}
