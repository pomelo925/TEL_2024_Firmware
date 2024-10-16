# 2024 東京威力科創機器人大賽

[官方簡章](http://teltwrobotcombat.rs-event.com.tw/a04.html#)


隊伍名稱： DIT_明天再做（75%）

隊員：黃興佑、汪柏叡、張修齊

</br>

**本人負責 Github 中之所有項目**，即全韌體開發（含馬達控制、策略、排程等）。

兩位隊友負責機構及電路。

</br>

## 架構

> 使用兩片 STM32H723ZG 微控制器運算。
> 全程遙控，不使用上位機。

* TEL24_Master
  * 接收遙控器訊號、傳輸遙控訊號給 `TEL24_Auxiliary`
  * 控制底盤

* TEL24_Auxiliary
  * 接收 `TEL24_Master` 傳輸遙控訊號（USART）
  * 控制砲台、轉盤

</br>

## 細項

1. 類比遙控器接收晶片，預計輸出 10 個 PWM 訊號通道，最少 8 個。
2. `TEL24_Master` 與 `TEL24_Auxiliary` 通訊採 USART。
因受限於現有邏輯版的接口設計，原希望採 ETH 通訊。
3. 部分控制迴路，未來可能不使用 Encoder。
4. 無刷馬達 FOC 控制已集成於馬達內部，只須輸出 PWM。

</br>

## 型錄

* 底盤
  * 直流馬達（x2）：驅動版-`Polulu G2 24V21`、4-pin 編碼器
* 雙砲台
  * 直流馬達（x4）：驅動版-`Polulu G2 24V21`、4-pin 編碼器
  * 步進馬達（x2）：驅動版-`TMC 2208`
  * 伺服馬達（x2）：UART
* 轉盤
  * 直流馬達（x2）：驅動版-`Polulu G2 24V21`、6-pin 編碼器-`OID-V2,3808B-12SIS`、蝸桿傳動

</br>

## 日誌

歡迎追蹤我的 [Threads](https://www.threads.net/@liebestraum_925) !