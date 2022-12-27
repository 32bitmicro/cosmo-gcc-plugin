/*- mode:c++;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c++ ts=2 sts=2 sw=2 fenc=utf-8                              :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright © 2022, Gautham Venkatasubramanian                                 │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "ifswitch.h"

int plugin_is_GPL_compatible; /* ISC */

static struct plugin_name_args ifswitch_info = {
    .base_name = IFSWITCH, .version = IFSWITCH_VERSION, .help = IFSWITCH_HELP};

void at_decl(void *gcc_data, void *user_data) {
  tree t = (tree)gcc_data;
  tree t2;
  if (TREE_CODE(t) == FUNCTION_DECL) {
    /* this function is defined within the file I'm processing */
    printf("start_decl calling %s\n", IDENTIFIER_NAME(t));
    if (!strcmp("exam_func", IDENTIFIER_NAME(t))) {
      t2 = DECL_SAVED_TREE(t);
      // process_stmt(&t2);
      debug_tree(t2);
    }
  }
}

void print_debug(void *gcc_data, void *user_data) {
  tree t = (tree)gcc_data;
  tree t2;
  if (TREE_CODE(t) == FUNCTION_DECL && DECL_INITIAL(t) != NULL &&
      TREE_STATIC(t)) {
    /* this function is defined within the file I'm processing */
    printf("pre-genericize calling %s\n", IDENTIFIER_NAME(t));
    t2 = DECL_SAVED_TREE(t);
    process_stmt(&t2);
    // debug_tree(DECL_SAVED_TREE(t));
  }
}

int plugin_init(struct plugin_name_args *plugin_info,
                struct plugin_gcc_version *version) {
  if (!plugin_default_version_check(version, &gcc_version)) {
    fprintf(stderr, "GCC version incompatible!\n");
    return 1;
  }
  printf("Loading plugin %s on GCC %s...\n", plugin_info->base_name,
         version->basever);
  register_callback(plugin_info->base_name, PLUGIN_INFO, NULL, &ifswitch_info);
  register_callback(plugin_info->base_name, PLUGIN_START_PARSE_FUNCTION,
                    at_decl, NULL);
  register_callback(plugin_info->base_name, PLUGIN_PRE_GENERICIZE, print_debug,
                    NULL);
  return 0;
}