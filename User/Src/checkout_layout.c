#include "checkout_layout.h"

layout_obj_t checkout_layout;
static lv_obj_t * checkout_screen;
static void generateChildELm(uint8_t flag);
static void freeChildELm(void);
static void keyEventHandler(key_type_t key);

void checkoutLayoutInit(void) {
    checkout_screen = lv_obj_create(NULL);
    lv_obj_set_size(checkout_screen, 128, 160);
    lv_obj_set_style_pad_all(checkout_screen,0,0);

    checkout_layout.obj = checkout_screen;
    checkout_layout.generateChildELm = generateChildELm;
    checkout_layout.freeChildELm     = freeChildELm;
    checkout_layout.keyEventHandler  = keyEventHandler;
}

static void generateChildELm(uint8_t flag) {
    lv_obj_t* label = lv_label_create(checkout_screen);
    if (flag) {
        lv_label_set_text(label,"you won");
    }else {
        lv_label_set_text(label,"game over");
    }
    lv_obj_set_align(label,LV_ALIGN_CENTER);
}

static void freeChildELm(void) {
    lv_obj_clean(checkout_screen);
}

static void keyEventHandler(key_type_t key) {
    if (key == KEY_A) {
        switchLayout(&game_layout,0);
    }
    if (key == KEY_B) {
        switchLayout(&main_layout,0);
    }
}