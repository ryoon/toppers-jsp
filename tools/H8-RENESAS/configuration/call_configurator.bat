echo off
rem システム・コンフィギュレーションとベクタテーブルの自動生成

rem HEWでは、相対パス指定で外部ツールを呼び出せないため、
rem バッチファイルを経由して、コンフィギュレータを呼び出している
rem
rem システム・コンフィギュレーション・ファイルがプリプロセスされた
rem 結果のファイル名が、引数%1としてHEWからこのバッチファイルに渡される。

rem 以下の部分は移植の際に変更する必要がある
rem システム依存部のディレクトリ名
rem cfg.exe -s %1 -c -obj -cpu h8 -system xxxxxxxx

rem ベクタテーブルのサイズ
rem genvector.pl -s xxx

echo on

..\..\..\..\cfg\cfg.exe -s %1 -c -obj -cpu h8 -system hsb8f3048bf25
perl ..\..\..\..\utils\h8\filter1.pl %1 > tmpfile2
perl ..\..\..\..\utils\h8-renesas\filter2.pl tmpfile2 > tmp_script.pl
type ..\..\..\..\config\h8-renesas\vector_header.src > vector.src
perl ..\..\..\..\utils\h8-renesas\genvector.pl -s 61 >> vector.src
del tmpfile2 tmp_script.pl
