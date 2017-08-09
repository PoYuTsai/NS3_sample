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
log.txt.log        //產生log檔

script log/log.txt // 在scratch 裡的log資料夾產生log檔案，方便等等執行完抓取實驗數據

exit

sudo ./waf --run scratch/third.cc 

3.如果要開啟影像模擬：sudo ./waf --run scratch/wifi-tcp --vis

4.在/ns-allinone-3.26/ns-3.26/log 以終端機模擬打入：

sudo ns.py //跑出實驗數據

## 模擬內容
量測封包間隔延遲

