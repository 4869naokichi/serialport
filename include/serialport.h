/**
 * @file serialport.cpp
 * @brief Windows APIを用いてシリアル通信を行う
 * @author naokichi
 * @date 2023/02/27
 */

#pragma once

#include <tchar.h>
#include <windows.h>

#include <cstring>
#include <iostream>

#include "speak.h"

/**
 * @brief COMポートをセットアップする
 * @param port シリアルポート名
 * @param baudrate ボーレート
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL SetupComPort(std::string port, DWORD baudrate);

/**
 * @brief 1バイトのデータを送信する
 * @param txData 送信するデータ
 * @retval TRUE 送信成功
 * @retval FALSE 送信失敗
 */
BOOL WriteByte(uint8_t txData);

/**
 * @brief 1バイトのデータを受信する
 * @return 受信したデータ
 * @note ポーリングで受信する
 */
uint8_t ReadByte(void);

/**
 * @brief 文字列を送信する
 * @param txData 送信する文字列
 * @retval TRUE 送信成功
 * @retval FALSE 送信失敗
 */
BOOL WriteLine(const std::string txData);

/**
 * @brief 文字列を受信する
 * @return 受信した文字列
 * @note ポーリングで受信する
 */
std::string ReadLine(void);

/**
 * @brief float型の数値を送信する
 * @param f 送信する数値
 * @retval TRUE 送信成功
 * @retval FALSE 送信失敗
 */
BOOL WriteFloat(float f);