#include "monitor/watchpoint.h"
#include "monitor/expr.h"
#include "color.h"
#include <stdlib.h>

#define NR_WP 32

static WP wp_list[NR_WP];
static WP *head, *head_, *free_;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].expr = NULL;
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = wp_list;
	head_ = NULL;
	free_ = wp_list;
}

WP *new_wp() {
	if(free_->next == NULL) {
		assert(0);
	}
	head_ = free_;
	free_ = free_->next;
	return head_;
}

void free_wp(WP *wp) {

    free(wp->expr);
    wp->expr = NULL;
	free_ = wp;
}

void add_wp(char* args) {
	bool succ;
	WP* wp = new_wp();
	uint32_t value = expr(args, &succ);
	if(succ) {
		wp->expr = (char *)malloc(sizeof(char) * (strlen(args) + 1));
		strcpy(wp->expr, args);
		wp->value = value;
		printf("The watchpoint is set.\n");
	}
}

int del_wp(int NO) {
	if(wp_list[NO].expr == NULL) {
		printf("This is a empty watchpoint\n");
		return 0;
	}
	if(head_ == &wp_list[NO]) {
		free_ = head_;
		if(head_ == head) {
			head_ = NULL;
		}
		else {
			head_ = &wp_list[NO - 1];
		}
		free(wp_list[NO].expr);
	    wp_list[NO].expr = NULL;

		return 0;
	}
	else {
		wp_list[NO - 1].next = wp_list[NO].next;
		wp_list[NO].next = free_;
		free(wp_list[NO].expr);

	    wp_list[NO].expr = NULL;
		free_ = &wp_list[NO];
		head_->next = free_;

		return 0;
	}
}

int show_all_wp() {
	WP* wp = head;
	if(head_ == NULL) {
		return 0;
	}
	printf(GREEN"+----+----------------------------------------------------------+"NONE);
	printf(GREEN"-----------------------+\n"NONE);
	printf(GREEN"|"NONE \
			LIGHT_RED" NO "NONE \
			GREEN"|"NONE \
			LIGHT_RED"\t\t\t\tExpr\t\t\t\t"NONE \
			GREEN"|"NONE \
			LIGHT_RED"\tVuale\t\t"NONE \
			GREEN"|\n"NONE);
	printf(GREEN"+----+----------------------------------------------------------+"NONE);
	printf(GREEN"-----------------------+\n"NONE);
	do{
		if(wp->expr != NULL) {
			printf(GREEN"|"NONE \
					LIGHT_CYAN" %-3d"NONE \
					GREEN"|"NONE \
					LIGHT_PURPLE"  %-56s"NONE \
					GREEN"|"NONE \
					LIGHT_PURPLE"\t%-16d"NONE \
					GREEN"|\n"NONE,
					wp->NO, wp->expr, wp->value);
			printf(GREEN"+----+----------------------------------------------------------+"NONE);
			printf(GREEN"-----------------------+\n"NONE);
		}
		wp = wp->next;
	}while(wp != NULL);
	return 0;
}

/* Implement the functionality of watchpoint */
