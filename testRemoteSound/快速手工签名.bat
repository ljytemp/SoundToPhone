set ZIPALIGN_PATH=C:\adt-bundle-windows-x86-20131030\sdk\tools
@Rem ���ļ���ǩ���ļ�ͬĿ¼(��Ҫljy)
@Rem androidǩ������ //ע��ָ��
@Rem echo����ʾָ�� ��ʽ��echo [{on|off}] [message]
@echo **********************************************************
@Rem �ļ��Ƿ���������ʽ��if exist ·��+�ļ��� ����
@if exist android.keystore goto sign
@Rem jdk ��װ��C:\Program Files (x86)\Java\jdk1.6.0_02\binĿ¼���Ǹ��޴�Ĵ����пո�
@echo ����ǩ���ļ�android.keystore
@Rem keytool�����ʽ��-genkey����ǩ�� -alias���� -keyalg�����㷨 -validity��Ч���� -keystore����ǩ���ļ�����
@Rem ljy�Ѿ�����ǩ���ļ��� keytool -genkey -alias android.keystore -keyalg RSA -validity 40000 -keystore android.keystore
@echo ��ʼǩ����
@Rem jarsigner�����ʽ��-verbose�����ϸ��Ϣ -keystore��Կ��λ�� -signedjarҪ���ɵ��ļ� Ҫǩ�����ļ� ��Կ���ļ�����
@Rem
@Rem ljy:jarsigner -verbose -keystore android.keystore -storepass 123456ljy -signedjar temp_signed.apk objects\bin\temp.ap_ android.keystore
@Rem
@Rem
@Rem
@goto over
@Rem
@Rem
@Rem
:sign
@echo ��ʼǩ����
jarsigner -verbose -keystore android.keystore -storepass 123456ljy -signedjar temp_signed.apk objects\bin\temp.ap_ android.keystore
%ZIPALIGN_PATH%\zipalign -f -v 4 temp_signed.apk temp_zip.apk
%ZIPALIGN_PATH%\zipalign -c -v 4 temp_zip.apk
@Rem
@Rem
@Rem
@Rem
:over
@echo ********************temp.ap_ ǩ�����************************
pause
