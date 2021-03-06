本プロジェクトファイルはAP-RX62N-0A向けサンプルプログラムを学習目的で改変、改造したものになります。

******************************************************************************
    μNet3/Compact         AP-RX62N-0A 向けサンプルプログラム
    Copyright (c)  2012, eForce Co., Ltd. All rights reserved.
******************************************************************************

・本ファイルは、サンプルプログラムの概要、及び専用デバイスドライバを含む場合は
  その概要、そのほか、更新履歴などの情報を記述しています。

１．サンプルプログラムの概要
------------------------------------------------------------------------------
DHCPクライアント, HTTPサーバー, DNSクライアント, SNTPクライアント, ping送受信の
各ネットワークアプリを動作させるサンプルプログラムです。
ただしDNSクライアント, SNTPクライアント, pingはHTTPクライアント(ブラウザ)から
実行を要求するためHTTPサーバーが正常に動作する必要があります。

・DHCPクライアント
DHCPサーバから動的にIPアドレスを取得し自ホストに割り当てます。
コンフィグレータのIPアドレス取得機能を使って割り当てられたIPアドレスを確認する
ことができます。


・HTTPサーバー
ウェブブラウザからCGIを使って次の操作が可能です。

    1.ターゲットボードのLED点滅間隔を変更します。
       => ボード上の「MON」LEDが点灯と消灯を繰り返します。
 
    2.指定したIPv4アドレスに対してpingを送信し応答があるかを確認します。
    3.NTPサーバーからNTP時刻を取得して表示します。
    4.DNS名からIPv4アドレスを取得して表示します。

ウェブブラウザからターゲットボードへアクセスするには、ボードのIPアドレスを指定
して以下のようにURLを入力します。
（IPアドレスが不明な場合はコンフィグレータのIPアドレス取得機能を使用します)

例）http://192.168.x.x/


・ping
ping(ICMP Echo要求)の送信先のIPv4アドレスを入力しPINGボタンを押下します。
3秒以内に相手から応答が得られれば「Success reply from 192.168.x.x」と表示され
ます。応答が得られない場合は「No response from 192.168.x.x」と表示されます。


・DNSクライアント

DNS名とDNSサーバーアドレスを指定して、DNS名のIPv4アドレスを解決します。


・SNTPクライアント

NTPパケットを利用してネットワーク上の時刻サーバー（NTPサーバー）から NTP時刻を
取得します。
情報通信研究機構(NICT)が公開しているNTPサーバー名を以下に記します。
NTPサーバーはIPv4アドレスで指定する必要があるため、これらのIPv4アドレスを解決
して設定して下さい。

    ntp.jst.mfeed.ad.jp
    ntp1.jst.mfeed.ad.jp
    ntp2.jst.mfeed.ad.jp


２．ロードモジュールの生成までの手順
------------------------------------------------------------------------------
コンフィグレータμC3/Configuratorによりファイルを生成し、CubeSuite+によりロード
モジュールを生成し、オンチップデバッギングエミュレータE1を使用してデバッグを
実施することができます。

・ファイルの生成
μC3/Configuratorを起動し、サンプルフォルダのuC3Project_net.3cfをプロジェクトファ
イルとして開きます。次に「プロジェクト」→「ソース生成」を実施し、「フォルダの選択」
ではプロジェクトファイルのあるフォルダを選択し、必要なフォルダを生成します。
　この操作で、μC3/Compactを実行するためのソースコードが生成されます。
　なお、この際にサンプルプログラムの実行では、生成されるスケルトンコードmain.cは
使用せず、必要なコードを書き足したsample_net.cを使用しています。

・ロードモジュールの生成
CubeSuite+を起動し、「プロジェクト」→「プロジェクトを開く」により、サンプルフォルダ
の「\AP_RX62N_0A.NET\ap_rx62n.mtpj」プロジェクトファイルを開きます。
その後、「ビルド」→「ビルド・プロジェクト」によりロードモジュール「ap_rx62n.abs」と
「ap_rx62n.mot」を生成します。

・デバッグの実行
CubeSuite+の「デバッグ」→「デバッグ・ツールへダウンロード」を実行することで、
ロードモジュールがデバイスに書き込まれ動作確認を実行することが出来ます。

３．更新履歴
------------------------------------------------------------------------------
    Version Information  
    2012.11: Created   
             Renesas  CubeSuite+ v1.03.00 に対応 
