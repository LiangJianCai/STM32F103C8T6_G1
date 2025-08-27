/**
 * @file rtc.h
 * @brief ʵʱʱ�ӣ�RTC��������ͷ�ļ�
 * @details ������ʱ��ṹ��� RTC ��غ���������
 */

#ifndef RTC_H
#define RTC_H

#include "system_config.h"

/**
 * @brief ʱ��ṹ�壬���ڴ洢�ꡢ�¡��ա����ڡ�ʱ���֡���
 */
typedef struct {
    uint16_t year;  /**< ��� */
    uint8_t month;  /**< �·� */
    uint8_t day;    /**< ���� */
    uint8_t weekday;/**< ���ڣ�0 ��ʾ���գ�1 ��ʾ��һ���Դ����� */
    uint8_t hour;   /**< Сʱ */
    uint8_t minute; /**< ���� */
    uint8_t second; /**< �� */
} Time_t;

/**
 * @brief ��ʼ�� RTC ģ��
 * @details ���� RTC ʱ��Դ��ʹ�� RTC ����Ȳ���
 * @return ��
 */
void RTC_Init(void);

/**
 * @brief ���ݺ���ʱ������� RTC ʱ��
 * @details ������ʱ���ת��Ϊ�ꡢ�¡��ա�ʱ���֡��룬�����õ� RTC ��
 * @param timestamp ����ʱ���
 * @return ��ʱ�����Ч�򷵻� 1����Ч�򷵻� 0
 */
uint8_t RTC_SetTimeByTimestamp(uint64_t timestamp);

/**
 * @brief ��ȡ��ǰ RTC ʱ��
 * @details �� RTC �ж�ȡ��ǰʱ�䣬���洢��ʱ��ṹ����
 * @param time ָ��ʱ��ṹ���ָ�룬���ڴ洢��ȡ����ʱ��
 * @return ��
 */
void RTC_GetCurrentTime(Time_t *time);

#endif /* RTC_H */
