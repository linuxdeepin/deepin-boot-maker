# 啟動碟製作工具|deepin-boot-maker|

## 概述

啟動碟製作工具是一款將系統映像檔案寫入USB手指等安裝媒介的小工具，介面簡潔、操作簡單。

## 使用入門

透過以下方式執行或關閉啟動碟製作工具，或建立工具捷徑。

### 執行啟動碟製作工具

1. 按一下工作列上的啟動器圖示 ![deepin_launcher](../common/deepin_launcher.svg)，進入啟動器介面。
2. 滾動滑鼠或搜尋，找到啟動碟製作工具圖示 ![deepin_boot_maker](../common/deepin_boot_maker.svg)，按一下執行。
3. 右鍵按一下 ![deepin_boot_maker](../common/deepin_boot_maker.svg) 可：
   - 按一下 **傳送到桌面**，建立桌面捷徑。
   - 按一下 **釘選到工作列**，將應用固定於工作列。
   - 按一下 **開機自動啟動**，將應用加入開機啟動項。

### 關閉啟動碟製作工具

- 按一下介面中的 ![close](../common/close.svg) 退出。
- 在工作列右鍵按一下 ![deepin_boot_maker](../common/deepin_boot_maker.svg)，選擇 **全部關閉**。
- 按一下介面中的 ![icon_menu](../common/icon_menu.svg)，選擇 **退出**。

## 操作介紹

### 準備工作

1. 到官網下載最新的系統映像檔案。
2. 準備一個至少8GB的USB手指。
3. 獲取啟動碟製作工具：
   - 系統預裝了啟動碟製作工具，在啟動器中查找。
   - 訪問官網下載指定版本的啟動碟製作工具。
   - Windows系統可以從光碟映像中解壓 xxx_boot_maker.exe 到電腦中。

### 選擇映像檔案

您可以通過以下兩種方式選擇映像檔案：
- 按一下 **請選擇映像檔案**，選中已下載的映像檔案後按一下 **下一步**。
- 直接將映像檔案拖拽到製作工具介面，然後按一下 **下一步**。

<img src="fig/select_file.png" alt="0|boot_maker" style="zoom:67%;" />

### 選擇分割區

選中製作啟動碟的USB手指，如果USB手指有多個分割區，只能選擇第一個分割區。按一下 **開始製作** 或 **返回** 按鈕。
- 開始製作：建議勾選 **格式化分割區**，製作前請保存好USB手指的數據。
- 返回：返回到選擇映像介面。

<img src="fig/select_disk.png" alt="0|select_disk" style="zoom:67%;" />

### 製作成功

製作過程需要一定的時間，請耐心等待，製作時請不要移除USB手指或關機。待進度條滿格，出現 **製作成功** 的提示，表明啟動碟已經製作完成。

<img src="fig/boot_making.png" alt="boot_maker" style="zoom:67%;" />

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;

### 製作失敗

如果製作失敗，可能由如下情況導致：
- 磁碟格式錯誤
- 磁碟空間不足
- 磁碟掛載錯誤
- 解壓映像錯誤

當出現未知錯誤時，製作失敗視窗會出現「提交回饋」按鈕。按一下 **提交回饋** 後，跳轉到相關論壇進行問題回饋。

## 主菜單

在主菜單中，您可以切換視窗主題、查看說明手冊等。

### 主題
視窗主題包含淺色主題、深色主題和系統主題。
1. 在啟動碟製作工具介面，按一下 ![icon_menu](../common/icon_menu.svg)。
2. 按一下 **主題**，選擇一個主題顏色。

### 說明
1. 在啟動碟製作工具介面，按一下 ![icon_menu](../common/icon_menu.svg)。
2. 按一下 **說明**，查看說明手冊，進一步了解和使用啟動碟製作工具。

### 關於
1. 在啟動碟製作工具介面，按一下 ![icon_menu](../common/icon_menu.svg)。
2. 按一下 **關於**，查看啟動碟製作工具的版本和介紹。

### 退出
1. 在啟動碟製作工具介面，按一下 ![icon_menu](../common/icon_menu.svg)。
2. 按一下 **退出**。