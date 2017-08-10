# Ns3
## 安裝教學
安裝穩定版本：

下載：https://github.com/Lu-Yi-Hsun/Ns3/tree/master/install/install-ns3.sh
執行安裝
```
sudo sh install-ns3.sh
```


```
git clone https://github.com/nsnam/ns-3-dev-git.git
./waf configure --enable-examples
./waf

```

## 使用教學
1.把要執行的程式放入/ns-allinone-3.26/ns-3.26/scratch

2.在/ns-allinone-3.26/ns-3.26目錄 以終端機模擬打入：

script log/log.txt                  //在log資料夾裡產生一個log檔，方便紀錄實驗數據



sudo ./waf --run scratch/third     //執行程式，檔名third

exit                               //結束


3.在/ns-allinone-3.26/ns-3.26/log 以終端機模擬打入：

pythin ns.py                       //跑出實驗數據(用python小程式抓取”數字“，方便貼到excel畫圖)

## 模擬內容
量測封包間隔延遲，實驗結果顯示在時間間隔為一秒時會趨於穩定，低於一秒時封包丟失率為72%，而在約三秒的時候丟失率約為34%。
（此為UDP的做法）

