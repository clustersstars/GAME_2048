#ifndef GAME_DEF_H_
#define GAME_DEF_H_
#include "src/core/lv_obj.h"
#include "stdlib.h"

typedef enum {
    KEY_A = 1,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
}key_type_t;

typedef enum {
    MAIN_LAYOUT = 0,
    GAME_LAYOUT,
    CHECKOUT_LAYOUT
}layout_type_t;

typedef struct {
    lv_obj_t *obj;
    void (*generateChildELm)(uint8_t flag);
    void (*freeChildELm)(void);
    void (*keyEventHandler)(key_type_t key);
}layout_obj_t;

typedef struct {
    layout_type_t layout_type;
    layout_obj_t *active_layout_obj;
}layout_manage_t;

extern layout_obj_t main_layout;
extern layout_obj_t game_layout;
extern layout_obj_t checkout_layout;
extern layout_manage_t layout_manage;

void switchLayout(layout_obj_t *layout_obj,uint8_t child_flag);
#endif