-----------------------------------------------------------------------------------------------------------<br/>
�o����socket programming�@�~,�ڪ��D�D�O�@��²�����󥭥xsocket api<br/>
�D�n���ѤT�Ӽҫ�,��@��²�����h�uServer,�G��²�������d��Client,�T��²����WebServer<br/>
�èϥΦ�api���T�Ӽҫ��\��U�ۼ��g�F�@��example��Demo<br/>
���O�ثeWebServer�ҫ��bWindows�W�������U�ذ��D,�D�n�O�]���ĥ�Windows��CreateThread�N��fork�����Y<br/>
����|�Q��k�ѨM,�ثe�l�ͪ����D���bWindows�W�ĥΦ��ҫ����g��WebServer�{��,�b�s����,IE�s�����W�|�L�^��</br>
��Chrome�s�������ɷ|���^��,���ɵL�^��.<br/>
�bLinux�t�Ω�Mac�W�ϥΦ��ҫ����g�νsĶ�X�Ӫ�WebServer�{���h�L�����D<br/>
<br/>
��api�z�L�@�ǫʸ�,���ϥΪ̯���Lc�y��²���a���g�㦳�h�u�\�઺²�����A���ݥH�Ψ㦳Timeout�\�઺²���Ȥ��<br/>
�t�~,��api�]�ʸˤF������HTTP1.0���A���\��,������ϥΪֳ̧t�]�p�@�Ө㦳�����A�ȥ\�઺���A����<br/>
<br/>
Demo�{����������,�@��²�檺DateTime���A���H�ΫȤ��<br/>
�G��²���������A�Ȧ��A��,�N�O�U���{�b���b�ϥΪ��o�ӥ\��<br/>

<br/>
-----------------------------------------------------------------------------------------------------------<br/>
<pre>
�sĶ���O:
make        �sĶ�Ҧ���Example
make all    �sĶ�Ҧ���Example
make ex1    �u�sĶExample1_Timeserver
make ex2    �u�sĶExample2_Timeclient
make ex3    �u�sĶExample3_Webserver
make clean  �M���Ҧ��sĶ���G

����:
Example1_Timeserver:
/* �b������port 9999���� */
./Timeserver
/* �b������port X���� */
./Timeserver X

Example2_Timeclient:
1.���Ұ�Timeserver
2.�Ұ�Timeclient
/* �s���쥻��port 9999��Timeserver */
./Timeserver
/* �s����ip��xxx.xxx.xxx.xxx,port��Y��Timeserver */
./Timeclient xxx.xxx.xxx.xxx Y
Example: ./Timeclient 192.168.1.100 9999

Example3_Webserver:(����port 10000,�i�ۦ�ק�source code)
1.���Ұ�Webserver�󥻾�(���]ip��xxx.xxx.xxx.xxx)
./Webserver
2.�}�ҥ��N�s����,����}�C��Jhttp://xxx.xxx.xxx.xxx:9999/
�`�N:�ثe�bWindows�W����WebServer�����s�����ݨ�|���ǰ��D,��ĳ�blinux��Mac�W����WebServer
</pre>
</br>
-----------------------------------------------------------------------------------------------------------<br/>