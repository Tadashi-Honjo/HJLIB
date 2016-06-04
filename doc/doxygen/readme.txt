//===============================================//
//	doxygenについて								 //
//									2014/04/14   //
//									honjo		 //
//===============================================//
＜ダウンロード＞
http://www.stack.nl/~dimitri/doxygen/download.html#latestsrc
doxygen-(バージョン)-setup.exeをダウンロード。

＜インストール＞
doxygen-(バージョン)-setup.exe起動し、順にインストールします。

＜設定＞
・Wizardタブ->Project
Project name			・・・プロジェクト
Project synopsis		・・・プロジェクトの説明?
Project version or id	・・・ver1.0
Source code directory	・・・ドキュメント化するディレクトリフォルダを設定します。(階層的にスキャンする場合はScan recursivelyをチェック)
Destination directory	・・・ドキュメント出力フォルダ

・Wizardタブ->Output
LaTexのチェックを外す。

・Expertタブ->Project
DOXYFILE_ENCORDING・・・「UTF-8」を「CP932」に変更
OUTPUT_LANGUAGE・・・「Japanese」に変更

・Expertタブ->Input
INPUT_ENCORDING・・・「UTF-8」を「CP932」に変更

＜出力＞
・Runタブの「Run Doxygen」を押すと出力します。