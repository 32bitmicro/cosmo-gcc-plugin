#ifndef IFSWITCH_H
#define IFSWITCH_H
#include "headers.h"
#include "sub_usage.h"

#define IFSWITCH         "ifswitch"
#define IFSWITCH_VERSION "0.0.1"
#define IFSWITCH_HELP    "convert switch statements to if statements"

/* in process.cpp */
void process_body(tree *, subu_list *);

/* in utils.cpp */
const char *get_tree_code_str(tree);
int get_value_of_const(char *);
tree get_ifsw_identifier(char *);
int check_magic_equal(tree, char *);

void handle_start_tu(void *, void *);
void handle_finish_tu(void *, void *);
void handle_pre_genericize(void *, void *);
void handle_decl(void *, void *);

#endif /* IFSWITCH_H */
