#include "game_layout.h"
#include "game_def.h"
#include <stdlib.h>

extern const lv_img_dsc_t screen_bg;
layout_obj_t game_layout;
//棋盘大小4*4
static lv_obj_t *game_screen = NULL;
static lv_obj_t *game_arr[4] = {0};
static lv_obj_t *item[4][4] = {0};
static lv_obj_t *item_label[4][4] = {0};
static int16_t buf[4][4] = {0};
static uint8_t random_pos = 0;

static const uint32_t tile_bg_color[11] = {
    0xF3A047,  // exp1 2：浅琥珀橙
    0xF08C38,  // exp2 4：亮橙
    0xED772C,  // exp3 8：暖橙
    0xE96426,  // exp4 16：橙红
    0xE35121,  // exp5 32：橘红
    0xD93F1E,  // exp6 64：正红橙
    0xCC3022,  // exp7 128：艳红
    0xBC2424,  // exp8 256：深红
    0xA81C26,  // exp9 512：酒红
    0x941828,  // exp10 1024：深酒红
    0x7F1428   // exp11 2048：暗莓红
};
static uint8_t is_game_over(int16_t buf[4][4]);
static uint8_t game_spawn_new(void);
static void refresh_game_ui(uint8_t new_val);
static void update_new_style(uint8_t new_val);
static void back_default_style(uint8_t old_val);
static void generateChildELm(uint8_t flag);
static void freeChildELm(void);
static void keyEventHandler(key_type_t key);

void gameLayoutInit(void) {
    //创建 屏幕
    game_screen = lv_obj_create(NULL);
    lv_obj_set_size(game_screen,128,160);
    //清除game_screen的pad
    lv_obj_set_style_pad_all(game_screen, 0,0);
    //设置无边框
    lv_obj_set_style_border_width(game_screen,0,0);
    //设置背景图
    lv_obj_set_style_bg_img_src(game_screen,&screen_bg,0);
    //清除滑动条
    lv_obj_clear_flag(game_screen, LV_OBJ_FLAG_SCROLLABLE);

    game_layout.obj = game_screen;
    game_layout.generateChildELm = generateChildELm;
    game_layout.freeChildELm     = freeChildELm;
    game_layout.keyEventHandler  = keyEventHandler;
}

uint8_t game_spawn_new(void) {
    int empty_x[16], empty_y[16];
    int cnt = 0;
    for(int r=0;r<4;r++){
        for(int c=0;c<4;c++){
            if(buf[r][c]==0){
                empty_x[cnt]=r;
                empty_y[cnt]=c;
                cnt++;
            }
        }
    }
    if(cnt ==0) return 0;
    int sel = rand() % cnt;
    int16_t val = (rand()%10 == 0) ? 4 : 2;
    buf[empty_x[sel]][empty_y[sel]] = val;
    return ((empty_x[sel]+1) << 4) | (empty_y[sel]+1);
}

static int row_left(int16_t row[4])
{
    int changed = 0;
    // 1. 压缩，去掉0
    int16_t temp[4] = {0};
    int idx = 0;
    for(int i = 0; i < 4; i++)
    {
        if(row[i] != 0)
        {
            temp[idx++] = row[i];
        }
    }
    // 2. 合并相邻相同
    for(int i = 0; i < 3; i++)
    {
        if(temp[i] != 0 && temp[i] == temp[i+1])
        {
            temp[i] *= 2;
            temp[i+1] = 0;
            changed = 1;
        }
    }
    // 3. 再次压缩，把合并产生的0挤到末尾
    int16_t res[4] = {0};
    idx = 0;
    for(int i = 0; i < 4; i++)
    {
        if(temp[i] != 0)
        {
            res[idx++] = temp[i];
        }
    }
    // 回填原行，并判断是否和原来不一样
    for(int i = 0; i < 4; i++)
    {
        if(row[i] != res[i])
            changed = 1;
        row[i] = res[i];
    }
    return changed;
}

void refresh_game_ui(uint8_t new_val) {
    uint8_t value_x = (new_val >> 4)-1;
    uint8_t value_y = (new_val & 0xf)-1;
    for (uint8_t r = 0; r < 4; r++) {
        for (uint8_t c = 0; c < 4; c++) {
            if (buf[r][c] != 0) {
                if (buf[r][c] == 2048) {
                    switchLayout(&checkout_layout,1);
                    return;
                }
                uint8_t exp = __builtin_ctz(buf[r][c]);
                lv_label_set_text_fmt(item_label[r][c],"%u",buf[r][c]);
                //设置背景风格
                if (value_x != r || value_y != c) {
                    lv_obj_set_style_bg_color(item[r][c],lv_color_hex(tile_bg_color[exp-1]),0);
                }
            }else {
                //恢复初始背景、字体
                if((r + c) % 2 == 0)
                {
                    lv_obj_set_style_bg_color(item[r][c],lv_color_hex(0x58c013),0);
                }
                else
                {
                    lv_obj_set_style_bg_color(item[r][c],lv_color_hex(0x92e013),0);
                }
                lv_label_set_text(item_label[r][c],"");
                lv_obj_center(item_label[r][c]);
                lv_obj_set_style_text_font(item_label[r][c], &lv_font_montserrat_10, 0);
            }
        }
    }
}

void update_new_style(uint8_t new_val) {
    uint8_t value_x = (new_val >> 4)-1;
    uint8_t value_y = (new_val & 0xf)-1;
    lv_obj_set_style_bg_color(item[value_x][value_y],lv_color_hex(0x0a8410),0);
    lv_obj_set_style_text_font(item_label[value_x][value_y],&lv_font_montserrat_12,0);
}

void back_default_style(uint8_t old_val) {
    if (old_val == 0) {
        return;
    }
    uint8_t value_x = (old_val >> 4)-1;
    uint8_t value_y = (old_val & 0xf)-1;
    if((value_x + value_y) % 2 == 0)
    {
        lv_obj_set_style_bg_color(item[value_x][value_y],lv_color_hex(0x58c013),0);
    }
    else
    {
        lv_obj_set_style_bg_color(item[value_x][value_y],lv_color_hex(0x92e013),0);
    }
    lv_obj_set_style_text_font(item_label[value_x][value_y], &lv_font_montserrat_10, 0);

}

static void generateChildELm(uint8_t flag) {
    //创建 game_box
    lv_obj_t *game_box = lv_obj_create(game_screen);
    //设置game_box大小
    lv_obj_set_size(game_box, 116,116);
    //设置pad为0
    lv_obj_set_style_pad_all(game_box, 0,0);
    //设置game_box的位置
    lv_obj_align_to(game_box,game_screen,LV_ALIGN_CENTER,0,0);
    //设置game_box边框为方形
    lv_obj_set_style_border_width(game_box,2,0);
    //设置半径为0
    lv_obj_set_style_radius(game_box, 0, 0);
    //设置边框颜色(白色)
    lv_obj_set_style_border_color(game_box,lv_color_hex(0xbef06e),0);
    //设置flex布局
    lv_obj_set_flex_flow(game_box,LV_FLEX_FLOW_COLUMN);
    //清除子元素的间隔
    lv_obj_set_style_pad_gap(game_box, 0, 0);
    //初始化game_arr
    for (uint8_t i=0;i<4;i++) {
        //创建game_arr
        game_arr[i] = lv_obj_create(game_box);
        //设置size
        lv_obj_set_size(game_arr[i],112,28);
        //清除pad
        lv_obj_set_style_pad_all(game_arr[i],0,0);
        //设置无边框
        lv_obj_set_style_border_width(game_arr[i],0,0);
        //设置半径为0
        lv_obj_set_style_radius(game_arr[i],0,0);
        //开启flex
        lv_obj_set_flex_flow(game_arr[i],LV_FLEX_FLOW_ROW);
        //清除子元素的间隔
        lv_obj_set_style_pad_gap(game_arr[i], 0, 0);
        for (uint8_t j=0;j<4;j++) {
            //创建item
            item[i][j] = lv_obj_create(game_arr[i]);
            //设置size
            lv_obj_set_size(item[i][j],28,28);
            //清除pad
            lv_obj_set_style_pad_all(item[i][j],0,0);
            //设置无边框
            lv_obj_set_style_border_width(item[i][j],0,0);
            //设置半径为0
            lv_obj_set_style_radius(item[i][j],0,0);
            //设置背景颜色
            if((i + j) % 2 == 0)
            {
                lv_obj_set_style_bg_color(item[i][j],lv_color_hex(0x58c013),0);
            }
            else
            {
                lv_obj_set_style_bg_color(item[i][j],lv_color_hex(0x92e013),0);
            }
            //创建label
            item_label[i][j] = lv_label_create(item[i][j]);
            lv_label_set_text(item_label[i][j],"");
            lv_obj_center(item_label[i][j]);
            lv_obj_set_style_text_font(item_label[i][j], &lv_font_montserrat_10, 0);
            lv_obj_set_style_text_color(item_label[i][j],lv_color_hex(0xffffff),0);
        }
    }
    //
    random_pos = game_spawn_new();
    if (random_pos == 0) {
        //没有空位,游戏结束
        switchLayout(&checkout_layout,0);
    }
    //更新ui
    update_new_style(random_pos);
    refresh_game_ui(random_pos);
}

static void freeChildELm(void) {
    lv_obj_clean(game_screen);
    memset(game_arr,0,sizeof(game_arr));
    memset(item,0,sizeof(item));
    memset(item_label,0,sizeof(item_label));
    memset(buf,0,sizeof(buf));
}

static void keyEventHandler(key_type_t key) {
    int total_change = 0;
    switch (key) {
        case KEY_A: {
            //重新开始游戏
            switchLayout(&game_layout,0);
        }break;
        case KEY_B: {
            //返回主界面
            switchLayout(&main_layout,0);
        }break;
        case KEY_C: {
            //上
            for(int c = 0; c < 4; c++)
            {
                // 取出一列放到临时行
                int16_t col[4];
                for(int r = 0; r < 4; r++)
                    col[r] = buf[r][c];

                int chg = row_left(col);

                // 写回列
                for(int r = 0; r < 4; r++)
                    buf[r][c] = col[r];

                if(chg) total_change = 1;
            }
        }break;
        case KEY_D: {
            //下
            for(int c = 0; c < 4; c++)
            {
                // 取出一列，上下反转
                int16_t col[4];
                for(int r = 0; r < 4; r++)
                    col[r] = buf[3 - r][c];

                int chg = row_left(col);

                // 反转写回
                for(int r = 0; r < 4; r++)
                    buf[r][c] = col[3 - r];

                if(chg) total_change = 1;
            }
        }break;
        case KEY_E: {
            //左
            for(int r = 0; r < 4; r++)
            {
                if(row_left(buf[r]))
                {
                    total_change = 1;
                }
            }
        }break;
        case KEY_F: {
            //右
            for(int r = 0; r < 4; r++)
            {
                // 反转行
                int16_t rev[4];
                for(int c = 0; c < 4; c++)
                    rev[c] = buf[r][3 - c];

                int chg = row_left(rev);

                // 反转回去
                for(int c = 0; c < 4; c++)
                    buf[r][c] = rev[3 - c];

                if(chg) total_change = 1;
            }
        }break;
    }
    if (total_change != 0) {
        //清理就样式
        back_default_style(random_pos);
        //生成新的值
        random_pos = game_spawn_new();
        if (is_game_over(buf) == 1) {
            //游戏结束
            switchLayout(&checkout_layout,0);
            return;
        }
        //更新ui
        update_new_style(random_pos);
        refresh_game_ui(random_pos);
    }
}

uint8_t is_game_over(int16_t buf[4][4])
{
    // 第一步：先查有没有空位，有空位直接没结束
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            if(buf[y][x] == 0)
            {
                return 0;
            }
        }
    }

    // 第二步：无空位，检查相邻能不能合并
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            // 检查右边
            if(x+1 < 4 && buf[y][x] == buf[y][x+1])
                return 0;
            // 检查下边
            if(y+1 < 4 && buf[y][x] == buf[y+1][x])
                return 0;
        }
    }
    // 没有空位 + 没有可合并 = 结束
    return 1;
}
