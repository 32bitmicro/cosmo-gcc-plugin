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

int plugin_is_GPL_compatible;

static struct plugin_name_args ifswitch_info = {
    .base_name = IFSWITCH, .version = IFSWITCH_VERSION, .help = IFSWITCH_HELP};

/* FREE THE PARTS OF THIS AT PLUGIN_FINISH */
SubContext plugin_context;

void handle_finish(void *gcc_data, void *user_data) {
  SubContext *ctx = (SubContext *)user_data;
  if (ctx != &plugin_context) {
    fatal_error(MAX_LOCATION_T, "unable to clear plugin data!");
  } else {
    if (ctx->mods) {
      if (ctx->mods->count != 0) {
        for (auto it = ctx->mods->head; it; it = it->next) {
          error_at(it->loc, "unable to substitute constant\n");
        }
      }
      delete_subu_list(ctx->mods);
      ctx->mods = NULL;
    }
    ctx->prev = NULL;
    inform(UNKNOWN_LOCATION, "rewrote %u switch statements", ctx->switchcount);
    ctx->switchcount = 0;
    inform(UNKNOWN_LOCATION, "modified %u initializations", ctx->initcount);
    ctx->initcount = 0;
    inform(UNKNOWN_LOCATION, "modified %u other macro uses", ctx->subcount);
    ctx->subcount = 0;
  }
}

int plugin_init(struct plugin_name_args *plugin_info,
                struct plugin_gcc_version *version) {
  if (!plugin_default_version_check(version, &gcc_version)) {
    DEBUGF("GCC version incompatible!\n");
    return 1;
  }
  plugin_context.mods = init_subu_list();
  plugin_context.prev = NULL;
  plugin_context.switchcount = 0;
  plugin_context.initcount = 0;
  plugin_context.subcount = 0;

  DEBUGF("Loading plugin %s on GCC %s...\n", plugin_info->base_name,
         version->basever);
  register_callback(plugin_info->base_name, PLUGIN_INFO, NULL, &ifswitch_info);
  register_callback(plugin_info->base_name, PLUGIN_START_UNIT, handle_start_tu,
                    &plugin_context);
  register_callback(plugin_info->base_name, PLUGIN_PRE_GENERICIZE,
                    handle_pre_genericize, &plugin_context);
  register_callback(plugin_info->base_name, PLUGIN_FINISH_DECL, handle_decl,
                    &plugin_context);
  register_callback(plugin_info->base_name, PLUGIN_FINISH_UNIT,
                    handle_finish_tu, &plugin_context);
  register_callback(plugin_info->base_name, PLUGIN_FINISH, handle_finish,
                    &plugin_context);
  return 0;
}
