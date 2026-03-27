/**
 ****************************************************************************************
 *
 * @file calendar_display_modified.c
 *
 * @brief Modified calendar display with analog clock functionality
 *
 ****************************************************************************************
 */

#include "calendar_display.h"
#include "analog_clock.h"
#include "GUI_Paint.h"
#include "etime.h"
#include "fonts.h"

// Bảng số ngày trong tháng (năm thường và năm nhuận)
static const uint8_t days_in_month[2][12] = {
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, // Năm thường
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}  // Năm nhuận
};

// Tên các thứ trong tuần
static const char* week_names_vi[] = {"CN", "T2", "T3", "T4", "T5", "T6", "T7"};

/**
 * @brief Lấy thứ của ngày đầu tiên trong tháng
 * @param year Năm
 * @param month Tháng (1-12)
 * @return Thứ trong tuần (0=Chủ Nhật, 1=Thứ Hai, ..., 6=Thứ Bảy)
 */
static uint8_t get_first_day_of_month(uint16_t year, uint8_t month)
{
    return day_of_week_get(month, 1, year);
}

/**
 * @brief Lấy số ngày trong tháng
 * @param year Năm
 * @param month Tháng (1-12)
 * @return Số ngày của tháng đó
 */
static uint8_t get_days_in_month(uint16_t year, uint8_t month)
{
    uint8_t leap = is_leap(year);
    return days_in_month[leap][month - 1];
}

/**
 * @brief Vẽ tiêu đề lịch (năm tháng)
 * @param year Năm
 * @param month Tháng
 */
static void draw_calendar_title(uint16_t year, uint8_t month)
{
    char title_buf[20];
    sprintf(title_buf, "%d / %d", year, month);
    
    // Hiển thị tiêu đề căn giữa ở đầu màn hình
    EPD_DrawUTF8(20, 2, 0, title_buf, EPD_ASCII_11X16, EPD_FontUTF8_16x16, BLACK, WHITE);
}

/**
 * @brief Vẽ dòng tiêu đề các thứ trong tuần
 */
static void draw_week_header(void)
{
    uint8_t x_start = 10;
    uint8_t y_pos = 20;
    uint8_t cell_width = 16;
    
    for (uint8_t i = 0; i < 7; i++)
    {
        uint8_t x_pos = x_start + i * cell_width;
        EPD_DrawUTF8(x_pos + 8, y_pos, 0, week_names_vi[i], EPD_ASCII_7X12, EPD_FontUTF8_16x16, BLACK, WHITE);
    }
}
 
/**
 * @brief Vẽ các con số ngày
 * @param year Năm
 * @param month Tháng
 * @param current_day Ngày hiện tại (dùng để làm nổi bật)
 */
static void draw_calendar_dates(uint16_t year, uint8_t month, uint8_t current_day)
{
    uint8_t x_start = 10;
    uint8_t y_start = 40;
    uint8_t cell_width = 20;
    uint8_t cell_height = 10;
    
    uint8_t first_day = get_first_day_of_month(year, month);
    uint8_t days_count = get_days_in_month(year, month);
    
    char day_buf[3];
    uint8_t row = 0;
    uint8_t col = first_day; // Bắt đầu từ thứ tương ứng của ngày đầu tiên
    
    for (uint8_t day = 1; day <= days_count; day++)
    {
        uint8_t x_pos = x_start + col * cell_width + 8;
        uint8_t y_pos = y_start + row * cell_height + 2;
        
        sprintf(day_buf, "%d", day);
        
        // Nếu là ngày hiện tại, sử dụng hiển thị nghịch màu
        if (day == current_day)
        {
            // Vẽ hình chữ nhật nền
            Paint_DrawRectangle(x_start + col * cell_width + 4, 
                              y_start + row * cell_height + 3,
                              x_start + (col + 1) * cell_width - 2,
                              y_start + (row + 1) * cell_height+ 3,
                              BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
            
            // Phông chữ trắng
            EPD_DrawUTF8(x_pos, y_pos, 0, day_buf, EPD_ASCII_7X12, 0, WHITE, BLACK);
        }
        else
        {
            // Phông chữ đen bình thường
            EPD_DrawUTF8(x_pos, y_pos, 0, day_buf, EPD_ASCII_7X12, 0, BLACK, WHITE);
        }
        
        col++;
        if (col >= 7)
        {
            col = 0;
            row++;
        }
    }
}

/**
 * @brief Vẽ trang lịch đầy đủ (phiên bản gốc, duy trì tính tương thích)
 * @param unix_time Dấu thời gian Unix hiện tại
 */
void draw_calendar_page(uint32_t unix_time)
{
    tm_t tm;
    transformTime(unix_time, &tm);
    
    uint16_t year = tm.tm_year + YEAR0;
    uint8_t month = tm.tm_mon + 1;
    uint8_t current_day = tm.tm_mday;
    
    // Xóa canvas
    Paint_Clear(WHITE);
    
    // Vẽ tiêu đề lịch
    draw_calendar_title(year, month);
    
    // Vẽ dòng tiêu đề các thứ trong tuần
    draw_week_header();
    
    // Vẽ lưới lịch
    //draw_calendar_grid();
    
    // Vẽ các con số ngày
    draw_calendar_dates(year, month, current_day);
    
    // Hiển thị thời gian hiện tại ở dưới cùng (định dạng số)
    char time_buf[20];
    sprintf(time_buf, "%02d:%02d", tm.tm_hour, tm.tm_min);
    EPD_DrawUTF8(150, 110, 1, time_buf, EPD_ASCII_11X16, EPD_FontUTF8_16x16, BLACK, WHITE);
}

/**
 * @brief Vẽ trang lịch kèm đồng hồ kim (phiên bản mới)
 * @param unix_time Dấu thời gian Unix hiện tại
 * @param force_redraw Có bắt buộc vẽ lại đồng hồ hay không
 */

 /*
 void draw_calendar_page_with_analog_clock(uint32_t unix_time, bool force_redraw)
{
    tm_t tm;
    transformTime(unix_time, &tm);
    
    uint16_t year = tm.tm_year + YEAR0;
    uint8_t month = tm.tm_mon + 1;
    uint8_t current_day = tm.tm_mday;
    
    if (force_redraw)
    {
        // Xóa canvas khi vẽ lại hoàn toàn
        Paint_Clear(WHITE);
        
        // Vẽ tiêu đề lịch
        draw_calendar_title(year, month);
        
        // Vẽ dòng tiêu đề các thứ trong tuần
        draw_week_header();
        
        // Vẽ lưới lịch
        //draw_calendar_grid();
        
        // Vẽ các con số ngày
        draw_calendar_dates(year, month, current_day);
    }
    
    // Vẽ đồng hồ kim ở bên phải (120x120 pixel)
    // Điều chỉnh vị trí theo kích thước màn hình: màn hình rộng 290
    uint16_t clock_x = 120; // Vị trí bên phải
    uint16_t clock_y = 12;  // Căn giữa theo chiều dọc
    uint16_t clock_size = 60;
    
    draw_analog_clock(clock_x, clock_y, clock_size, unix_time, force_redraw);
}
 */


 