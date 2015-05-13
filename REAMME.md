-----------------------------------------------------------------------------------------------------------<br/>
這次的socket programming作業,我的主題是一個簡易的跨平台socket api<br/>
主要提供三個模型,其一為簡易的多工Server,二為簡易的防卡死Client,三為簡易的WebServer<br/>
並使用此api的三個模型功能各自撰寫了一個example來Demo<br/>
但是目前WebServer模型在Windows上貌似有各種問題,主要是因為採用Windows的CreateThread代替fork的關係<br/>
之後會想辦法解決,目前衍生的問題為在Windows上採用此模型撰寫的WebServer程式,在瀏覽時,IE瀏覽器上會無回應</br>
而Chrome瀏覽器有時會有回應,有時無回應.<br/>
在Linux系統或Mac上使用此模型撰寫及編譯出來的WebServer程式則無此問題<br/>
<br/>
此api透過一些封裝,讓使用者能夠過c語言簡易地撰寫具有多工功能的簡易伺服器端以及具有Timeout功能的簡易客戶端<br/>
另外,此api也封裝了部分的HTTP1.0伺服器功能,能夠讓使用者快速設計一個具有網頁服務功能的伺服器端<br/>
<br/>
Demo程式分為兩類,一為簡單的DateTime伺服器以及客戶端<br/>
二為簡易的網頁服務伺服器,就是各為現在正在使用的這個功能<br/>

<br/>
-----------------------------------------------------------------------------------------------------------<br/>
<pre>
編譯指令:
make        編譯所有的Example
make all    編譯所有的Example
make ex1    只編譯Example1_Timeserver
make ex2    只編譯Example2_Timeclient
make ex3    只編譯Example3_Webserver
make clean  清除所有編譯結果

執行:
Example1_Timeserver:
/* 在本機的port 9999執行 */
./Timeserver
/* 在本機的port X執行 */
./Timeserver X

Example2_Timeclient:
1.先啟動Timeserver
2.啟動Timeclient
/* 連接到本機port 9999的Timeserver */
./Timeserver
/* 連接到ip為xxx.xxx.xxx.xxx,port為Y的Timeserver */
./Timeclient xxx.xxx.xxx.xxx Y
Example: ./Timeclient 192.168.1.100 9999

Example3_Webserver:(佔用port 10000,可自行修改source code)
1.先啟動Webserver於本機(假設ip為xxx.xxx.xxx.xxx)
./Webserver
2.開啟任意瀏覽器,於網址列輸入http://xxx.xxx.xxx.xxx:9999/
注意:目前在Windows上執行WebServer的話瀏覽器看到會有些問題,建議在linux或Mac上執行WebServer
</pre>
</br>
-----------------------------------------------------------------------------------------------------------<br/>