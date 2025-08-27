/**
 * @file rtc.h
 * @brief 实时时钟（RTC）驱动的头文件
 * @details 定义了时间结构体和 RTC 相关函数的声明
 */

#ifndef RTC_H
#define RTC_H

#include "system_config.h"

/**
 * @brief 时间结构体，用于存储年、月、日、星期、时、分、秒
 */
typedef struct {
    uint16_t year;  /**< 年份 */
    uint8_t month;  /**< 月份 */
    uint8_t day;    /**< 日期 */
    uint8_t weekday;/**< 星期，0 表示周日，1 表示周一，以此类推 */
    uint8_t hour;   /**< 小时 */
    uint8_t minute; /**< 分钟 */
    uint8_t second; /**< 秒 */
} Time_t;

/**
 * @brief 初始化 RTC 模块
 * @details 配置 RTC 时钟源、使能 RTC 外设等操作
 * @return 无
 */
void RTC_Init(void);

/**
 * @brief 根据毫秒时间戳设置 RTC 时间
 * @details 将毫秒时间戳转换为年、月、日、时、分、秒，并设置到 RTC 中
 * @param timestamp 毫秒时间戳
 * @return 若时间戳有效则返回 1，无效则返回 0
 */
uint8_t RTC_SetTimeByTimestamp(uint64_t timestamp);

/**
 * @brief 获取当前 RTC 时间
 * @details 从 RTC 中读取当前时间，并存储到时间结构体中
 * @param time 指向时间结构体的指针，用于存储获取到的时间
 * @return 无
 */
void RTC_GetCurrentTime(Time_t *time);

#endif /* RTC_H */
