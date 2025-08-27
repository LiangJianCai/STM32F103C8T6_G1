/**
 * @file rtc.c
 * @brief 实时时钟（RTC）驱动的实现文件
 * @details 实现了 RTC 初始化、根据时间戳设置时间和获取当前时间等功能
 */

#include "rtc.h"


// 初始化 RTC
void RTC_Init(void) {
	// 使能 PWR 和 BKP 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    // 使能对 BKP 寄存器的访问
    PWR_BackupAccessCmd(ENABLE);
    // 复位备份区域
    BKP_DeInit();

    // 尝试使用 LSE 作为时钟源
    RCC_LSEConfig(RCC_LSE_ON);
    uint32_t timeout = 0xFFFF;
    // 等待 LSE 稳定
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && timeout > 0) {
        timeout--;
    }

    if (timeout == 0) {
        // LSE 未能起振，使用 LSI 作为时钟源
        RCC_LSICmd(ENABLE);
        while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
        // 设置 RTC 预分频值以使用 LSI（LSI 频率约为 40kHz）
        RTC_SetPrescaler(39999); 
    } else {
        // LSE 起振成功，使用 LSE 作为时钟源
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        // 设置 RTC 预分频值以使用 LSE（LSE 频率为 32.768kHz）
        RTC_SetPrescaler(32767); 
    }

    // 使能 RTC 时钟
    RCC_RTCCLKCmd(ENABLE);
    // 等待 RTC 寄存器同步
    RTC_WaitForSynchro();
    // 等待上一次对 RTC 寄存器的写操作完成
    RTC_WaitForLastTask();
}

uint8_t RTC_SetTimeByTimestamp(uint64_t timestamp) {
    // 转换为秒级时间戳
    time_t time_sec = timestamp / 1000;

    // 检查时间戳是否超出RTC支持范围
    if (time_sec > UINT32_MAX) {
        return 0; // 时间戳超出范围
    }

    // 直接设置RTC计数器的值
    RTC_WaitForLastTask();
    RTC_SetCounter((uint32_t)time_sec);
    RTC_WaitForLastTask();

    return 1; // 设置成功
}

// 获取当前 RTC 时间（本地时间）
void RTC_GetCurrentTime(Time_t *time) {
    // 获取RTC计数器的值
    uint32_t rtc_counter = RTC_GetCounter();
    time_t utc_time = (time_t)rtc_counter;
    struct tm *tm_info;

    // 使用gmtime获取UTC时间
    tm_info = localtime(&utc_time);

    if (tm_info != NULL) {
        // 假设时区偏移为8小时（北京时间）
        int timezone_offset = 8; 

        // 计算偏移后的时间
        time_t local_time = utc_time + timezone_offset * 3600;
        struct tm *local_tm = localtime(&local_time);

        if (local_tm != NULL) {
            time->year = local_tm->tm_year + 1900;
            time->month = local_tm->tm_mon + 1;
            time->day = local_tm->tm_mday;
            time->weekday = local_tm->tm_wday;
            time->hour = local_tm->tm_hour;
            time->minute = local_tm->tm_min;
            time->second = local_tm->tm_sec;
        } else {
            // 处理错误情况，设置默认值
            memset(time, 0, sizeof(Time_t));
        }
    } else {
        // 处理错误情况，设置默认值
        memset(time, 0, sizeof(Time_t));
    }
}
