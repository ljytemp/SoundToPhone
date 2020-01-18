set ZIPALIGN_PATH=C:\adt-bundle-windows-x86-20131030\sdk\tools
@Rem 本文件和签名文件同目录(重要ljy)
@Rem android签名程序 //注释指令
@Rem echo是显示指令 格式：echo [{on|off}] [message]
@echo **********************************************************
@Rem 文件是否存在命令格式：if exist 路径+文件名 命令
@if exist android.keystore goto sign
@Rem jdk 安装在C:\Program Files (x86)\Java\jdk1.6.0_02\bin目录，是个巨大的错误，有空格
@echo 创建签名文件android.keystore
@Rem keytool命令格式：-genkey产生签名 -alias别名 -keyalg加密算法 -validity有效天数 -keystore生产签名文件名称
@Rem ljy已经做成签名文件了 keytool -genkey -alias android.keystore -keyalg RSA -validity 40000 -keystore android.keystore
@echo 开始签名：
@Rem jarsigner命令格式：-verbose输出详细信息 -keystore密钥库位置 -signedjar要生成的文件 要签名的文件 密钥库文件别名
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
@echo 开始签名：
jarsigner -verbose -keystore android.keystore -storepass 123456ljy -signedjar temp_signed.apk objects\bin\temp.ap_ android.keystore
%ZIPALIGN_PATH%\zipalign -f -v 4 temp_signed.apk temp_zip.apk
%ZIPALIGN_PATH%\zipalign -c -v 4 temp_zip.apk
@Rem
@Rem
@Rem
@Rem
:over
@echo ********************temp.ap_ 签名完成************************
pause
