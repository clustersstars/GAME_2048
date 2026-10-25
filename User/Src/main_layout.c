#include "main_layout.h"
// #include "st7735s.h"
#include "lvgl.h"

layout_obj_t main_layout;

extern const lv_img_dsc_t game_cover;
extern const lv_img_dsc_t title;

static lv_obj_t *main_screen = NULL;
static void generateChildELm(uint8_t flag);
static void freeChildELm(void);
static void keyEventHandler(key_type_t key);

void mainLayoutInit(void) {
    //底层棋盘背景
    main_screen = lv_obj_create(NULL);
    lv_obj_set_size(main_screen,128,160);
    lv_obj_set_style_bg_img_src(main_screen,&game_cover,0);

    lv_obj_set_style_border_width(main_screen, 0, 0);
    lv_obj_set_style_shadow_width(main_screen, 0, 0);
    lv_obj_set_style_pad_all(main_screen, 0, 0);
    lv_obj_clear_flag(main_screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_gap(main_screen, 0, 0);



    main_layout.obj = main_screen;
    main_layout.generateChildELm = generateChildELm;
    main_layout.freeChildELm     = freeChildELm;
    main_layout.keyEventHandler  = keyEventHandler;
}

static void generateChildELm(uint8_t flag) {
    lv_obj_t *box_1 = lv_obj_create(main_screen);
    lv_obj_t *box_2 = lv_obj_create(main_screen);
    lv_obj_set_style_pad_all(box_1, 0, 0);
    lv_obj_set_style_pad_all(box_2, 0, 0);

    lv_obj_set_flex_flow(main_screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_grow(box_1, 1);
    lv_obj_set_flex_grow(box_2, 1);

    lv_obj_set_style_bg_opa(box_1, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(box_1, 0, 0);
    lv_obj_set_style_shadow_width(box_1, 0, 0);

    lv_obj_set_style_bg_opa(box_2, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(box_2, 0, 0);
    lv_obj_set_style_shadow_width(box_2, 0, 0);

    //透明标题图片：父=屏幕，和img、box同级
    lv_obj_t *game_title = lv_label_create(box_1);
    lv_label_set_text(game_title,"2048");
    lv_obj_set_style_text_font(game_title,&lv_font_montserrat_14,0);
    lv_obj_align(game_title,LV_ALIGN_BOTTOM_MID,0,-35);

    //按钮放在下半区 box_2
    lv_obj_t* button = lv_btn_create(box_2);
    lv_obj_set_size(button, 60, 20);
    lv_obj_align_to(button,main_screen,LV_ALIGN_BOTTOM_MID,0,-5);
    lv_obj_t *label_button = lv_label_create(button);
    lv_label_set_text(label_button, "start");
    lv_obj_center(label_button);
}

static void freeChildELm(void) {
    lv_obj_clean(main_screen);
}

static void keyEventHandler(key_type_t key) {
    if (key == KEY_A) {
        switchLayout(&game_layout,0);
    }
}




