/**
 * @file rtc.c
 * @brief ʵʱʱ�ӣ�RTC��������ʵ���ļ�
 * @details ʵ���� RTC ��ʼ��������ʱ�������ʱ��ͻ�ȡ��ǰʱ��ȹ���
 */

#include "rtc.h"


// ��ʼ�� RTC
void RTC_Init(void) {
	// ʹ�� PWR �� BKP ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    // ʹ�ܶ� BKP �Ĵ����ķ���
    PWR_BackupAccessCmd(ENABLE);
    // ��λ��������
    BKP_DeInit();

    // ����ʹ�� LSE ��Ϊʱ��Դ
    RCC_LSEConfig(RCC_LSE_ON);
    uint32_t timeout = 0xFFFF;
    // �ȴ� LSE �ȶ�
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET && timeout > 0) {
        timeout--;
    }

    if (timeout == 0) {
        // LSE δ������ʹ�� LSI ��Ϊʱ��Դ
        RCC_LSICmd(ENABLE);
        while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
        // ���� RTC Ԥ��Ƶֵ��ʹ�� LSI��LSI Ƶ��ԼΪ 40kHz��
        RTC_SetPrescaler(39999); 
    } else {
        // LSE ����ɹ���ʹ�� LSE ��Ϊʱ��Դ
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        // ���� RTC Ԥ��Ƶֵ��ʹ�� LSE��LSE Ƶ��Ϊ 32.768kHz��
        RTC_SetPrescaler(32767); 
    }

    // ʹ�� RTC ʱ��
    RCC_RTCCLKCmd(ENABLE);
    // �ȴ� RTC �Ĵ���ͬ��
    RTC_WaitForSynchro();
    // �ȴ���һ�ζ� RTC �Ĵ�����д�������
    RTC_WaitForLastTask();
}

uint8_t RTC_SetTimeByTimestamp(uint64_t timestamp) {
    // ת��Ϊ�뼶ʱ���
    time_t time_sec = timestamp / 1000;

    // ���ʱ����Ƿ񳬳�RTC֧�ַ�Χ
    if (time_sec > UINT32_MAX) {
        return 0; // ʱ���������Χ
    }

    // ֱ������RTC��������ֵ
    RTC_WaitForLastTask();
    RTC_SetCounter((uint32_t)time_sec);
    RTC_WaitForLastTask();

    return 1; // ���óɹ�
}

// ��ȡ��ǰ RTC ʱ�䣨����ʱ�䣩
void RTC_GetCurrentTime(Time_t *time) {
    // ��ȡRTC��������ֵ
    uint32_t rtc_counter = RTC_GetCounter();
    time_t utc_time = (time_t)rtc_counter;
    struct tm *tm_info;

    // ʹ��gmtime��ȡUTCʱ��
    tm_info = localtime(&utc_time);

    if (tm_info != NULL) {
        // ����ʱ��ƫ��Ϊ8Сʱ������ʱ�䣩
        int timezone_offset = 8; 

        // ����ƫ�ƺ��ʱ��
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
            // ����������������Ĭ��ֵ
            memset(time, 0, sizeof(Time_t));
        }
    } else {
        // ����������������Ĭ��ֵ
        memset(time, 0, sizeof(Time_t));
    }
}
