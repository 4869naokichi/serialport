/**
 * @file serialport.cpp
 * @brief Windows APIを用いてシリアル通信を行う
 * @author naokichi
 * @date 2023/02/27
 */

#include <serialport.h>

static HANDLE hComPort;

/**
 * @brief COMポートをセットアップする
 * @param port シリアルポート名
 * @param baudrate ボーレート
 * @retval TRUE 成功
 * @retval FALSE 失敗
 */
BOOL SetupComPort(std::string port, DWORD baudrate) {
  hComPort = CreateFileA(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
                         OPEN_EXISTING, 0, NULL);
  if (hComPort == INVALID_HANDLE_VALUE) {
    std::cout << "COMポートが開けません。" << std::endl;
    Speak("COMポートが開けません。");
    CloseHandle(hComPort);
    return FALSE;
  } else {
    std::cout << "COMポートは正常に開けました。" << std::endl;
    Speak("COMポートは正常に開けました。");
  }

  BOOL retval;
  retval = SetupComm(hComPort, 1024, 1024);
  if (retval == FALSE) {
    std::cout << "送受信バッファの設定ができません。" << std::endl;
    Speak("送受信バッファの設定ができません。");
    CloseHandle(hComPort);
    return FALSE;
  } else {
    std::cout << "送受信バッファの設定が完了しました。" << std::endl;
    Speak("送受信バッファの設定が完了しました。");
  }

  retval = PurgeComm(
      hComPort, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
  if (retval == FALSE) {
    std::cout << "送受信バッファの初期化ができません。" << std::endl;
    Speak("送受信バッファの初期化ができません。");
    CloseHandle(hComPort);
    return FALSE;
  } else {
    std::cout << "送受信バッファの初期化が完了しました。" << std::endl;
    Speak("送受信バッファの初期化が完了しました。");
  }

  DCB dcb;
  GetCommState(hComPort, &dcb);

  dcb.DCBlength = sizeof(DCB);
  dcb.BaudRate = baudrate;
  dcb.ByteSize = 8;
  dcb.fBinary = TRUE;
  dcb.fParity = NOPARITY;
  dcb.StopBits = ONESTOPBIT;
  dcb.fOutxCtsFlow = FALSE;
  dcb.fOutxDsrFlow = FALSE;
  dcb.fDtrControl = DTR_CONTROL_DISABLE;
  dcb.fRtsControl = RTS_CONTROL_DISABLE;

  dcb.fOutX = FALSE;
  dcb.fInX = FALSE;
  dcb.fTXContinueOnXoff = TRUE;
  dcb.XonLim = 512;
  dcb.XoffLim = 512;
  dcb.XonChar = 0x11;
  dcb.XoffChar = 0x13;

  dcb.fNull = TRUE;
  dcb.fAbortOnError = TRUE;
  dcb.fErrorChar = FALSE;
  dcb.ErrorChar = 0x00;
  dcb.EofChar = 0x03;
  dcb.EvtChar = 0x02;

  retval = SetCommState(hComPort, &dcb);
  if (retval == FALSE) {
    std::cout << "COMポート構成情報の変更に失敗しました。" << std::endl;
    Speak("COMポート構成情報の変更に失敗しました。");
    CloseHandle(hComPort);
    return FALSE;
  } else {
    std::cout << "COMポート構成情報を変更しました。" << std::endl;
    Speak("COMポート構成情報を変更しました。");
  }

  COMMTIMEOUTS TimeOut;
  GetCommTimeouts(hComPort, &TimeOut);

  TimeOut.ReadTotalTimeoutMultiplier = 0;
  TimeOut.ReadTotalTimeoutConstant = 1000;
  TimeOut.WriteTotalTimeoutMultiplier = 0;
  TimeOut.WriteTotalTimeoutConstant = 1000;

  retval = SetCommTimeouts(hComPort, &TimeOut);
  if (retval == FALSE) {
    std::cout << "タイムアウトの設定に失敗しました。" << std::endl;
    Speak("タイムアウトの設定に失敗しました。");
    CloseHandle(hComPort);
    return FALSE;
  } else {
    std::cout << "タイムアウトの設定に成功しました。" << std::endl;
    Speak("タイムアウトの設定に成功しました。");
  }

  return TRUE;
}

/**
 * @brief 1バイトのデータを送信する
 * @param txData 送信するデータ
 * @retval TRUE 送信成功
 * @retval FALSE 送信失敗
 */
BOOL WriteByte(uint8_t txData) {
  DWORD writeSize;
  BOOL retval = WriteFile(hComPort, &txData, 1, &writeSize, NULL);
  return retval;
}

/**
 * @brief 1バイトのデータを受信する
 * @return 受信したデータ
 * @note ポーリングで受信する
 */
uint8_t ReadByte() {
  uint8_t rxData;
  DWORD readSize;
  while (1) {
    ReadFile(hComPort, &rxData, 1, &readSize, 0);
    if (readSize == 1) {
      return rxData;
    } else {
      Sleep(10);
    }
  }
}

/**
 * @brief 文字列を送信する
 * @param txData 送信する文字列
 * @retval TRUE 送信成功
 * @retval FALSE 送信失敗
 */
BOOL WriteLine(const std::string txData) {
  DWORD writeSize;
  BOOL retval =
      WriteFile(hComPort, txData.c_str(), txData.size(), &writeSize, NULL);
  return retval;
}

/**
 * @brief 文字列を受信する
 * @return 受信した文字列
 * @note ポーリングで受信する
 */
std::string ReadLine(void) {
  std::string rxData;
  for (int i = 0; i < 256; i++) {
    rxData += ReadByte();
    if (rxData.back() == '\n') break;
  }
  return rxData;
}

/**
 * @brief float型の数値を送信する
 * @param f 送信する数値
 * @retval TRUE 送信成功
 * @retval FALSE 送信失敗
 */
BOOL WriteFloat(float f) {
  uint8_t txData[4];
  std::memcpy(txData, &f, 4);
  DWORD writeSize;
  BOOL retval = WriteFile(hComPort, txData, 4, &writeSize, NULL);
  return retval;
}