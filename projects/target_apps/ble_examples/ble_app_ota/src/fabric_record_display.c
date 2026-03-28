/**
 ****************************************************************************************
 *
 * @file fabric_record_display.c
 *
 * @brief Layout implementation for Fabric Relaxing Record display mode
 *
 ****************************************************************************************
 */

#include "fabric_record_display.h"
#include "EPD_2in13_V2.h"
#include "Fonts/fonts.h"
#include "GUI_Paint.h"
#include "fonts.h"
#include <stdio.h>
#include <string.h>

extern uint8_t epd_buffer[];

static void draw_field(uint16_t x, uint16_t y, const char *label,
                       const char *value, uint8_t max_value_len) {
  char line[32];
  size_t value_len = strlen(value);

  if (value_len > max_value_len) {
    value_len = max_value_len;
  }

  snprintf(line, sizeof(line), "%s:%.*s", label, (int)value_len, value);
  EPD_DrawUTF8(x, y, 0, line, EPD_ASCII_Font8, 0, BLACK, WHITE);
}

void draw_fabric_record_page(const fabric_record_t *record, bool force_redraw) {
  (void)force_redraw;

  Paint_NewImage(epd_buffer, EPD_2IN13_V2_WIDTH, EPD_2IN13_V2_HEIGHT, 270,
                 WHITE);
  Paint_SelectImage(epd_buffer);
  Paint_SetMirroring(MIRROR_VERTICAL);
  Paint_Clear(WHITE);

  Paint_DrawRectangle(1, 1, 211, 103, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Paint_DrawRectangle(2, 2, 170, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
  Paint_DrawRectangle(3, 3, 169, 19, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
  EPD_DrawUTF8(8, 6, 0, "THE KHO VAI", EPD_ASCII_Font8, 0, BLACK, WHITE);
  Paint_DrawLine(106, 23, 106, 100, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

  draw_field(4, 25, "W", record->width, 13);
  draw_field(4, 37, "PO", record->po, 12);
  draw_field(4, 49, "IT", record->item, 12);
  draw_field(4, 61, "LO", record->lot, 12);
  draw_field(4, 73, "NV", record->staff, 12);
  draw_field(4, 85, "XA", record->relax_date, 12);

  draw_field(110, 25, "OK", record->ok_date, 12);
  draw_field(110, 37, "CO", record->color, 12);
  draw_field(110, 49, "BU", record->buy, 12);
  draw_field(110, 61, "RO", record->roll, 12);
  draw_field(110, 73, "YD", record->yds, 12);
  draw_field(110, 85, "NT", record->note, 12);
}
